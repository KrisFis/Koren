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
	static void Free(void* ptr, uint32 alignment) noexcept;

	// Malloc / MallocAs
	// Allocates uninitialized memory. MallocAs takes an element count; Malloc takes a byte size.
	// * Once allocated with a given `alignment`, all subsequent calls on that
	//   block (Free/Realloc) must be passed the SAME alignment.
	// -------------------------------------------------------------------------

	static void* Malloc(uint64 size) noexcept;
	static void* Malloc(uint64 size, uint32 alignment) noexcept;
	template<typename T> static T* MallocAs(uint64 num = 1) noexcept;
	template<typename T> static T* MallocAs(uint64 num, uint32 alignment) noexcept;

	// Calloc / CallocAs
	// Allocates zero-initialized memory. CallocAs takes an element count; Calloc takes a byte size.
	// * Once allocated with a given `alignment`, all subsequent calls on that
	//   block (Free/Realloc) must be passed the SAME alignment.
	// -------------------------------------------------------------------------

	static void* Calloc(uint64 size) noexcept;
	static void* Calloc(uint64 size, uint32 alignment) noexcept;
	template<typename T> static T* CallocAs(uint64 num = 1) noexcept;
	template<typename T> static T* CallocAs(uint64 num, uint32 alignment) noexcept;

	// Realloc / ReallocAs
	// Resizes a previously allocated block. ReallocAs takes an element count; Realloc takes a byte size.
	// * `alignment` must match the value originally used to allocate `ptr`.
	// -------------------------------------------------------------------------

	static void* Realloc(void* ptr, uint64 size) noexcept;
	static void* Realloc(void* ptr, uint64 size, uint32 alignment) noexcept;
	template<typename T> static T* ReallocAs(T* ptr, uint64 num = 1) noexcept;
	template<typename T> static T* ReallocAs(T* ptr, uint64 num, uint32 alignment) noexcept;

	// Copy / CopyConstruct / CopyAssign
	// Copies elements from src to dest.
	// * Ranges must not overlap; use Move instead if they might.
	// * CopyConstruct: dest is RAW/UNINITIALIZED memory (placement copy-construct).
	// * CopyAssign: dest holds ALREADY-LIVE objects (copy assignment operator).
	// * Both bitwise-copy when T is trivially copyable.
	// -------------------------------------------------------------------------
	static void* Copy(void* dest, const void* src, uint64 size) noexcept;
	template<typename T> static void CopyConstruct(T* dest, const T* src, uint64 num = 1) noexcept;
	template<typename T> static void CopyAssign(T* dest, const T* src, uint64 num = 1) noexcept;

	// Move / MoveConstruct / MoveAssign
	// Moves elements from src to dest.
	// * MoveConstruct: dest is RAW/UNINITIALIZED memory (placement move-construct).
	//   Ranges must not overlap. Source is left unchanged (NOT destructed) — caller owns that lifetime.
	// * MoveAssign: dest holds ALREADY-LIVE objects (move assignment operator).
	//   Overlap-safe and direction-aware — correct for both a shift-left erase and
	//   a shift-right insert in the same call. Source is left in a valid-but-moved-from
	//   state and still needs an explicit Destruct if its lifetime is ending.
	// * Both bitwise-copy when T is trivially movable.
	// -------------------------------------------------------------------------
	static void* Move(void* dest, const void* src, uint64 size) noexcept;
	template<typename T> static void MoveConstruct(T* dest, T* src, uint64 num = 1) noexcept;
	template<typename T> static void MoveAssign(T* dest, T* src, uint64 num = 1) noexcept;

	// Fill / FillConstruct / FillAssign
	// Fills memory with `num` copies of val.
	// * Fill sets each byte to val (memset semantics) — bit pattern only, not element-aware.
	// * FillConstruct: dest is RAW/UNINITIALIZED memory (placement copy-construct per element).
	// * FillAssign: dest holds ALREADY-LIVE objects (copy assignment operator per element).
	// * Neither falls back to the byte-level Fill — val may not be a single repeated byte,
	//   so both always construct/assign element-by-element for correctness.
	// -------------------------------------------------------------------------
	static void* Fill(void* ptr, int32 val, uint64 size) noexcept;
	template<typename T> static void FillConstruct(T* ptr, const T& val, uint64 num = 1) noexcept;
	template<typename T> static void FillAssign(T* ptr, const T& val, uint64 num = 1) noexcept;

	// Zero / ZeroConstruct / ZeroAssign
	// Force-zeros the underlying bytes of `num` elements at ptr.
	// * Zero sets each byte to 0 (memset semantics).
	// * ZeroConstruct: dest is RAW/UNINITIALIZED memory — bypasses T's constructor entirely.
	// * ZeroAssign: dest holds ALREADY-LIVE objects — bypasses T's assignment operator AND
	//   does not run its destructor first.
	// * Only valid for types where an all-zero bit pattern is a legal state; for ZeroAssign,
	//   also only valid where skipping the old value's destructor is safe (no owned resources).
	// -------------------------------------------------------------------------
	static void* Zero(void* ptr, uint64 size) noexcept;
	template<typename T> static void ZeroConstruct(T* ptr, uint64 num = 1) noexcept;
	template<typename T> static void ZeroAssign(T* ptr, uint64 num = 1) noexcept;

	// Swap
	// Swaps values between lhs and rhs.
	// * Ranges must not overlap
	// * SwapAs invokes move construction for non-bitwise-movable types.
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

	// Destruct
	// Invokes the destructor of T at ptr, for `num` elements. No-op if trivially destructible.
	// -------------------------------------------------------------------------

	template<typename T>
	static void Destruct(T* ptr, uint64 num = 1) noexcept;
};

#include "Kor/Inl/MemoryOps.inl"

KOR_NAMESPACE_END
