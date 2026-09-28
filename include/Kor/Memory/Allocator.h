// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/MemoryOps.h"

KOR_NAMESPACE_BEGIN

// Reference model for the allocator
// * Each allocator must expose "Typed" and/or "Untyped"
// * Typed/Untyped types must satisfy TIsAllocator concept
struct CAllocator
{
	template<typename T, uint32 DefaultAlignment = alignof(T)>
	struct Typed
	{
		// Allocate
		// * Allocates storage for num elements, using ElementType's natural alignment.
		// @param num - Number of elements to allocate storage for.
		// @param alignment - [OPTIONAL] Required alignment of the returned block, in bytes.
		// @return Pointer to the allocated elements, or nullptr on failure.

		KOR_FORCEINLINE_DEBUG T* Allocate(int32 num = 1, uint32 alignment = DefaultAlignment) noexcept
		{
			KOR_ASSERT_DEBUG(num > 0);
			return (T*)SMemoryOps::Malloc(num * sizeof(T), alignment);
		}

		// Reallocate [OPTIONAL]
		// @param ptr - Block previously returned by Allocate/Reallocate.
		// @param num - New number of elements the block should hold.
		// @param alignment - [OPTIONAL] Required alignment of the returned block, in bytes.
		// @return Pointer to the (possibly relocated) elements, or nullptr on failure.

		KOR_FORCEINLINE_DEBUG T* Reallocate(T* ptr, int32 num, uint32 alignment = DefaultAlignment) noexcept
		{
			KOR_ASSERT_DEBUG(num > 0);
			return (T*)SMemoryOps::Realloc(ptr, num * sizeof(T), alignment);
		}

		// Free
		// Frees a block previously returned by Allocate/Reallocate, using T's natural alignment.
		// @param ptr - Block to free.
		// @param alignment - [OPTIONAL] Alignment the block was originally allocated with.

		KOR_FORCEINLINE_DEBUG void Deallocate(T* ptr, uint32 alignment = DefaultAlignment) noexcept
		{
			KOR_ASSERT_DEBUG(ptr != nullptr);
			SMemoryOps::Free(ptr, alignment);
		}
	};

	using Untyped = Typed<uint8, KOR_DEFAULT_HEAP_ALIGNMENT>;
};

KOR_NAMESPACE_END
