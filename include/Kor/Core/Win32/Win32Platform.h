// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

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
#define KOR_NODISCARD __declspec(noreturn)
#define KOR_NORETURN __declspec(noreturn)

#define KOR_FORCEINLINE __forceinline
#define KOR_FORCENOINLINE __declspec(noinline)

#define KOR_UNREACHABLE_CODE() __assume(false)

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

// Strings
////////////////////////////////////////////////

#define KOR_WCHAR_BYTES 2

// Types
////////////////////////////////////////////////

KOR_NAMESPACE_BEGIN

struct SWin32Types
{
	typedef __int8 Int8;
	typedef unsigned __int8 Uint8;

	typedef __int16 Int16;
	typedef unsigned __int16 Uint16;

	typedef __int32 Int32;
	typedef unsigned __int32 Uint32;

	typedef __int64 Int64;
	typedef unsigned __int64 Uint64;

	typedef char Achar;
	typedef wchar_t Wchar;

	typedef char8_t Char8;
	typedef char16_t Char16;
	typedef char32_t Char32;
};

KOR_NAMESPACE_END