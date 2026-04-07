// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Build.h"

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

// Strings
////////////////////////////////////////////////

#define KOR_WCHAR_BYTES 4

#define KOR_CHAR_NEWLINE_WIDE L"\n"
#define KOR_CHAR_NEWLINE_ANSI "\n"

// Types
////////////////////////////////////////////////

KOR_NAMESPACE_BEGIN

struct SLinuxTypes
{
	typedef int8_t Int8;
	typedef uint8_t Uint8;

	typedef int16_t Int16;
	typedef uint16_t Uint16;

	typedef int32_t Int32;
	typedef uint32_t Uint32;

	typedef int64_t Int64;
	typedef uint64_t Uint64;

	typedef char Achar;
	typedef wchar_t Wchar;

	typedef char8_t Char8;
	typedef char16_t Char16;
	typedef char32_t Char32;
};

KOR_NAMESPACE_END