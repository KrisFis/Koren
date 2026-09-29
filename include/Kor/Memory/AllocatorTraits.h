// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/TypeTrait/Decay.h"
#include "Kor/TypeTrait/MemberFunction.h"
#include "Kor/TypeTrait/Macros/HasFieldCheck.h"

KOR_NAMESPACE_BEGIN

// See Allocator.h for model concept

namespace Detail
{
	KOR_DEFINE_HAS_TYPE_TRAIT(THasUntypedType, Untyped);
	KOR_DEFINE_HAS_TYPE_TRAIT(THasTypedType, Typed);

	KOR_DEFINE_HAS_MEMBER_TRAIT(THasAllocateMember, Allocate)
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasReallocateMember, Reallocate)
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasDeallocateMember, Deallocate)

	KOR_DEFINE_MEMBER_FUNCTION_TRAIT(TAllocateFunctionTrait, Allocate)

	template<typename AllocatorFamilyT, bool Assert>
	struct TAllocatorFamilyConcept
	{
	private:
		static constexpr bool HasTypedType = THasTypedType<AllocatorFamilyT>::Value;
		static constexpr bool HasUntypedType = THasUntypedType<AllocatorFamilyT>::Value;

		static_assert(!Assert || HasUntypedType,
			"Allocator Family must define `Untyped` allocator type"
		);

		static_assert(!Assert || HasTypedType,
			"Allocator Family must define `Typed<T>` allocator type"
		);

	public:
		static constexpr bool Passed = HasTypedType && HasUntypedType;
	};

	template<typename AllocatorT, bool Assert>
	struct TAllocatorConcept
	{
	private:
		static constexpr bool HasAllocate = THasAllocateMember<AllocatorT>::Value;
		static constexpr bool HasDeallocate = THasDeallocateMember<AllocatorT>::Value;

		static_assert(!Assert || HasAllocate,
			"Allocator must define `Allocate(size)` or `Allocate(size, alignment)` method"
		);

		static_assert(!Assert || HasDeallocate,
			"Allocator must define `Deallocate(ptr, size)` or `Deallocate(ptr, size, alignment)` method"
		);

	public:
		static constexpr bool Passed = HasAllocate && HasDeallocate;
	};
}

// [Allocator Family Assert]

template<typename T>
using TAllocatorFamilyAssert = Detail::TAllocatorFamilyConcept<T, true>;

// [Is Allocator Family]

template<typename T>
struct TIsAllocatorFamily : TBoolValue<Detail::TAllocatorFamilyConcept<T, false>::Passed> {};

// [Allocator Assert]

template<typename T>
using TAllocatorAssert = Detail::TAllocatorConcept<T, true>;

// [Is Allocator]

template<typename T>
struct TIsAllocator : TBoolValue<Detail::TAllocatorConcept<T, false>::Passed> {};

// [Allocator Traits]

template<typename T>
struct TAllocatorTraits
{
	static_assert(TAllocatorAssert<T>::Passed);

	// Size type used by the allocator
	// * Infers type from first argument of "Allocate" method
	using SizeType = typename Detail::TAllocateFunctionTrait<T>::template ArgType<0>;

	// Pointer type used by the allocator
	// * Infers type from return type of "Allocate" method
	using PointerType = typename Detail::TAllocateFunctionTrait<T>::ReturnType;

	// Allocator type for ease of use
	using AllocatorType = T;

	// Allocator element type
	using ElementType = TRemovePointer<PointerType>::Type;

	enum
	{
		// Whether allocator allows "Alignment" parameter
		SupportsAlignment = Detail::TAllocateFunctionTrait<T>::Arity >= 2,

		// Whether allocator exposes "Reallocate" method
		SupportsReallocate = Detail::THasReallocateMember<T>::Value,

		// Whether allocator is typed allocator
		IsTyped = !TIsSame<ElementType, void>::Value,

		// Whether allocator is untyped/raw allocator
		IsUntyped = !IsTyped,
	};
};

KOR_NAMESPACE_END