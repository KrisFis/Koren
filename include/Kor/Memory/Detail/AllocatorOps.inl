// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename AllocatorT>
KOR_FORCEINLINE typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::Allocate(
	AllocatorType& allocator,
	SizeType capacity,
	uint32 alignment) noexcept
{
	if constexpr (Traits::NeedsAlignment) { return allocator.Allocate(capacity, alignment); }
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
	              "Prefer `ReallocateWithFallback(allocator, ptr, newCapacity, oldCapacity, alignment)`");

	if constexpr (Traits::NeedsAlignment) { return allocator.Reallocate(ptr, newCapacity, alignment); }
	else { return allocator.Reallocate(ptr, newCapacity); }
}

template<typename AllocatorT>
typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::ReallocateWithFallback(
	AllocatorType& allocator,
	PointerType ptr,
	SizeType newCapacity,
	SizeType oldCapacity,
	uint32 alignment) noexcept
{
	if constexpr (Traits::SupportsReallocate)
	{
		if constexpr (Traits::NeedsAlignment) { return allocator.Reallocate(ptr, newCapacity, alignment); }
		else { return allocator.Reallocate(ptr, newCapacity); }
	}
	else if (oldCapacity > 0)
	{
		PointerType newData = Allocate(allocator, newCapacity, alignment);
		KOR_ASSERT(newData);

		SPlatformMemoryOps::Move(newData, ptr, SMathOps::Min(oldCapacity, newCapacity) * ElementSize);
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
	SizeType newCapacity,
	SizeType oldCapacity,
	SizeType oldConstructed,
	uint32 alignment) noexcept
{
	if constexpr (!Traits::IsTyped)
	{
		// We know that "oldConstructed" will be used for moving bytes and we don't want to move "oldCapacity"
		return ReallocateWithFallback(allocator, ptr, newCapacity, oldCapacity, alignment);
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
			return ReallocateWithFallback(allocator, ptr, newCapacity, oldCapacity, alignment);
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
	if constexpr (Traits::NeedsAlignment) allocator.Deallocate(ptr, alignment);
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
	SizeType newCapacity,
	SizeType oldCapacity,
	uint32 alignment) noexcept
{
	KOR_ASSERT(newCapacity >= oldCapacity);

	if (newCapacity == oldCapacity)
	{
		return oldCapacity;
	}

	SizeType result;
	if constexpr (Traits::SupportsGrowPolicy)
	{
		if constexpr (Traits::NeedsAlignment) result = allocator.CalculateGrow(newCapacity, oldCapacity, alignment);
		else result = allocator.CalculateGrow(newCapacity, oldCapacity);
	}
	else
	{
		result = SDefaultAllocationPolicy::CalculateGrow(newCapacity, oldCapacity, ElementSize, ElementAlignment);
	}

	// never less than requested
	return SMathOps::Max(result, newCapacity);
}

template<typename AllocatorT>
typename TAllocatorOps<AllocatorT>::SizeType TAllocatorOps<AllocatorT>::CalculateShrink(
	AllocatorType& allocator,
	SizeType newCapacity,
	SizeType oldCapacity,
	uint32 alignment) noexcept
{
	KOR_ASSERT(newCapacity <= oldCapacity);

	if (newCapacity == oldCapacity)
	{
		return oldCapacity;
	}

	SizeType result;
	if constexpr (Traits::SupportsShrinkPolicy)
	{
		if constexpr (Traits::NeedsAlignment) result = allocator.CalculateShrink(newCapacity, oldCapacity, alignment);
		else result = allocator.CalculateShrink(newCapacity, oldCapacity);
	}
	else
	{
		result = SDefaultAllocationPolicy::CalculateShrink(newCapacity, oldCapacity, ElementSize, ElementAlignment);
	}

	// never below requested, never above current
	return SMathOps::Clamp(result, newCapacity, oldCapacity);
}