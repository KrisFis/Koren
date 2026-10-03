// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename T>
KOR_FORCEINLINE T* CAllocator::Typed<T>::Allocate(SizeType num, uint32 alignment) noexcept
{
	return (T*)SMemoryOps::Malloc(num * SizeOf<T, 1>(), alignment);
}

template<typename T>
KOR_FORCEINLINE T* CAllocator::Typed<T>::Reallocate(T* ptr, SizeType num, uint32 alignment) noexcept
{
	return (T*)SMemoryOps::Realloc((void*)ptr, num * SizeOf<T, 1>(), alignment);
}

template<typename T>
KOR_FORCEINLINE void CAllocator::Typed<T>::Deallocate(T* ptr, uint32 alignment) noexcept
{
	SMemoryOps::Free((void*)ptr, alignment);
}