// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include <cstdarg>
#include <stdint.h>

// This file shouldn't have any kor dependencies
// * Can include platform types and macros
// * Can define macros
// * Can NOT define kor types (restriction is only for this Build.h, not other platform files)

// BUILD
////////////////////////////////////////////////////////////////////////

#define KOR_DO_PRAGMA(command) _Pragma(#command)

#define KOR_ASSEMBLY(code) __asm__(code)

#define KOR_HAS_INCLUDE(file) __has_include(file)

#define KOR_DEPRECATED __attribute__ ((__deprecated__))
#define KOR_DEPRECATED_MSG(msg) __attribute__ ((__deprecated__(msg)))
#define KOR_NODISCARD [[__nodiscard__]]
#define KOR_NORETURN __attribute__((noreturn))

#define KOR_FORCEINLINE inline __attribute__((always_inline))
#define KOR_FORCENOINLINE __attribute__((noinline))

#define KOR_UNREACHABLE_CODE() __builtin_unreachable()

#if KOR_BUILD_DEBUG
	#define KOR_FORCEINLINE_DEBUGGABLE inline
#else
	#define KOR_FORCEINLINE_DEBUGGABLE KOR_FORCEINLINE
#endif

#if KOR_COMPILER_CLANG
	#define KOR_DEBUG_BREAK() __builtin_debugtrap()
#else
	#define KOR_DEBUG_BREAK() __asm__ volatile("int3")
#endif

#define KOR_DLL_EXPORT
#define KOR_DLL_IMPORT

// DIAGNOSTICS
////////////////////////////////////////////////////////////////////////

#define KOR_DIAG_WARNINGS_PUSH() KOR_DO_PRAGMA(GCC diagnostic push)
#define KOR_DIAG_WARNINGS_POP() KOR_DO_PRAGMA(GCC diagnostic pop)
#define KOR_DIAG_WARNINGS_SUPPRESS(value) KOR_DO_PRAGMA(GCC diagnostic ignored value)

#define KOR_DIAG_WARNING_UNUSED_VALUE "-Wunused-value"
#define KOR_DIAG_WARNING_NULL_DEREFERENCE "-Wnull-dereference"
#define KOR_DIAG_WARNING_IMPLICIT_NARROWING "-Wconversion"
#define KOR_DIAG_WARNING_SHIFT_OVERFLOW "-Wno-shift-overflow"
#define KOR_DIAG_WARNING_SHIFT_NEGATIVE_VALUE "-Wno-shift-negative-value"

#if KOR_DEFAULT_WARNING_SUPPRESS
	KOR_DIAG_WARNINGS_SUPPRESS(KOR_DIAG_WARNING_IMPLICIT_NARROWING)
#endif

// OPTIMIZATIONS
// * Uses options, make sure it does not colide with anything
// * Otherwise update macros
////////////////////////////////////////////////////////////////////////

#define KOR_OPTIMIZATIONS_DISABLE() KOR_DO_PRAGMA(GCC push_options) KOR_DO_PRAGMA(GCC optimize("O0"))
#define KOR_OPTIMIZATIONS_RESET() KOR_DO_PRAGMA(GCC pop_options)

// Prediction
////////////////////////////////////////////////////////////////////////

#define KOR_LIKELY(x) __builtin_expect(!!(x), 1)
#define KOR_UNLIKELY(x) __builtin_expect(!!(x), 0)

// Integers
////////////////////////////////////////////////

#define KOR_PLATFORM_INT8 int8_t
#define KOR_PLATFORM_UINT8 uint8_t

#define KOR_PLATFORM_INT16 int16_t
#define KOR_PLATFORM_UINT16 uint16_t

#define KOR_PLATFORM_INT32 int32_t
#define KOR_PLATFORM_UINT32 uint32_t

#define KOR_PLATFORM_INT64 int64_t
#define KOR_PLATFORM_UINT64 uint64_t

// Characters
////////////////////////////////////////////////

#define KOR_PLATFORM_WCHAR wchar_t
