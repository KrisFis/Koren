// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

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