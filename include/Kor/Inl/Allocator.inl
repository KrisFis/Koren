// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE AllocatorT* TTypedAllocator<AllocatorT, ElementT>::operator->() noexcept
{
	return &_allocator;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE const AllocatorT* TTypedAllocator<AllocatorT, ElementT>::operator->() const noexcept
{
	return &_allocator;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE AllocatorT* TTypedAllocator<AllocatorT, ElementT>::operator*() noexcept
{
	return &_allocator;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE const AllocatorT* TTypedAllocator<AllocatorT, ElementT>::operator*() const noexcept
{
	return &_allocator;
}

KOR_FORCEINLINE void* CAllocator::Allocate(SizeType bytes, SizeType alignment) noexcept
{
	KOR_ASSERT_DEBUG(bytes > 0);
	return SMemoryOps::Malloc(bytes, alignment);
}

KOR_FORCEINLINE void* CAllocator::Reallocate(void* ptr, SizeType bytes, SizeType alignment) noexcept
{
	KOR_ASSERT_DEBUG(bytes > 0);
	return SMemoryOps::Realloc(ptr, bytes, alignment);
}

KOR_FORCEINLINE void CAllocator::Deallocate(void* ptr, SizeType alignment) noexcept
{
	return SMemoryOps::Free(ptr, alignment);
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE AllocatorT& TTypedAllocator<AllocatorT, ElementT>::Get() noexcept
{
	return _allocator;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE const AllocatorT& TTypedAllocator<AllocatorT, ElementT>::Get() const noexcept
{
	return _allocator;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocator<AllocatorT, ElementT>::Allocate(SizeType num) noexcept
{
	return (ElementType*)_allocator.Allocate(num * sizeof(ElementType), alignof(ElementType));
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocator<AllocatorT, ElementT>::Allocate(SizeType num, SizeType alignment) noexcept
{
	return (ElementType*)_allocator.Allocate(num * sizeof(ElementType), alignment);
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocator<AllocatorT, ElementT>::Reallocate(ElementType* ptr, SizeType num) noexcept
{
	return (ElementType*)_allocator.Reallocate(ptr, num * sizeof(ElementType), alignof(ElementType));
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocator<AllocatorT, ElementT>::Reallocate(ElementType* ptr, SizeType num, SizeType alignment) noexcept
{
	return (ElementType*)_allocator.Reallocate(ptr, num * sizeof(ElementType), alignment);
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE void TTypedAllocator<AllocatorT, ElementT>::Deallocate(ElementType* ptr) noexcept
{
	_allocator.Deallocate(ptr, alignof(ElementType));
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE void TTypedAllocator<AllocatorT, ElementT>::Deallocate(ElementType* ptr, SizeType alignment) noexcept
{
	_allocator.Deallocate(ptr, alignment);
}