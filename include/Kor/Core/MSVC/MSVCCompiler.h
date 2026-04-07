// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

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