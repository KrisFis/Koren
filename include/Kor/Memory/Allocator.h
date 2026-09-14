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
	using SizeType = int32;

	// Allocates a raw, untyped memory block.
	// @param bytes - Number of bytes to allocate.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the allocated block, or nullptr on failure.
	void* Allocate(SizeType bytes, uint32 alignment) noexcept;

	// Resizes a previously allocated block, possibly moving it.
	// @param ptr - Block previously returned by Allocate/Reallocate.
	// @param bytes - New size of the block, in bytes.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the (possibly relocated) block, or nullptr on failure.
	void* Reallocate(void* ptr, SizeType bytes, uint32 alignment) noexcept;

	// Frees a block previously returned by Allocate/Reallocate.
	// @param ptr - Block to free.
	// @param alignment - Alignment the block was originally allocated with.
	void Deallocate(void* ptr, uint32 alignment) noexcept;
};

template<>
struct TAllocatorTraits<CAllocator> : TAllocatorTraitsBase<CAllocator>
{
	using SizeType = typename CAllocator::SizeType;

	enum
	{
		NeedsAlignment = true,
		HasReallocate = true,
	};
};

#include "Kor/Memory/Detail/Allocator.inl"

KOR_NAMESPACE_END
