// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/MemoryOps.h"

#include "Kor/TypeTrait/Integer.h"

#include "Kor/Utility/SizeAlignOf.h"

KOR_NAMESPACE_BEGIN

// FAMILY CONTRACT
// -------------------------------------------------------------------------
// A family exposes three members:
//   - `SizeType` : integral type used for every size/count in the family
//   - `Typed<T>` : allocator template for objects of type T
//   - `Untyped`  : allocator for raw bytes
//
// `Typed<T>` and `Untyped` must satisfy TIsAllocator. Or left void/undeclared.
//
// `SizeType` is the single source of truth:
// * allocators use it for every size/count parameter, and TAllocatorOps/containers take it from the family.
//
// ALLOCATOR CONTRACT (for `Typed<T>` and `Untyped`)
// -------------------------------------------------------------------------
// Typed allocators return `T*` from `Allocate`; untyped ones return `void*` and count in bytes.
// Declare each method once (no overloads).
// Every size/count parameter must be the family's `SizeType`.
//
// Optional parameters and methods:
// * If declared, the allocator honours it
// * If omitted, the allocator ignores it. Don't add defaults, since TAllocatorOps fills them in.
//
// Failure is reported by returning nullptr. All methods are noexcept.
// -------------------------------------------------------------------------
struct CAllocator
{
	using SizeType = int32;

	template<typename T>
	struct Typed
	{
		// [REQUIRED] Allocates storage for `num` elements (bytes if untyped).
		// @param num       - New element (or byte) count.
		// @param alignment - [OPTIONAL] Required block alignment, in bytes.
		// @return Allocated block, or nullptr on failure.
		T* Allocate(SizeType num, uint32 alignment) noexcept;

		// [OPTIONAL] Resizes a block, possibly moving it; contents are preserved up
		// to the smaller size. Omit it if not natively supported. Containers then
		// fall back to Allocate + copy + Deallocate.
		// @param ptr       - Block from Allocate/Reallocate.
		// @param num       - New element (or byte) count.
		// @param alignment - [OPTIONAL] Must match the original alignment.
		// @return Possibly relocated block, or nullptr on failure (original stays valid).
		T* Reallocate(T* ptr, SizeType newNum, uint32 alignment) noexcept;

		// [REQUIRED] Frees a block from Allocate/Reallocate.
		// @param ptr       - Block to free (not nullptr).
		// @param alignment - [OPTIONAL] Alignment the block was allocated with.
		void Deallocate(T* ptr, uint32 alignment) noexcept;

		// [OPTIONAL] Calculates capacity for growing allocation from oldNum to fit newNum
		// @param newNum    - New element (or byte) count
		// @param oldNum    - Old element (or byte) count
		// @param alignment - [OPTIONAL] Alignment the block was allocated with
		//
		// SizeType CalculateGrow(SizeType newNum, SizeType oldNum, uint32 alignment) const noexcept;

		// [OPTIONAL] Calculates capacity for shrinking allocation from oldNum towards newNum
		// @param newNum    - New element (or byte) count
		// @param oldNum    - Old element (or byte) count
		// @param alignment - [OPTIONAL] Alignment the block was allocated with
		//
		// SizeType CalculateShrink(SizeType newNum, SizeType oldNum, uint32 alignment) const noexcept;
	};

	// Untyped allocator: returns void*.
	using Untyped = Typed<void>;
};

// Default sizing policy.
// * TAllocatorOps falls back to it when an allocator does not declare its own CalculateGrow / CalculateShrink.
//
// Intended as an unclamped intermediate step in the allocator call flow
struct SDefaultAllocationPolicy
{
	// Calculates capacity for growing an allocation from oldNum to fit newNum.
	// @param newNum          - New element count
	// @param oldNum          - Old element count
	// @param bytesPerElement - Size of one element in bytes
	// @param alignment       - Alignment the block was allocated with
	template<typename SizeType>
	static SizeType CalculateGrow(SizeType newNum, SizeType oldNum, uint32 bytesPerElement, uint32 alignment) noexcept;

	// Calculates capacity for shrinking an allocation from oldNum towards newNum.
	// @param newNum          - New element count
	// @param oldNum          - Old element count
	// @param bytesPerElement - Size of one element in bytes
	// @param alignment       - Alignment the block was allocated with
	template<typename SizeType>
	static SizeType CalculateShrink(SizeType newNum, SizeType oldNum, uint32 bytesPerElement, uint32 alignment) noexcept;
};

#include "Kor/Memory/Detail/Allocator.inl"

KOR_NAMESPACE_END
