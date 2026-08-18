// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE AllocatorT* TTypedAllocator<AllocatorT, ElementT>::operator->() noexcept
{
	return this;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE const AllocatorT* TTypedAllocator<AllocatorT, ElementT>::operator->() const noexcept
{
	return this;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE AllocatorT* TTypedAllocator<AllocatorT, ElementT>::operator*() noexcept
{
	return this;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE const AllocatorT* TTypedAllocator<AllocatorT, ElementT>::operator*() const noexcept
{
	return this;
}

KOR_FORCEINLINE void* CAllocator::Allocate(SizeType bytes, uint32 alignment) noexcept
{
	KOR_ASSERT_DEBUG(bytes > 0);
	return SMemoryOps::Malloc(bytes, alignment);
}

KOR_FORCEINLINE void* CAllocator::Reallocate(void* ptr, SizeType bytes, uint32 alignment) noexcept
{
	KOR_ASSERT_DEBUG(bytes > 0);
	return SMemoryOps::Realloc(ptr, bytes, alignment);
}

KOR_FORCEINLINE void CAllocator::Deallocate(void* ptr, uint32 alignment) noexcept
{
	return SMemoryOps::Free(ptr, alignment);
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE AllocatorT& TTypedAllocator<AllocatorT, ElementT>::Get() noexcept
{
	return *this;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE const AllocatorT& TTypedAllocator<AllocatorT, ElementT>::Get() const noexcept
{
	return *this;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocator<AllocatorT, ElementT>::Allocate(SizeType num) noexcept
{
	return Allocate(num, alignof(ElementType));
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocator<AllocatorT, ElementT>::Allocate(SizeType num, uint32 alignment) noexcept
{
	using Traits = TAllocatorTraits<AllocatorT>;

	if constexpr (Traits::NeedsAlignment)
	{
		return (ElementType*)AllocatorType::Allocate(num * sizeof(ElementType), alignment);
	}
	else
	{
		return (ElementType*)AllocatorType::Allocate(num * sizeof(ElementType));
	}
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocator<AllocatorT, ElementT>::Reallocate(ElementType* ptr, SizeType num) noexcept
{
	return Reallocate(ptr, num, alignof(ElementType));
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocator<AllocatorT, ElementT>::Reallocate(ElementType* ptr, SizeType num, uint32 alignment) noexcept
{
	using Traits = TAllocatorTraits<AllocatorT>;

	static_assert(
		Traits::HasReallocate, 
		"AllocatorType has no native Reallocate. Caller must Allocate + Copy + Deallocate manually, since only the container knows the old element count");

	if constexpr (Traits::NeedsAlignment)
	{
		return (ElementType*)AllocatorType::Reallocate(ptr, num * sizeof(ElementType), alignment);
	}
	else
	{
		return (ElementType*)AllocatorType::Reallocate(ptr, num * sizeof(ElementType));
	}
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE void TTypedAllocator<AllocatorT, ElementT>::Deallocate(ElementType* ptr) noexcept
{
	Deallocate(ptr, alignof(ElementType));
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE void TTypedAllocator<AllocatorT, ElementT>::Deallocate(ElementType* ptr, uint32 alignment) noexcept
{
	using Traits = TAllocatorTraits<AllocatorT>;

	if constexpr (Traits::NeedsAlignment)
	{
		AllocatorType::Deallocate(ptr, alignment);
	}
	else
	{
		AllocatorType::Deallocate(ptr);
	}
}