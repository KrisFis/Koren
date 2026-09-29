// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/MemoryOps.h"

#include "Kor/Math/MathOps.h"

#include "Kor/TypeTrait/Construct.h"

KOR_NAMESPACE_BEGIN

template<typename AllocatorT>
struct SAllocatorOps
{
	static_assert(TAllocatorAssert<AllocatorT>::Passed);

	using Traits = TAllocatorTraits<AllocatorT>;

	using AllocatorType = AllocatorT;
	using SizeType = typename Traits::SizeType;
	using ElementType = typename Traits::ElementType;
	using PointerType = typename Traits::PointerType;

	static constexpr uint32 DefaultAlignment = Traits::IsTyped
		? AlignOf<ElementType>()
		: KOR_DEFAULT_HEAP_ALIGNMENT;

	KOR_FORCEINLINE static PointerType Allocate(
		AllocatorType& allocator,
		SizeType capacity,
		uint32 alignment = DefaultAlignment) noexcept
	{
		if constexpr (Traits::SupportsAlignment) { return allocator.Allocate(capacity, alignment); }
		else { return allocator.Allocate(capacity); }
	}

	KOR_INLINE static PointerType Reallocate(
		AllocatorType& allocator,
		PointerType ptr,
		SizeType newCapacity,
		uint32 alignment = DefaultAlignment) noexcept
	{
		static_assert(Traits::SupportsReallocate,
			"Allocator does not support Reallocate. "
			"Prefer `ReallocateWithFallback(allocator, ptr, oldCapacity, newCapacity, alignment)`");

		if constexpr (Traits::SupportsAlignment) { return allocator.Reallocate(ptr, newCapacity, alignment); }
		else { return allocator.Reallocate(ptr, newCapacity); }
	}

	KOR_INLINE static PointerType ReallocateWithFallback(
		AllocatorType& allocator,
		PointerType ptr,
		SizeType oldCapacity,
		SizeType newCapacity,
		uint32 alignment = DefaultAlignment) noexcept
	{
		if constexpr (Traits::SupportsReallocate)
		{
			if constexpr (Traits::SupportsAlignment) { return allocator.Reallocate(ptr, newCapacity, alignment); }
			else { return allocator.Reallocate(ptr, newCapacity); }
		}
		else if (oldCapacity > 0)
		{
			PointerType newData = Allocate(allocator, newCapacity, alignment);
			KOR_ASSERT(newData);

			SPlatformMemoryOps::Move(newData, ptr, SMathOps::Min(oldCapacity, newCapacity) * SizeOf<ElementType, 1>());
			Deallocate(allocator, ptr, alignment);

			return newData;
		}
		else
		{
			if (ptr) Deallocate(allocator, ptr, alignment);
			return Allocate(allocator, newCapacity, alignment);
		}
	}

	KOR_INLINE static PointerType ReallocateConstructed(
		AllocatorType& allocator,
		PointerType ptr,
		SizeType oldCapacity,
		SizeType oldConstructed,
		SizeType newCapacity,
		uint32 alignment = DefaultAlignment) noexcept
	{
		if constexpr (!Traits::IsTyped)
		{
			// We know that "oldConstructed" will be used for moving bytes and we don't want to move "oldCapacity"
			return ReallocateWithFallback(allocator, ptr, oldConstructed, newCapacity, alignment);
		}
		else // Traits::IsTyped
		{
			if constexpr (TIsTriviallyRelocatable<ElementType>::Value)
			{
				// We know that "oldConstructed" will be used for moving bytes and we don't want to move "oldCapacity"
				return ReallocateWithFallback(allocator, ptr, oldConstructed, newCapacity, alignment);
			}
			else if (oldConstructed > 0)
			{
				PointerType newData = Allocate(allocator, newCapacity, alignment);
				KOR_ASSERT(newData);

				SMemoryOps::MoveConstruct(newData, ptr, oldConstructed);
				DeallocateConstructed(allocator, ptr, oldConstructed, alignment);

				return newData;
			}
			else
			{
				if (oldCapacity > 0) Deallocate(allocator, ptr, alignment);
				return Allocate(allocator, newCapacity, alignment);
			}
		}
	}

	KOR_FORCEINLINE static void Deallocate(
		AllocatorType& allocator,
		PointerType ptr,
		uint32 alignment = DefaultAlignment) noexcept
	{
		if constexpr (Traits::SupportsAlignment) allocator.Deallocate(ptr, alignment);
		else allocator.Deallocate(ptr);
	}

	KOR_INLINE static void DeallocateConstructed(
		AllocatorType& allocator,
		PointerType ptr,
		SizeType numConstructed,
		uint32 alignment = DefaultAlignment) noexcept
	{
		if (numConstructed > 0)
		{
			SMemoryOps::Destruct(ptr, numConstructed);
		}

		Deallocate(allocator, ptr, alignment);
	}
};

KOR_NAMESPACE_END