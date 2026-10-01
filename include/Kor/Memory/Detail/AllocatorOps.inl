// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename AllocatorT>
typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::Allocate(AllocatorType& allocator, SizeType capacity, uint32 alignment) noexcept
{
	if constexpr (Traits::SupportsAlignment) { return allocator.Allocate(capacity, alignment); }
	else { return allocator.Allocate(capacity); }
}

template<typename AllocatorT>
typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::Reallocate(AllocatorType& allocator, PointerType ptr, SizeType newCapacity, uint32 alignment) noexcept
{
	static_assert(Traits::SupportsReallocate,
	              "Allocator does not support Reallocate. "
	              "Prefer `ReallocateWithFallback(allocator, ptr, oldCapacity, newCapacity, alignment)`");

	if constexpr (Traits::SupportsAlignment) { return allocator.Reallocate(ptr, newCapacity, alignment); }
	else { return allocator.Reallocate(ptr, newCapacity); }
}

template<typename AllocatorT>
typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::ReallocateWithFallback(AllocatorType& allocator, PointerType ptr, SizeType oldCapacity, SizeType newCapacity, uint32 alignment) noexcept
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
typename TAllocatorOps<AllocatorT>::PointerType TAllocatorOps<AllocatorT>::ReallocateConstructed(AllocatorType& allocator, PointerType ptr, SizeType oldCapacity, SizeType oldConstructed, SizeType newCapacity, uint32 alignment) noexcept
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
void TAllocatorOps<AllocatorT>::Deallocate(AllocatorType& allocator, PointerType ptr, uint32 alignment) noexcept
{
	if constexpr (Traits::SupportsAlignment) allocator.Deallocate(ptr, alignment);
	else allocator.Deallocate(ptr);
}

template<typename AllocatorT>
void TAllocatorOps<AllocatorT>::DeallocateConstructed(AllocatorType& allocator, PointerType ptr, SizeType numConstructed, uint32 alignment) noexcept
{
	if (numConstructed > 0)
	{
		SMemoryOps::Destruct(ptr, numConstructed);
	}

	Deallocate(allocator, ptr, alignment);
}