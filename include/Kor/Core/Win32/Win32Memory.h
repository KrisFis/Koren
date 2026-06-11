// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

KOR_NAMESPACE_BEGIN

// TODO(krisfis): Virtual memory allocations
// * see: https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
struct SWin32MemoryOps
{
	// Gets cached handle to process heap
	// * Avoids PEB field read on subsequence calls
	KOR_INLINE static HANDLE GetHeap() noexcept { static HANDLE procHeap = ::GetProcessHeap(); return procHeap; }

	// Allocates new memory
	KOR_FORCEINLINE static void* Malloc(int64 size) noexcept { return HeapAlloc(GetHeap(), 0, size); }

	// Allocates new memory and sets every bit to zero
	KOR_FORCEINLINE static void* Calloc(int64 size) noexcept { return HeapAlloc(GetHeap(), HEAP_ZERO_MEMORY, size); }

	// Reallocates memory preferably in the same memory block
	KOR_FORCEINLINE static void* Realloc(void* ptr, int64 size) noexcept { return HeapReAlloc(GetHeap(), 0, ptr, size); }

	// Deallocates memory
	KOR_FORCEINLINE static void Free(void* ptr) noexcept { HeapFree(GetHeap(), 0, ptr); }

	// Copies block of memory from destination to source (does not handle overlapping)
	KOR_FORCEINLINE static void* Copy(void* dest, const void* src, int64 size) noexcept { return CopyMemory(dest, src, size); }

	// Copies block of memory from destination to source (handles overlapping)
	KOR_FORCEINLINE static void* Move(void* dest, const void* src, int64 size) noexcept { return MoveMemory(dest, src, size); }

	// Fills block of memory with specific value
	KOR_FORCEINLINE static void* Fill(void* dest, int32 val, int64 size) noexcept { return FillMemory(dest, size, val); }

	// Fills block of memory with zero value
	KOR_FORCEINLINE static void* Zero(void* dest, int64 size) noexcept { return ZeroMemory(dest, size); }

	// Compares two blocks of memory
	KOR_FORCEINLINE static int32 Compare(const void* lhs, const void* rhs, int64 size) noexcept { return RtlEqualMemory(lhs, rhs, size); }
};

KOR_NAMESPACE_END