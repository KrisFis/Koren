// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/AllocatorTraits.h"
#include "Kor/Memory/MemoryOps.h"

KOR_NAMESPACE_BEGIN

// Reference model for the untyped allocator interface.
// Any type satisfying TIsAllocator must expose Allocate/Reallocate/Deallocate with this signature
class CAllocator
{
public:
	// Allocates a raw, untyped memory block.
	// @param bytes - Number of bytes to allocate.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the allocated block, or nullptr on failure.
	void* Allocate(int32 bytes, uint32 alignment = KOR_DEFAULT_HEAP_ALIGNMENT) noexcept;

	// Resizes a previously allocated block, possibly moving it.
	// @param ptr - Block previously returned by Allocate/Reallocate.
	// @param bytes - New size of the block, in bytes.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the (possibly relocated) block, or nullptr on failure.
	void* Reallocate(void* ptr, int32 bytes, uint32 alignment = KOR_DEFAULT_HEAP_ALIGNMENT) noexcept;

	// Frees a block previously returned by Allocate/Reallocate.
	// @param ptr - Block to free.
	// @param alignment - Alignment the block was originally allocated with.
	void Deallocate(void* ptr, uint32 alignment = KOR_DEFAULT_HEAP_ALIGNMENT) noexcept;
};

#include "Kor/Memory/Detail/Allocator.inl"

KOR_NAMESPACE_END
