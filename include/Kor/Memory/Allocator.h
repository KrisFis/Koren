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
	void* Allocate(int32 bytes, uint32 alignment) noexcept;

	// Resizes a previously allocated block, possibly moving it.
	// @param ptr - Block previously returned by Allocate/Reallocate.
	// @param bytes - New size of the block, in bytes.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the (possibly relocated) block, or nullptr on failure.
	void* Reallocate(void* ptr, int32 bytes, uint32 alignment) noexcept;

	// Frees a block previously returned by Allocate/Reallocate.
	// @param ptr - Block to free.
	// @param alignment - Alignment the block was originally allocated with.
	void Deallocate(void* ptr, uint32 alignment) noexcept;
};

template<typename ElementT>
class TTypedAllocator<CAllocator, ElementT> : protected CAllocator
{
public:
	// Asserts
	// -------------------------------------------------------------------------

	static_assert(
		!TIsVoid<ElementT>::Value && TIsClean<ElementT>::Value,
		"ElementType must be a non-void clean type");

	// Allocator Interface
	// -------------------------------------------------------------------------

	// Allocate
	// * Allocates storage for num elements.
	// @param num - Number of elements to allocate storage for.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the allocated elements, or nullptr on failure.
	ElementT* Allocate(
		int32 num,
		uint32 alignment
	) noexcept { return (ElementT*)CAllocator::Allocate(sizeof(ElementT) * num, alignment); }

	// Reallocate
	// @param ptr - Block previously returned by Allocate/Reallocate.
	// @param num - New number of elements the block should hold.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the (possibly relocated) elements, or nullptr on failure.
	ElementT* Reallocate(
		ElementT* ptr,
		int32 num,
		uint32 alignment
	) noexcept { return (ElementT*)CAllocator::Reallocate((void*)ptr, sizeof(ElementT) * num, alignment); }

	// Free
	// Frees a block previously returned by Allocate/Reallocate, using ElementT's natural alignment.
	// @param ptr - Block to free.
	// @param alignment - Alignment the block was originally allocated with.
	void Deallocate(
		ElementT* ptr,
		uint32 alignment
	) noexcept { return CAllocator::Deallocate((void*)ptr, alignment); }
};

#include "Kor/Memory/Detail/Allocator.inl"

KOR_NAMESPACE_END
