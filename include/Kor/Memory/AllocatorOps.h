// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/Allocator.h"
#include "Kor/Memory/AllocatorTraits.h"
#include "Kor/Memory/MemoryOps.h"

#include "Kor/Math/MathOps.h"

#include "Kor/TypeTrait/Construct.h"

KOR_NAMESPACE_BEGIN

// TAllocatorOps
// -------------------------------------------------------------------------
// Uniform front-end over allocators
// * Forwards alignment only when the allocator supports it
// * Adds fallback, growth policy and element-lifetime handling on top.
//
// All sizes and capacities are in elements, not bytes.
template<typename AllocatorT>
struct TAllocatorOps
{
	using Traits = TAllocatorTraits<AllocatorT>;

	using AllocatorType = AllocatorT;
	using SizeType = typename Traits::SizeType;
	using ElementType = typename Traits::ElementType;
	using PointerType = typename Traits::PointerType;

	static constexpr uint32 ElementAlignment = Traits::ElementAlignment;
	static constexpr uint32 ElementSize = Traits::ElementSize;

	// Allocate | Reallocate
	// -------------------------------------------------------------------------

	// Allocates uninitialized storage for capacity elements
	// Failure behavior is defined by the allocator
	static PointerType Allocate(
		AllocatorType& allocator,
		SizeType capacity,
		uint32 alignment = ElementAlignment) noexcept;

	// Resizes a block using the allocator's native Reallocate
	// Static-asserts if the allocator has no Reallocate — use ReallocateWithFallback instead
	// May move the block bitwise, no constructors or destructors run
	static PointerType Reallocate(
		AllocatorType& allocator,
		PointerType ptr,
		SizeType newCapacity,
		uint32 alignment = ElementAlignment) noexcept;

	// Uses native Reallocate if available, otherwise Allocate + Move + Deallocate
	// The fallback moves min(oldCapacity, newCapacity) elements bitwise (memmove)
	// Asserts if the fallback allocation fails
	// oldCapacity == 0 -> frees ptr (if any) and allocates a fresh block
	static PointerType ReallocateWithFallback(
		AllocatorType& allocator,
		PointerType ptr,
		SizeType newCapacity,
		SizeType oldCapacity,
		uint32 alignment = ElementAlignment) noexcept;

	// Lifetime-aware reallocation: only [0, oldConstructed) holds live elements
	// Untyped or trivially relocatable -> ReallocateWithFallback, moving oldConstructed elements
	// Otherwise -> Allocate + MoveConstruct + destroy and free the old block
	// The remainder of the new block is left uninitialized
	// oldConstructed == 0 -> frees ptr (if oldCapacity > 0) and allocates a fresh block
	static PointerType ReallocateConstructed(
		AllocatorType& allocator,
		PointerType ptr,
		SizeType newCapacity,
		SizeType oldCapacity,
		SizeType oldConstructed,
		uint32 alignment = ElementAlignment) noexcept;

	// Deallocate
	// -------------------------------------------------------------------------

	// Frees storage only — elements must already be destroyed
	// Null handling is defined by the allocator
	static void Deallocate(
		AllocatorType& allocator,
		PointerType ptr,
		uint32 alignment = ElementAlignment) noexcept;

	// Destroys [0, numConstructed), then frees the block
	// numConstructed == 0 -> skips destruction
	static void DeallocateConstructed(
		AllocatorType& allocator,
		PointerType ptr,
		SizeType numConstructed,
		uint32 alignment = ElementAlignment) noexcept;

	// Policy
	// -------------------------------------------------------------------------
	// Policy result is a hint, Ops enforces the bounds, callers can use the result as is
	// newCapacity == oldCapacity is a no-op, returns oldCapacity (policy not consulted)

	// Returns a capacity for growing from oldCapacity to fit newCapacity
	// Requires newCapacity >= oldCapacity
	// Result >= newCapacity
	static SizeType CalculateGrow(
		AllocatorType& allocator,
		SizeType newCapacity,
		SizeType oldCapacity,
		uint32 alignment = ElementAlignment) noexcept;

	// Returns a capacity for shrinking from oldCapacity toward newCapacity
	// Requires newCapacity <= oldCapacity
	// newCapacity <= result <= oldCapacity, may keep slack (result > newCapacity)
	static SizeType CalculateShrink(
		AllocatorType& allocator,
		SizeType newCapacity,
		SizeType oldCapacity,
		uint32 alignment = ElementAlignment) noexcept;
};

#include "Kor/Memory/Detail/AllocatorOps.inl"

KOR_NAMESPACE_END
