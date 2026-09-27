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
	KOR_DEFINE_MEMBER_FUNCTION_TRAIT(TAllocateFunctionTrait, Allocate)
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasReallocate, Reallocate)
	KOR_DEFINE_HAS_MEMBER_TRAIT(THasDeallocate, Deallocate)

	template<typename T>
	struct TFollowsAllocatorConcept
		: TBoolValue<TAllocateFunctionTrait<T>::Valid && THasDeallocate<T>::Value>
	{};

	template<typename T> struct TIsTypedAllocator : TFalseValue {};

	template<typename AllocatorT, typename ElementT>
	struct TIsTypedAllocator<TTypedAllocator<AllocatorT, ElementT>> : TTrueValue {};

	template<typename T>
	struct TIsUntypedAllocator : TBoolValue<!TIsTypedAllocator<T>::Value && TFollowsAllocatorConcept<T>::Value> {};

	template<typename T>
	struct TAnyAllocatorTraitsBase
	{
		static_assert(
			TAllocateFunctionTrait<T>::Valid,
			"T must define Allocate(size, alignment) method"
		);

		static_assert(
			THasDeallocate<T>::Value,
			"T must define Deallocate(ptr, size, alignment) method"
		);

		using Type = T;

		// Gets first argument type of "Allocate" method
		using SizeType = typename TAllocateFunctionTrait<T>::template ArgType<0>;

		// Checks whether allocator exposes "Reallocate" method
		static constexpr bool HasReallocate = THasReallocate<T>::Value;
	};
}

// [Is Allocator]

template<typename T>
using TIsAllocator = Detail::TIsUntypedAllocator<T>;

// [Is Typed Allocator]

template<typename T>
using TIsTypedAllocator = Detail::TIsTypedAllocator<T>;

// [Allocator Traits]

template<typename T>
struct TAllocatorTraits	: Detail::TAnyAllocatorTraitsBase<T>
{
private:
	using Super = Detail::TAnyAllocatorTraitsBase<T>;

public:
	using AllocatorType = T;

	template<typename ElementT>
	using TypedType = TTypedAllocator<AllocatorType, ElementT>;

	// Algo

	static void* Reallocate(
		Super::Type& allocator,
		void* ptr,
		typename Super::SizeType oldSize,
		typename Super::SizeType newSize,
		uint32 alignment = KOR_DEFAULT_HEAP_ALIGNMENT)
	{
		if constexpr (Super::HasReallocate)
		{
			return allocator.Reallocate(ptr, newSize, alignment);
		}
		else
		{
			void* newData = allocator.Allocate(newSize, alignment);
			KOR_ASSERT(newData);

			if (oldSize > 0)
			{
				SPlatformMemoryOps::Move(newData, ptr, oldSize);
				allocator.Deallocate(ptr);
			}

			return newData;
		}
	}
};

// [Typed Allocator Traits]

template<typename AllocatorT, typename ElementT>
struct TTypedAllocatorTraits : Detail::TAnyAllocatorTraitsBase<TTypedAllocator<AllocatorT, ElementT>>
{
private:
	using Super = Detail::TAnyAllocatorTraitsBase<TTypedAllocator<AllocatorT, ElementT>>;

public:
	using AllocatorType = AllocatorT;
	using ElementType = ElementT;

	template<typename OtherElementT>
	using CastedType = TTypedAllocator<AllocatorT, OtherElementT>;

	// Algo

	static ElementType* Reallocate(
		Super::Type& allocator,
		ElementType* ptr,
		typename Super::SizeType oldNum,
		typename Super::SizeType newNum,
		uint32 alignment = alignof(ElementType))
	{
		if constexpr (Super::HasReallocate)
		{
			return allocator.Reallocate(ptr, newNum, alignment);
		}
		else
		{
			ElementType* newData = allocator.Allocate(newNum, alignment);
			KOR_ASSERT(newData);

			if (oldNum > 0)
			{
				SMemoryOps::MoveConstruct(newData, ptr, oldNum);
				SMemoryOps::Destruct(ptr, oldNum);
				allocator.Deallocate(ptr, oldNum);
			}

			return newData;
		}
	}
};

KOR_NAMESPACE_END