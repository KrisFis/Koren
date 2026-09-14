// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"

#include "Kor/Memory/AllocatorTraits.h"
#include "Kor/TypeTrait/Category.h"
#include "Kor/TypeTrait/Qualifier.h"

KOR_NAMESPACE_BEGIN

// TTypedAllocator
// Adapter of AllocatorT for ElementT
// * Wraps an untyped allocator and exposes an element-typed interface (element counts -> bytes).
// * Inherits AllocatorT for EBO on empty allocators. AllocatorT must not be final.
// * Alignment handling depends on TAllocatorTraits<AllocatorT>::NeedsAlignment.
// * Custom specializations should preserve EBO so TIsEmpty checks stay meaningful.
template<typename AllocatorT, typename ElementT>
class TTypedAllocator : protected AllocatorT
{
public:
	// Asserts
	// -------------------------------------------------------------------------

	static_assert(
		!TIsVoid<ElementT>::Value && TIsClean<ElementT>::Value,
		"ElementType must be a non-void clean type");

	static_assert(TIsAllocator<AllocatorT>::Value,
	              "AllocatorType must be a valid allocator type");

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

	// Getters
	// -------------------------------------------------------------------------

	// Returns the underlying untyped allocator.
	AllocatorType& Get() noexcept;
	const AllocatorType& Get() const noexcept;

	// Allocator Interface
	// -------------------------------------------------------------------------

	// Allocate
	// * Allocates storage for num elements, using ElementType's natural alignment.
	// @param num - Number of elements to allocate storage for.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the allocated elements, or nullptr on failure.

	ElementType* Allocate(
		SizeType num = 1,
		uint32 alignment = alignof(ElementType)
	) noexcept;

	// Reallocate
	// @param ptr - Block previously returned by Allocate/Reallocate.
	// @param num - New number of elements the block should hold.
	// @param alignment - Required alignment of the returned block, in bytes.
	// @return Pointer to the (possibly relocated) elements, or nullptr on failure.

	ElementType* Reallocate(
		ElementType* ptr,
		SizeType num,
		uint32 alignment = alignof(ElementType)
	) noexcept;

	// Free
	// Frees a block previously returned by Allocate/Reallocate, using ElementType's natural alignment.
	// @param ptr - Block to free.
	// @param alignment - Alignment the block was originally allocated with.

	void Deallocate(
		ElementType* ptr,
		uint32 alignment = alignof(ElementType)
	) noexcept;
};

#include "Kor/Memory/Detail/TypedAllocator.inl"

KOR_NAMESPACE_END
