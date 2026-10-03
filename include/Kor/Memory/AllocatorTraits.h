// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/Detail/AllocatorTraitsDetail.h"
#include "Kor/Utility/SizeAlignOf.h"

KOR_NAMESPACE_BEGIN

// [Is Allocator Family]
// * Checks whether type follows AllocatorFamily concept

template<typename FamilyT>
using TIsAllocatorFamily = Detail::Allocator::TIsFamily<FamilyT>;

// [Is Allocator]
// * Checks whether type follows Allocator concept

template<typename AllocatorT>
using TIsAllocator = Detail::Allocator::TIsAllocator<AllocatorT>;

// [Allocator Family Traits]
// * Traits for allocator family (not suitable for allocator)

template<typename FamilyT>
struct TAllocatorFamilyTraits
{
	static_assert(TIsAllocatorFamily<FamilyT>::Value, "T is not an allocator family type");

	// Size type used by the allocator family
	// * Gets type from FamilyT::SizeType or asserts
	using SizeType = typename Detail::Allocator::TSizeTypeOfValidated<FamilyT>::Type;

	// Get untyped allocator type lazily
	// * Asserts on resolve if allocator is not supported
	struct UntypedAllocator : TType<typename Detail::Allocator::TUntypedOfValidated<FamilyT>::Type> {};

	// Gets typed allocator type lazily
	// * Asserts on resolve if allocator is not supported
	template<typename ElementT>
	using TypedAllocator = typename Detail::Allocator::TTypedOfValidated<FamilyT, ElementT>;
};

// [Allocator Traits]
// * Traits for Allocator (not suitable for AllocatorFamily)

template<typename T>
struct TAllocatorTraits
{
	static_assert(TIsAllocator<T>::Value, "T is not an allocator type");

	// Size type used by the allocator
	// * Infers type from first argument of "Allocate" method
	using SizeType = typename Detail::Allocator::TAllocateFunctionTraits<T>::template ArgType<0>;

	// Pointer type used by the allocator
	// * Infers type from return type of "Allocate" method
	using PointerType = typename Detail::Allocator::TAllocateFunctionTraits<T>::ReturnType;

	// Allocator type for ease of use
	using AllocatorType = T;

	// Allocator element type
	using ElementType = TRemovePointer<PointerType>::Type;

	// Default element alignment for allocator
	// * alignof(ElementType) for typed allocators, KOR_DEFAULT_HEAP_ALIGNMENT otherwise.
	static constexpr uint32 ElementAlignment = AlignOf<ElementType, KOR_DEFAULT_HEAP_ALIGNMENT>();

	// Default element size for allocator
	// * sizeof(ElementType) for typed allocators, 1 (bytes) otherwise
	static constexpr uint32 ElementSize = SizeOf<ElementType, 1>();

	enum
	{
		// Whether allocator allows "Alignment" parameter
		NeedsAlignment = Detail::Allocator::TAllocateFunctionTraits<T>::Arity >= 2,

		// Whether allocator exposes "Reallocate" method
		SupportsReallocate = Detail::Allocator::HasReallocate<T>,

		// Whether allocator exposed "CalculateGrow" method
		SupportsGrowPolicy = Detail::Allocator::HasCalculateGrow<T>,

		// Whether allocator exposed "CalculateShrink" method
		SupportsShrinkPolicy = Detail::Allocator::HasCalculateShrink<T>,

		// Whether allocator is typed allocator
		IsTyped = !TIsVoid<ElementType>::Value,

		// Whether allocator is untyped/raw allocator
		IsUntyped = !IsTyped,
	};
};

KOR_NAMESPACE_END