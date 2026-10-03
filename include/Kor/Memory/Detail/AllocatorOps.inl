// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Detail
{
	template<typename ElementT, typename SizeT>
	struct TDefaultAllocationPolicy
	{
		static constexpr SizeT INIT = 4;
		static constexpr SizeT PADDING = KOR_DEFAULT_HEAP_ALIGNMENT; // in elements
		static constexpr SizeT FACTOR_NUM = 3;
		static constexpr SizeT FACTOR_DEN = 8;

		KOR_INLINE static SizeT CalculateGrow(
			SizeT oldNum,
			SizeT num,
			uint32 /*alignment*/) noexcept
		{
			// First small allocation: start at INIT
			if (oldNum == 0 && num <= INIT)
			{
				return INIT;
			}

			// 64-bit intermediate so num * FACTOR_NUM can't overflow a 32-bit SizeT
			const uint64 n = (uint64)num;
			const uint64 result = n + (n * FACTOR_NUM) / FACTOR_DEN + PADDING;

			return (SizeT)SMathOps::Clamp<uint64>(result, 0, TLimits<SizeT>::Max);
		}

		KOR_INLINE static SizeT CalculateShrink(
			SizeT oldNum,
			SizeT num,
			uint32 /*alignment*/) noexcept
		{
			// num <= oldNum, so slack is oldNum - num
			const uint64 slack = (uint64)oldNum - (uint64)num;
			const uint64 slackBytes = slack * (uint64)sizeof(ElementT);

			// Keep the block unless enough memory would be reclaimed
			return (slackBytes >= KOR_BUFFER_SIZE_LARGE) ? num : oldNum;
		}
	};
}

template<typename AllocatorT>
KOR_FORCEINLINE typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::Allocate(
	AllocatorType& allocator,
	SizeType capacity,
	uint32 alignment) noexcept
{
	if constexpr (Traits::SupportsAlignment) { return allocator.Allocate(capacity, alignment); }
	else { return allocator.Allocate(capacity); }
}

template<typename AllocatorT>
KOR_FORCEINLINE typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::Reallocate(
	AllocatorType& allocator,
	PointerType ptr,
	SizeType newCapacity,
	uint32 alignment) noexcept
{
	static_assert(Traits::SupportsReallocate,
	              "Allocator does not support Reallocate. "
	              "Prefer `ReallocateWithFallback(allocator, ptr, oldCapacity, newCapacity, alignment)`");

	if constexpr (Traits::SupportsAlignment) { return allocator.Reallocate(ptr, newCapacity, alignment); }
	else { return allocator.Reallocate(ptr, newCapacity); }
}

template<typename AllocatorT>
typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::ReallocateWithFallback(
	AllocatorType& allocator,
	PointerType ptr,
	SizeType oldCapacity,
	SizeType newCapacity,
	uint32 alignment) noexcept
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

template<typename AllocatorT>
typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::ReallocateConstructed(
	AllocatorType& allocator,
	PointerType ptr,
	SizeType oldCapacity,
	SizeType oldConstructed,
	SizeType newCapacity,
	uint32 alignment) noexcept
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
			if (newCapacity < oldConstructed)
			{
				SMemoryOps::Destruct(ptr + newCapacity, oldConstructed - newCapacity);
				oldConstructed = newCapacity;
			}

			// We know that "oldConstructed" will be used for moving bytes and we don't want to move "oldCapacity"
			return ReallocateWithFallback(allocator, ptr, oldConstructed, newCapacity, alignment);
		}
		else if (oldConstructed > 0)
		{
			PointerType newData = Allocate(allocator, newCapacity, alignment);
			KOR_ASSERT(newData);

			SMemoryOps::MoveConstruct(newData, ptr, SMathOps::Min(oldConstructed, newCapacity));
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

template<typename AllocatorT>
KOR_FORCEINLINE void TAllocatorOps<AllocatorT>::Deallocate(
	AllocatorType& allocator,
	PointerType ptr,
	uint32 alignment) noexcept
{
	if constexpr (Traits::SupportsAlignment) allocator.Deallocate(ptr, alignment);
	else allocator.Deallocate(ptr);
}

template<typename AllocatorT>
void TAllocatorOps<AllocatorT>::DeallocateConstructed(
	AllocatorType& allocator,
	PointerType ptr,
	SizeType numConstructed,
	uint32 alignment) noexcept
{
	if (numConstructed > 0)
	{
		SMemoryOps::Destruct(ptr, numConstructed);
	}

	Deallocate(allocator, ptr, alignment);
}

template<typename AllocatorT>
typename TAllocatorOps<AllocatorT>::SizeType TAllocatorOps<AllocatorT>::CalculateGrow(
	AllocatorType& allocator,
	SizeType oldCapacity,
	SizeType newCapacity,
	uint32 alignment) noexcept
{
	KOR_ASSERT(newCapacity >= oldCapacity);

	if (newCapacity == oldCapacity)
	{
		return oldCapacity;
	}

	// TODO: Allow allocators to provide their implementation
	const SizeType result = Detail::TDefaultAllocationPolicy<ElementType, SizeType>::CalculateGrow(
		oldCapacity,
		newCapacity,
		alignment
	);

	// never less than requested
	return SMathOps::Max(result, newCapacity);
}

template<typename AllocatorT>
typename TAllocatorOps<AllocatorT>::SizeType TAllocatorOps<AllocatorT>::CalculateShrink(
	AllocatorType& allocator,
	SizeType oldCapacity,
	SizeType newCapacity,
	uint32 alignment) noexcept
{
	KOR_ASSERT(newCapacity <= oldCapacity);

	if (newCapacity == oldCapacity)
	{
		return oldCapacity;
	}

	// TODO: Allow allocators to provide their implementation
	const SizeType result = Detail::TDefaultAllocationPolicy<ElementType, SizeType>::CalculateShrink(
		oldCapacity,
		newCapacity,
		alignment
	);

	// never below requested, never above current
	return SMathOps::Clamp(result, newCapacity, oldCapacity);
}