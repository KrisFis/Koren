// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/MemoryOps.h"
#include "Kor/Utility/SizeAlignOf.h"

KOR_NAMESPACE_BEGIN

// FAMILY CONTRACT
// -------------------------------------------------------------------------
// * A family exposes exactly two members:
//     - `Typed<T>` : allocator template for objects of type T
//     - `Untyped`  : allocator for raw bytes
// * Both must satisfy TIsAllocator.
//
// ALLOCATOR CONTRACT (for `Typed<T>` and `Untyped`)
// -------------------------------------------------------------------------
// * Typed allocators return `T*` from `Allocate`; untyped ones return `void*` and count in bytes.
// * Required: `Allocate`, `Deallocate`. Optional: `Reallocate`.
// * Declare each method once (no overloads).
// * Every size/count parameter in the allocator uses the same type (this model: int32).
// * `alignment` is an optional last parameter.
// ** If declared, the allocator honours it
// ** If omitted, the allocator ignores it. Don't add defaults, since SAllocatorOps fills them in.
// * Failure is reported by returning nullptr. All methods are noexcept.
// -------------------------------------------------------------------------
struct CAllocator
{
	template<typename T>
	struct Typed
	{
		// [REQUIRED] Allocates storage for `num` elements (bytes if untyped).
		// @param alignment - [OPTIONAL PARAMETER] Required block alignment, in bytes.
		// @return Allocated block, or nullptr on failure.
		KOR_FORCEINLINE T* Allocate(int32 num, uint32 alignment) noexcept
		{
			return (T*)SMemoryOps::Malloc(num * SizeOf<T, 1>(), alignment);
		}

		// [OPTIONAL] Resizes a block, possibly moving it; contents are preserved up
		// to the smaller size. Omit it if not natively supported. Containers then
		// fall back to Allocate + copy + Deallocate.
		// @param ptr       - Block from Allocate/Reallocate.
		// @param num       - New element (or byte) count.
		// @param alignment - [OPTIONAL PARAMETER] Must match the original alignment.
		// @return Possibly relocated block, or nullptr on failure (original stays valid).
		KOR_FORCEINLINE T* Reallocate(T* ptr, int32 num, uint32 alignment) noexcept
		{
			return (T*)SMemoryOps::Realloc((void*)ptr, num * SizeOf<T, 1>(), alignment);
		}

		// [REQUIRED] Frees a block from Allocate/Reallocate.
		// @param ptr       - Block to free (not nullptr).
		// @param alignment - [OPTIONAL PARAMETER] Alignment the block was allocated with.
		KOR_FORCEINLINE void Deallocate(T* ptr, uint32 alignment) noexcept
		{
			SMemoryOps::Free((void*)ptr, alignment);
		}
	};

	// Untyped allocator: returns void*.
	using Untyped = Typed<void>;
};

KOR_NAMESPACE_END
