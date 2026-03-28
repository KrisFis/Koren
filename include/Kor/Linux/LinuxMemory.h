// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Kor/Build.h"

#include <cstdlib>
#include <memory.h>

KOR_NAMESPACE_BEGIN

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://linux.die.net/man/2/mmap
struct SLinuxPlatformMemory
{
	// Allocates new memory
	KOR_FORCEINLINE static void* Malloc(int64 size) { return malloc(size); }

	// Allocates new memory and sets every bit to zero
	KOR_FORCEINLINE static void* Calloc(int64 size)	{ return calloc(size, 1); }

	// Reallocates memory preferably in the same memory block
	KOR_FORCEINLINE static void* Realloc(void* ptr, int64 size) { return realloc(ptr, size); }

	// Deallocates memory
	KOR_FORCEINLINE static void Free(void* ptr) { return free(ptr); }

	// Copies block of memory from destionation to source (does not handle overlapping)
	KOR_FORCEINLINE static void* Copy(void* dest, const void* src, int64 size) { return memcpy(dest, src, size); }

	// Copies block of memory from destionation to source (handles overlapping)
	KOR_FORCEINLINE static void* Move(void* dest, const void* src, int64 size) { return memmove(dest, src, size); }
	
	// Fills block of memory with specific value
	KOR_FORCEINLINE static void* Fill(void* dest, int32 val, int64 size) { return memset(dest, val, size); }

	// Fills block of memory with zero value
	KOR_FORCEINLINE static void* Zero(void* dest, int64 size) { return memset(dest, 0, size); }

	// Compares two blocks of memory
	KOR_FORCEINLINE static int32 Compare(const void* lhs, const void* rhs, int64 num) { return memcmp(lhs, rhs, num); }
};

KOR_NAMESPACE_END