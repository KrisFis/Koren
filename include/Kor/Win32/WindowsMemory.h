// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Win32/WindowsBuild.h"

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
struct SWindowsPlatformMemory
{
	// Allocates new memory
	FORCEINLINE static void* Malloc(int64 size) { return HeapAlloc(GetProcessHeap(), 0, size); }

	// Allocates new memory and sets every bit to zero
	FORCEINLINE static void* Calloc(int64 size) { return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size); }

	// Reallocates memory preferably in the same memory block
	FORCEINLINE static void* Realloc(void* ptr, int64 size) { return HeapReAlloc(GetProcessHeap(), 0, ptr, size); }

	// Deallocates memory
	FORCEINLINE static void Free(void* ptr) { HeapFree(GetProcessHeap(), 0, ptr); }

	// Copies block of memory from destionation to source (does not handle overlapping)
	FORCEINLINE static void* Copy(void* dest, const void* src, int64 size) { return CopyMemory(dest, src, size); }

	// Copies block of memory from destionation to source (handles overlapping)
	FORCEINLINE static void* Move(void* dest, const void* src, int64 size) { return MoveMemory(dest, src, size); }

	// Fills block of memory with specific value
	FORCEINLINE static void* Fill(void* dest, int32 val, int64 size) { return FillMemory(dest, size, val); }

	// Fills block of memory with zero value
	FORCEINLINE static void* Zero(void* dest, int64 size) { return ZeroMemory(dest, size); }

	// Compares two blocks of memory
	FORCEINLINE static int32 Compare(const void* lhs, const void* rhs, int64 num) { return RtlEqualMemory(lhs, rhs, num); }
};
