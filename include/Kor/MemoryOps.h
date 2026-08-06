// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/Math.h"
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
// * Typed variants operate on element counts, not byte sizes
struct SMemoryOps
{
	// Free
	// Releases a previously allocated block.
	// * `alignment` must match the value passed to the corresponding Malloc/Calloc/Realloc call.
	// -------------------------------------------------------------------------

	static void Free(void* ptr) noexcept;
	static void Free(void* ptr, uint64 alignment) noexcept;

	// Malloc / MallocAs
	// Allocates uninitialized memory. MallocAs takes an element count; Malloc takes a byte size.
	// * Once allocated with a given `alignment`, all subsequent calls on that
	//   block (Free/Realloc) must be passed the SAME alignment.
	// -------------------------------------------------------------------------

	static void* Malloc(uint64 size) noexcept;
	static void* Malloc(uint64 size, uint64 alignment) noexcept;
	template<typename T> static T* MallocAs(uint64 num = 1) noexcept;
	template<typename T> static T* MallocAs(uint64 num, uint64 alignment) noexcept;

	// Calloc / CallocAs
	// Allocates zero-initialized memory. CallocAs takes an element count; Calloc takes a byte size.
	// * Once allocated with a given `alignment`, all subsequent calls on that
	//   block (Free/Realloc) must be passed the SAME alignment.
	// -------------------------------------------------------------------------

	static void* Calloc(uint64 size) noexcept;
	static void* Calloc(uint64 size, uint64 alignment) noexcept;
	template<typename T> static T* CallocAs(uint64 num = 1) noexcept;
	template<typename T> static T* CallocAs(uint64 num, uint64 alignment) noexcept;

	// Realloc / ReallocAs
	// Resizes a previously allocated block. ReallocAs takes an element count; Realloc takes a byte size.
	// * `alignment` must match the value originally used to allocate `ptr`.
	// -------------------------------------------------------------------------

	static void* Realloc(void* ptr, uint64 size) noexcept;
	static void* Realloc(void* ptr, uint64 size, uint64 alignment) noexcept;
	template<typename T> static T* ReallocAs(T* ptr, uint64 num = 1) noexcept;
	template<typename T> static T* ReallocAs(T* ptr, uint64 num, uint64 alignment) noexcept;

	// Copy / CopyAs
	// Copies elements from src to dest. CopyAs invokes copy construction for non-bitwise-copyable types.
	// * Ranges must not overlap; Use Move instead
	// * CopyAs invokes copy construction for non-bitwise-copyable types
	// -------------------------------------------------------------------------

	static void* Copy(void* dest, const void* src, uint64 size) noexcept;
	template<typename T> static void CopyAs(T* dest, const T* src, uint64 num = 1) noexcept;

	// Move / MoveAs
	// Moves a single element from src to dest.
	// * MoveAs ranges must not overlap for non-bitwise-movable types
	// * MoveAs invokes move construction for non-bitwise-movable types
	// -------------------------------------------------------------------------

	static void* Move(void* dest, const void* src, uint64 size) noexcept;
	template<typename T> static void MoveAs(T* dest, T* src, uint64 num = 1) noexcept;

	// Fill / FillAs
	// Fills memory with a repeated value.
	// * FillAs invokes copy construction for non-bitwise-copyable types.
	// * Fill sets each byte to val (same semantics as memset)
	// -------------------------------------------------------------------------

	static void* Fill(void* dest, int32 val, uint64 size) noexcept;
	template<typename T> static void FillAs(const T* dst, T val, uint64 num = 1) noexcept;

	// Zero / ZeroAs
	// Zeroes memory.
	// * ZeroAs invokes default construction for non-bitwise-copyable types.
	// -------------------------------------------------------------------------

	static void* Zero(void* dest, uint64 size) noexcept;
	template<typename T> static void ZeroAs(const T* dst, uint64 num = 1) noexcept;

	// Swap
	// Swaps values between lhs and rhs.
	// * Ranges must not overlap
	// * SwapAs invokes default construction for non-bitwise-movable types.
	// -------------------------------------------------------------------------

	static void Swap(void* lhs, void* rhs, uint64 size) noexcept;
	template<typename T> static void SwapAs(T* lhs, T* rhs, uint64 num = 1) noexcept;

	// Compare / CompareAs
	// Compares two memory regions. Returns negative, zero, or positive like memcmp.
	// * Ranges must not overlap
	// * CompareAs compares element-by-element; ordering uses operator< for non-bitwise-comparable types
	// -------------------------------------------------------------------------

	static int32 Compare(const void* lhs, const void* rhs, uint64 size) noexcept;
	template<typename T> static int32 CompareAs(const T* lhs, const T* rhs, uint64 num = 1) noexcept;

	// IsEqual / IsEqualAs
	// Returns true if both memory regions are identical.
	// * Ranges must not overlap
	// * IsEqualAs uses operator== for non-bitwise-comparable types
	// -------------------------------------------------------------------------

	static bool IsEqual(const void* lhs, const void* rhs, uint64 size) noexcept;
	template<typename T> static bool IsEqualAs(const T* lhs, const T* rhs, uint64 num = 1) noexcept;

	// Construct
	// Constructs a T in-place from arbitrary args. Single element only.
	// * Zero-initializes if no args and trivially constructible.
	// * Bitwise-copies/moves if the single arg is a T and trivially copy/move constructible.
	// -------------------------------------------------------------------------

	template<typename T, typename... ArgsT>
	static void Construct(T* ptr, ArgsT&&... Args) noexcept;

	// DefaultConstruct
	// Default-constructs `num` elements at ptr. Zero-fills if trivially constructible.
	// -------------------------------------------------------------------------

	template<typename T>
	static void DefaultConstruct(T* ptr, uint64 num = 1) noexcept;

	// CopyConstruct
	// Copy-constructs `num` elements at dest, one-to-one from src.
	// * Bitwise-copies when T == R and trivially copy-constructible.
	// * Ranges must not overlap.
	// -------------------------------------------------------------------------

	template<typename T, typename R>
	static void CopyConstruct(T* dest, const R* src, uint64 num = 1) noexcept;

	// MoveConstruct
	// Move-constructs `num` elements at dest, one-to-one from src.
	// * Bitwise-copies (src left unchanged) when T == R and trivially move-constructible.
	// * Ranges must not overlap.
	// -------------------------------------------------------------------------

	template<typename T, typename R>
	static void MoveConstruct(T* dest, R* src, uint64 num = 1) noexcept;

	// Destruct
	// Invokes the destructor of T at ptr, for `num` elements. No-op if trivially destructible.
	// -------------------------------------------------------------------------

	template<typename T>
	static void Destruct(T* ptr, uint64 num = 1) noexcept;
};

#include "Kor/Inl/MemoryOps.inl"

KOR_NAMESPACE_END
