// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/MemoryOps.h"

KOR_NAMESPACE_BEGIN

// Reference model for the untyped allocator interface.
// Any type satisfying TIsAllocator must expose Allocate/Reallocate/Deallocate with this signature
class CAllocator
{
public:
	// Types
	// -------------------------------------------------------------------------
	using SizeType = int64;

	// Model interface
	// -------------------------------------------------------------------------

	// Allocates a raw, untyped memory block.
	// @param bytes - Number of bytes to allocate.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the allocated block, or nullptr on failure.
	void* Allocate(SizeType bytes, SizeType alignment) noexcept;

	// Resizes a previously allocated block, possibly moving it.
	// @param ptr - Block previously returned by Allocate/Reallocate.
	// @param bytes - New size of the block, in bytes.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the (possibly relocated) block, or nullptr on failure.
	void* Reallocate(void* ptr, SizeType bytes, SizeType alignment) noexcept;

	// Frees a block previously returned by Allocate/Reallocate.
	// @param ptr - Block to free.
	// @param alignment - Alignment the block was originally allocated with.
	void Deallocate(void* ptr, SizeType alignment) noexcept;
};

template<>
struct TAllocatorTraits<CAllocator> : TAllocatorTraitsBase<CAllocator>
{
	using SizeType = typename CAllocator::SizeType;
};

// Adapter of AllocatorT for ElementT
// * Wraps an untyped (byte-oriented) allocator and exposes an element-typed interface
// ** converting element counts to byte sizes and casting the returned pointers to ElementType*.
// NOTE: Is not specifying traits and is technically not TIsAllocator, as its only adapter
template<typename AllocatorT, typename ElementT>
class TTypedAllocator
{
	static_assert(TIsAllocator<AllocatorT>::Value, "AllocatorT must be allocator type");
	static_assert(TIsPure<ElementT>::Value, "ElementT must be pure type");
public:
	// Types
	// -------------------------------------------------------------------------
	using AllocatorType = AllocatorT;
	using ElementType = ElementT;
	using SizeType = typename TAllocatorTraits<AllocatorT>::SizeType;

	// Operators
	// -------------------------------------------------------------------------

	// Provides direct member access to the underlying untyped allocator.
	AllocatorType* operator->() noexcept;
	const AllocatorType* operator->() const noexcept;

	// Dereferences to the underlying untyped allocator.
	AllocatorType* operator*() noexcept;
	const AllocatorType* operator*() const noexcept;

	// Properties
	// -------------------------------------------------------------------------

	// Returns the underlying untyped allocator.
	AllocatorType& Get() noexcept;
	const AllocatorType& Get() const noexcept;

	// Allocate
	// * Allocates storage for num elements, using ElementType's natural alignment.
	// @param num - Number of elements to allocate storage for.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the allocated elements, or nullptr on failure.
	// -------------------------------------------------------------------------

	ElementType* Allocate(SizeType num = 1) noexcept;
	ElementType* Allocate(SizeType num, SizeType alignment) noexcept;

	// Reallocate
	// @param ptr - Block previously returned by Allocate/Reallocate.
	// @param num - New number of elements the block should hold.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the (possibly relocated) elements, or nullptr on failure.
	// -------------------------------------------------------------------------

	ElementType* Reallocate(ElementType* ptr, SizeType num) noexcept;
	ElementType* Reallocate(ElementType* ptr, SizeType num, SizeType alignment) noexcept;

	// Free
	// Frees a block previously returned by Allocate/Reallocate, using ElementType's natural alignment.
	// @param ptr - Block to free.
	// @param alignment - Alignment the block was originally allocated with.
	// -------------------------------------------------------------------------

	void Deallocate(ElementType* ptr) noexcept;
	void Deallocate(ElementType* ptr, SizeType alignment) noexcept;

private:
	AllocatorType _allocator;
};

#include "Kor/Inl/Allocator.inl"

KOR_NAMESPACE_END