// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Build.h"

KOR_NAMESPACE_BEGIN

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
struct SWindowsPlatformMemory
{
	// Allocates new memory
	KOR_FORCEINLINE static void* Malloc(int64 size) { return HeapAlloc(GetProcessHeap(), 0, size); }

	// Allocates new memory and sets every bit to zero
	KOR_FORCEINLINE static void* Calloc(int64 size) { return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size); }

	// Reallocates memory preferably in the same memory block
	KOR_FORCEINLINE static void* Realloc(void* ptr, int64 size) { return HeapReAlloc(GetProcessHeap(), 0, ptr, size); }

	// Deallocates memory
	KOR_FORCEINLINE static void Free(void* ptr) { HeapFree(GetProcessHeap(), 0, ptr); }

	// Copies block of memory from destionation to source (does not handle overlapping)
	KOR_FORCEINLINE static void* Copy(void* dest, const void* src, int64 size) { return CopyMemory(dest, src, size); }

	// Copies block of memory from destionation to source (handles overlapping)
	KOR_FORCEINLINE static void* Move(void* dest, const void* src, int64 size) { return MoveMemory(dest, src, size); }

	// Fills block of memory with specific value
	KOR_FORCEINLINE static void* Fill(void* dest, int32 val, int64 size) { return FillMemory(dest, size, val); }

	// Fills block of memory with zero value
	KOR_FORCEINLINE static void* Zero(void* dest, int64 size) { return ZeroMemory(dest, size); }

	// Compares two blocks of memory
	KOR_FORCEINLINE static int32 Compare(const void* lhs, const void* rhs, int64 num) { return RtlEqualMemory(lhs, rhs, num); }
};

KOR_NAMESPACE_END