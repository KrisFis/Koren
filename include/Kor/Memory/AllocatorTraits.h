// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"

#include "Kor/TypeTrait/MemberFunction.h"
#include "Kor/TypeTrait/Macros/HasFieldCheck.h"

KOR_NAMESPACE_BEGIN

// See Allocator.h for model concept

namespace Detail
{
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasAllocate, Allocate)
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasReallocate, Reallocate)
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasDeallocate, Deallocate)

	template<typename T, typename = void>
	struct TIsAllocatorImpl : TFalseValue {};

	template<typename T>
	struct TIsAllocatorImpl<T, typename TEnableIf<THasAllocate<T>::Value && THasDeallocate<T>::Value>::Type> : TTrueValue {};

	template<typename T>
	struct TAllocatorTraitsHelper
	{
		static_assert(
			THasAllocate<T>::Value,
			"T must define Allocate(size, [optional] alignment) method"
		);

		static_assert(
			THasDeallocate<T>::Value,
			"T must define Deallocate(ptr, size, [optional] alignment) method"
		);

	private:
		using AllocateTraits = TMemberFunctionTraits<decltype(&T::Allocate)>;

	public:
		using SizeType = typename AllocateTraits::template ArgType<0>;

		enum
		{
			NeedsAlignment = AllocateTraits::Arity >= 2,
			SupportsReallocate = THasReallocate<T>::Value,
		};
	};
}

// [Typed Allocator]
// Adapter of AllocatorT for ElementT
template<typename AllocatorT, typename ElementT>
class TTypedAllocator;

// [Is Allocator]
// * Checks whether specific type is an allocator (follows allocator concept)
template<typename T>
using TIsAllocator = Detail::TIsAllocatorImpl<T>;

// [Allocator Traits]
template<typename T>
struct TAllocatorTraits : private Detail::TAllocatorTraitsHelper<T> {};

// [Is Typed Allocator]
// * Checks whether specific type is a typed allocator type

template<typename T> struct TIsTypedAllocator : TFalseValue {};

template<typename AllocatorT, typename ElementT>
struct TIsTypedAllocator<TTypedAllocator<AllocatorT, ElementT>> : TTrueValue {};

KOR_NAMESPACE_END