// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/TypeTrait/MemberFunction.h"
#include "Kor/TypeTrait/Macros/HasFieldCheck.h"

KOR_NAMESPACE_BEGIN

// See Allocator.h for model concept

namespace Detail
{
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasAllocate, Allocate)
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasReallocate, Reallocate)
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasDeallocate, Deallocate)

	KOR_DEFINE_MEMBER_FUNCTION_TRAIT(TAllocateFunctionTrait, Allocate)

	template<typename AllocatorT, bool Assert>
	struct TAllocatorConcept
	{
	private:
		static constexpr bool HasAllocate = THasAllocate<AllocatorT>::Value;
		static constexpr bool HasDeallocate = THasDeallocate<AllocatorT>::Value;

		static_assert(Assert || HasAllocate,
			"T must define `Allocate(size)` or `Allocate(size, alignment)` method"
		);

		static_assert(Assert || HasDeallocate,
			"T must define `Deallocate(ptr, size)` or `Deallocate(ptr, size, alignment)` method"
		);

	public:
		static constexpr bool Passed = HasAllocate && HasDeallocate;
	};

	template<typename T>
	struct TAllocatorTraitsBase
	{
		static_assert(TAllocatorConcept<T, true>::Passed);

		// Gets first argument type of "Allocate" method
		using SizeType = typename TAllocateFunctionTrait<T>::template ArgType<0>;

		// Checks whether allocator allows "Alignment" parameter
		static constexpr bool SupportsAlignment = TAllocateFunctionTrait<T>::Arity >= 2;

		// Checks whether allocator exposes "Reallocate" method
		static constexpr bool SupportsReallocate = THasReallocate<T>::Value;
	};
}

// [Allocator Concept]

template<typename T>
using TAllocatorAssert = Detail::TAllocatorConcept<T, true>;

// [Is Allocator]

template<typename T>
struct TIsAllocator : TBoolValue<Detail::TAllocatorConcept<T, false>::Passed> {};

// [Allocator Traits]

template<typename T>
struct TAllocatorTraits	: Detail::TAllocatorTraitsBase<T>
{
private:
	using Super = Detail::TAllocatorTraitsBase<T>;

public:
	using AllocatorType = T;

	template<typename ElementT>
	using TypedType = TTypedAllocator<AllocatorType, ElementT>;
};

// [Typed Allocator Traits]

template<template<typename AllocatorT, typename ElementT> Base>
struct TAllocatorTraits : Detail::TAllocatorTraitsBase<Base<AllocatorT, ElementT>>
{
private:
	using Super = Detail::TAllocatorTraitsBase<TTypedAllocator<AllocatorT, ElementT>>;

public:
	using AllocatorType = AllocatorT;
	using ElementType = ElementT;

	template<typename OtherElementT>
	using CastedType = TTypedAllocator<AllocatorT, OtherElementT>;
};

KOR_NAMESPACE_END