// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

// This file shouldn't have any kor dependencies
// * Can include platform types and macros
// * Can define macros
// * Can NOT define kor types (restriction is only for this Build.h, not other platform files)

// Windows setup
////////////////////////////////////////////////////////////////////////

#if KOR_USE_UNICODE && !defined(UNICODE)
	#define UNICODE
#endif

#if !defined(WIN32_LEAN_AND_MEAN)
	// Prevents pulling unnecessary Windows headers
	#define WIN32_LEAN_AND_MEAN
#endif

#if !defined(NOMINMAX)
	// Prevents windows to define min/max
	#define NOMINMAX
#endif

#pragma warning(disable : 4996)
#if !defined(_CRT_NONSTDC_NO_WARNINGS)
	// Disable CRT warnings
	#define _CRT_NONSTDC_NO_WARNINGS
#endif

#include <Windows.h>
#include <intrin.h>

#ifdef GetCommandLine
	#undef GetCommandLine
#endif

// BUILD
////////////////////////////////////////////////////////////////////////

#define KOR_DO_PRAGMA(command) __pragma(command)

#define KOR_ASSEMBLY(code) __asm(code)

#define KOR_HAS_INCLUDE(file) __has_include(file)

#define KOR_DEPRECATED __declspec(deprecated)
#define KOR_NODISCARD __declspec(nodiscard)

#define KOR_FORCEINLINE __forceinline
#define KOR_FORCENOINLINE __declspec(noinline)

#if KOR_BUILD_DEBUG
	#define KOR_FORCEINLINE_DEBUGGABLE __inline
#else
	#define KOR_FORCEINLINE_DEBUGGABLE KOR_FORCEINLINE
#endif

#define KOR_DEBUG_BREAK() __debugbreak()

#define KOR_DLL_EXPORT __declspec(dllexport)
#define KOR_DLL_IMPORT __declspec(dllimport)

// DIAGNOSTICS
////////////////////////////////////////////////////////////////////////

#define KOR_DIAG_WARNINGS_PUSH() KOR_DO_PRAGMA( warning( push ) )
#define KOR_DIAG_WARNINGS_POP() KOR_DO_PRAGMA( warning( pop ) )
#define KOR_DIAG_WARNINGS_SUPPRESS(value) KOR_DO_PRAGMA( warning( disable : value ) )

#define KOR_DIAG_WARNING_UNUSED_VALUE 4101
#define KOR_DIAG_WARNING_NULL_DEREFERENCE 6011
#define KOR_DIAG_WARNING_IMPLICIT_NARROWING 4244
#define KOR_DIAG_WARNING_SHIFT_OVERFLOW 4293
#define KOR_DIAG_WARNING_SHIFT_NEGATIVE_VALUE 4146

#if KOR_DEFAULT_WARNING_SUPPRESS
	KOR_DIAG_WARNINGS_SUPPRESS(KOR_DIAG_WARNING_IMPLICIT_NARROWING)
#endif

// Prediction
////////////////////////////////////////////////////////////////////////

#define KOR_LIKELY(x) (x)
#define KOR_UNLIKELY(x) (x)

// OPTIMIZATIONS
////////////////////////////////////////////////////////////////////////

#define KOR_OPTIMIZATIONS_DISABLE() KOR_DO_PRAGMA( optimize("", off) )
#define KOR_OPTIMIZATIONS_RESET() KOR_DO_PRAGMA( optimize("", on) )

// Integers
////////////////////////////////////////////////

#define KOR_PLATFORM_INT8 __int8;
#define KOR_PLATFORM_UINT8 unsigned __int8;

#define KOR_PLATFORM_INT16 __int16;
#define KOR_PLATFORM_UINT16 unsigned __int16;

#define KOR_PLATFORM_INT32 __int32;
#define KOR_PLATFORM_UINT32 unsigned __int32;

#define KOR_PLATFORM_INT64 __int64;
#define KOR_PLATFORM_UINT64 unsigned __int64;

// Characters
////////////////////////////////////////////////

#define KOR_PLATFORM_WCHAR __wchar_t;
