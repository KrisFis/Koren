// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Windows setup

#if ASTD_USE_UNICODE && !defined(UNICODE)
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

// COMPILATION & BUILD
////////////////////////////////////////////////////////////////////////

#define DO_PRAGMA(command) __pragma(command)

#define ASSEMBLY(code) __asm(code)

#define HAS_INCLUDE(file) __has_include(file)

#define DEPRECATED __declspec(deprecated)
#define NODISCARD __declspec(nodiscard)

#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)

#if BUILD_DEBUG
	#define FORCEINLINE_DEBUGGABLE __inline
#else
	#define FORCEINLINE_DEBUGGABLE FORCEINLINE
#endif

#define DEBUG_BREAK() __debugbreak()

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

// DIAGNOSTICS
////////////////////////////////////////////////////////////////////////

#define DIAG_WARNINGS_PUSH() DO_PRAGMA( warning( push ) )
#define DIAG_WARNINGS_POP() DO_PRAGMA( warning( pop ) )
#define DIAG_WARNINGS_SUPPRESS(value) DO_PRAGMA( warning( disable : value ) )

#define DIAG_WARNING_UNUSED_VALUE 4101
#define DIAG_WARNING_NULL_DEREFERENCE 6011
#define DIAG_WARNING_IMPLICIT_NARROWING 4244
#define DIAG_WARNING_SHIFT_OVERFLOW 4293
#define DIAG_WARNING_SHIFT_NEGATIVE_VALUE 4146

#if ASTD_DEFAULT_WARNING_SUPPRESS
	DIAG_WARNINGS_SUPPRESS(DIAG_WARNING_IMPLICIT_NARROWING)
#endif

// Prediction
////////////////////////////////////////////////////////////////////////

#define LIKELY(x) (x)
#define UNLIKELY(x) (x)

// OPTIMIZATIONS
////////////////////////////////////////////////////////////////////////

#define OPTIMIZATIONS_DISABLE() DO_PRAGMA( optimize("", off) )
#define OPTIMIZATIONS_RESET() DO_PRAGMA( optimize("", on) )

// Integers
////////////////////////////////////////////////

typedef __int8 int8;
typedef unsigned __int8 uint8;

typedef __int16 int16;
typedef unsigned __int16 uint16;

typedef __int32 int32;
typedef unsigned __int32 uint32;

typedef __int64 int64;
typedef unsigned __int64 uint64;

// Characters
////////////////////////////////////////////////

typedef __wchar_t wchar;
