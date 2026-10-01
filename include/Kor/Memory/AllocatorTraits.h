// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/Detail/AllocatorTraitsDetail.h"

KOR_NAMESPACE_BEGIN

// [Is Allocator Family]
// * Checks whether type follows AllocatorFamily concept

template<typename FamilyT>
using TIsAllocatorFamily = Detail::Allocator::TIsFamily<FamilyT>;

// [Is Allocator]
// * Checks whether type follows Allocator concept

template<typename AllocatorT>
struct TIsAllocator : TBoolValue<Detail::Allocator::IsAllocator<AllocatorT>> {};

// [Make Untyped/Typed Allocator]
// * Makes typed or untyped allocator from allocator family

template<typename FamilyT>
using TMakeUntypedAllocator = typename Detail::Allocator::TValidateMake<typename Detail::Allocator::TUntypedOf<FamilyT>::Type, false>;

template<typename FamilyT, typename ElementT>
using TMakeTypedAllocator = typename Detail::Allocator::TValidateMake<typename Detail::Allocator::TTypedOf<FamilyT, ElementT>::Type, true>;

// [Allocator Traits]
// * Traits for Allocator (not suitable for AllocatorFamily)
// * Provides types and flags about Allocators (SizeType, ElementType, SupportsAlignment etc..)

template<typename T>
struct TAllocatorTraits
{
	static_assert(TIsAllocator<T>::Value, "T is not an allocator type");

	// Size type used by the allocator
	// * Infers type from first argument of "Allocate" method
	using SizeType = typename Detail::Allocator::TAllocateFunctionTrait<T>::template ArgType<0>;

	// Pointer type used by the allocator
	// * Infers type from return type of "Allocate" method
	using PointerType = typename Detail::Allocator::TAllocateFunctionTrait<T>::ReturnType;

	// Allocator type for ease of use
	using AllocatorType = T;

	// Allocator element type
	using ElementType = TRemovePointer<PointerType>::Type;

	enum
	{
		// Whether allocator allows "Alignment" parameter
		SupportsAlignment = Detail::Allocator::TAllocateFunctionTrait<T>::Arity >= 2,

		// Whether allocator exposes "Reallocate" method
		SupportsReallocate = Detail::Allocator::HasReallocate<T>,

		// Whether allocator is typed allocator
		IsTyped = !TIsVoid<ElementType>::Value,

		// Whether allocator is untyped/raw allocator
		IsUntyped = !IsTyped,
	};
};

KOR_NAMESPACE_END