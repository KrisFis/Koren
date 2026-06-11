// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include KOR_PLATFORM_HEADER_FROM(Kor/Core, Memory)

#include <new>

KOR_NAMESPACE_BEGIN

// Platform typedefs type
using SPlatformMemoryOps = KOR_PLATFORM_STRUCT(MemoryOps);

// [ Memory Constant ]
// Conversion factors from bytes to common memory units, and standard buffer sizes.
struct SMemoryConstant
{
	static constexpr long double b_PER_BYTE = 8; // bits
	static constexpr long double KiB_PER_BYTE = 1.e-3; // kibibytes
	static constexpr long double Kb_PER_BYTE = 7.8e-3; // kilobits
	static constexpr long double KB_PER_BYTE = 1.e-3; // kilobytes
	static constexpr long double MiB_PER_BYTE = 9.53674e-7; // mebibytes
	static constexpr long double Mb_PER_BYTE = 7.629395e-6; // megabits
	static constexpr long double MB_PER_BYTE = 1.e-6; // megabytes
	static constexpr long double GiB_PER_BYTE = 9.31e-10; // gibibytes
	static constexpr long double Gb_PER_BYTE = 7.451e-9; // gigabits
	static constexpr long double GB_PER_BYTE = 1.e-9; // gigabytes
	static constexpr long double TiB_PER_BYTE = 1.e-12; // tebibytes
	static constexpr long double Tb_PER_BYTE = 7.e-12; // terabits
	static constexpr long double TB_PER_BYTE = 1.e-12; // terabytes
};

// [ Memory Ops ]
// Platform-agnostic memory operations.
// * Typed variants (As suffix) operate on element counts, not byte sizes
// * Typed variants dispatch on TTypeTraits to use construction/destruction where needed
// * Raw variants operate on bytes and forward directly to SPlatformMemoryOps
struct SMemoryOps
{
	// Free
	// Releases a previously allocated block.
	// -------------------------------------------------------------------------

	static void Free(void* ptr) noexcept;

	// Malloc / MallocAs
	// Allocates uninitialized memory. MallocAs takes an element count; Malloc takes a byte size.
	// -------------------------------------------------------------------------

	static void* Malloc(int64 size) noexcept;
	template<typename T> static T* MallocAs(int64 num = 1) noexcept;

	// Calloc / CallocAs
	// Allocates zero-initialized memory. CallocAs takes an element count; Calloc takes a byte size.
	// -------------------------------------------------------------------------

	static void* Calloc(int64 size) noexcept;
	template<typename T> static T* CallocAs(int64 num = 1) noexcept;

	// Realloc / ReallocAs
	// Resizes a previously allocated block. ReallocAs takes an element count; Realloc takes a byte size.
	// -------------------------------------------------------------------------

	static void* Realloc(void* ptr, int64 size) noexcept;
	template<typename T> static T* ReallocAs(T* ptr, int64 num = 1) noexcept;

	// Copy / CopyAs
	// Copies elements from src to dest. CopyAs invokes copy construction for non-bitwise-copyable types.
	// * Ranges must not overlap; use Move/MoveAs for overlapping regions
	// -------------------------------------------------------------------------

	static void* Copy(void* dest, const void* src, int64 size) noexcept;
	template<typename T> static void CopyAs(T* to, const T* from, int64 num = 1) noexcept;

	// Move / MoveAs
	// Moves a single element from src to dest. MoveAs invokes move construction for non-bitwise-movable types.
	// * MoveAs operates on a single element; use CopyAs for ranges
	// -------------------------------------------------------------------------

	static void* Move(void* dest, const void* src, int64 size) noexcept;
	template<typename T> static void MoveAs(T* to, T* from) noexcept;

	// Fill / FillAs
	// Fills memory with a repeated value. FillAs invokes copy construction for non-bitwise-copyable types.
	// * Fill sets each byte to val (same semantics as memset)
	// -------------------------------------------------------------------------

	static void* Fill(void* dest, int32 val, int64 size) noexcept;
	template<typename T> static void FillAs(const T* dst, T val, int64 num = 1) noexcept;

	// Zero / ZeroAs
	// Zeroes memory. ZeroAs invokes default construction for non-bitwise-copyable types.
	// -------------------------------------------------------------------------

	static void* Zero(void* dest, int64 size) noexcept;
	template<typename T> static void ZeroAs(const T* dst, int64 num = 1) noexcept;

	// Compare / CompareAs
	// Compares two memory regions. Returns negative, zero, or positive like memcmp.
	// * CompareAs compares element-by-element; ordering uses operator< for non-bitwise-comparable types
	// -------------------------------------------------------------------------

	static int32 Compare(const void* lhs, const void* rhs, int64 size) noexcept;
	template<typename T> static int32 CompareAs(const T* lhs, const T* rhs, int64 num = 1) noexcept;

	// IsEqual / IsEqualAs
	// Returns true if both memory regions are identical.
	// * IsEqualAs uses operator== for non-bitwise-comparable types
	// -------------------------------------------------------------------------

	static bool IsEqual(const void* lhs, const void* rhs, int64 size) noexcept;
	template<typename T> static bool IsEqualAs(const T* lhs, const T* rhs, int64 num = 1) noexcept;

	// Construct
	// Constructs a T in-place at ptr. Zero-initializes if trivially constructible.
	// -------------------------------------------------------------------------

	template<typename T, typename... ArgTypes>
	static void Construct(T* ptr, ArgTypes&&... Args) noexcept;

	// Destruct
	// Invokes the destructor of T at ptr. No-op if trivially destructible.
	// -------------------------------------------------------------------------

	template<typename T>
	static void Destruct(T* ptr) noexcept;
};

#include "Kor/Inl/MemoryOps.inl"

KOR_NAMESPACE_END
