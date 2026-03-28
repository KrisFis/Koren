// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

// Build type
// * Supported: Debug, Release
// * Example: KOR_BUILD_RELEASE
/////////////////////////////////

#ifndef KOR_BUILD_RELEASE
	#define KOR_BUILD_RELEASE 0
#endif

#ifndef KOR_BUILD_DEBUG
	#define KOR_BUILD_DEBUG 0
#endif

#if KOR_BUILD_RELEASE == KOR_BUILD_DEBUG
	#error "Please specify 'KOR_BUILD_RELEASE=1' or 'KOR_BUILD_DEBUG=1'"
#endif

// Compiler
// * Supported: MSVC, GNUC, GNUC_CLANG, GNUC_INTEL, GNUC_GCC
// * Example: COMKOR_MATH_PILER_GNUC_CLANG
/////////////////////////////////

#if defined(_MSC_VER)
	#define KOR_COMKOR_MATH_PILER_MSVC 1
#elif defined(__llvm__) && defined(__clang__)
	#define KOR_COMKOR_MATH_PILER_CLANG 1
#elif defined(__INTEL_COMKOR_MATH_PILER)
	#define KOR_COMKOR_MATH_PILER_INTEL 1
#elif defined(__GNUC__)
	#define KOR_COMKOR_MATH_PILER_GCC 1
#else
	#error "Unsupported compiler"
#endif

#ifndef KOR_COMKOR_MATH_PILER_MSVC
	#define KOR_COMKOR_MATH_PILER_MSVC 0
#endif

#ifndef KOR_COMKOR_MATH_PILER_CLANG
	#define KOR_COMKOR_MATH_PILER_CLANG 0
#endif

#ifndef KOR_COMKOR_MATH_PILER_INTEL
	#define KOR_COMKOR_MATH_PILER_INTEL 0
#endif

#ifndef KOR_COMKOR_MATH_PILER_GCC
	#define KOR_COMKOR_MATH_PILER_GCC 0
#endif

// Architecture
// * Supported: 64, 32
// * Example: KOR_ARCHITECTURE_64
/////////////////////////////////

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
	#define KOR_ARCHITECTURE_64 1
#elif defined(_WIN32) || defined(__i386__)
	#define KOR_ARCHITECTURE_32 1
#else
	#error "Unsupported architecture"
#endif

#ifndef KOR_ARCHITECTURE_32
	#define KOR_ARCHITECTURE_32 0
#endif

#ifndef KOR_ARCHITECTURE_64
	#define KOR_ARCHITECTURE_64 0
#endif

// Platform
// * Supported: Windows, Linux
// * Example: KOR_PLATFORM_WINDOWS
/////////////////////////////////

#if defined(_WIN32) || defined(_WIN64)
	#define KOR_PLATFORM_WINDOWS 1
#elif defined(__linux__)
	#define KOR_PLATFORM_LINUX 1
#elif defined(__APPLE__)
	#define KOR_PLATFORM_APPLE 1
#else
	#error "Unsupported platform"
#endif

#ifndef KOR_PLATFORM_WINDOWS
	#define KOR_PLATFORM_WINDOWS 0
#endif

#ifndef KOR_PLATFORM_LINUX
	#define KOR_PLATFORM_LINUX 0
#endif

#ifndef KOR_PLATFORM_APPLE
	#define KOR_PLATFORM_APPLE 0
#endif

// Optional
// * Configuration
/////////////////////////////////

// Whether we should use unicode
#ifndef KOR_USE_UNICODE
	#define KOR_USE_UNICODE KOR_PLATFORM_WINDOWS
#endif

// Whether we should allow checks in build. See Check.h
#ifndef KOR_DO_CHECKS
	#define KOR_DO_CHECKS KOR_BUILD_DEBUG
#endif

// Whether we want to track SMemory allocations, See Memory.h
#ifndef KOR_TRACK_MEMORY
	#define KOR_TRACK_MEMORY KOR_BUILD_DEBUG
#endif

// Whether we want Kor to suppress default build warnings defined by platform. See <Platform>Build.h
#ifndef KOR_DEFAULT_WARNING_SUPPRESS
	#define KOR_DEFAULT_WARNING_SUPPRESS 1
#endif

// Whether we want "new" and "delete" to use Kor memory alloc. See Memory.h
#ifndef KOR_NEW_DELETE
	#define KOR_NEW_DELETE 0
#endif

// Whether library should be encapsulated in "kor" namespace
#ifndef KOR_USE_NAMESPACE
	#define KOR_USE_NAMESPACE 1
#endif

// Library
///////////////////////////////////////////////////////

#if KOR_USE_NAMESPACE
#define KOR_NAMESPACE_BEGIN namespace kor {
#define KOR_NAMESPACE_END }
#define KOR_NAMESPACE kor::
#else
#define KOR_NAMESPACE_BEGIN
#define KOR_NAMESPACE_END
#define KOR_NAMESPACE
#endif

// Platform
/////////////////////////////////

#include "Kor/_internal/BuildPlatform.h"

// Platform Extras
/////////////////////////////////

#define KOR_PTR_DIFF(Ptr1, Ptr2) static_cast<int64>(Ptr1 - Ptr2)
#define KOR_PTR_DIFF_TYPED(RetType, Ptr1, Ptr2) static_cast<RetType>(Ptr1 -Ptr2)

#define KOR_ANSITEXT(text) text
#define KOR_WIDETEXT(text) L ## text

#if KOR_USE_UNICODE
	#define KOR_TEXT(text) KOR_WIDETEXT(text)
#else
	#define KTEXT(text) KOR_ANSITEXT(text)
#endif

#define KTEXT(text) KOR_TEXT(text)

#define KOR_INDEX_NONE -1
#define KOR_CHAR_TERM '\0'
#define KOR_CHAR_NEWLINE '\n'

#define KOR_MACRO_STRINGIFY(x) #x
#define KOR_MACRO_EXPAND(x) x

#define KOR_MACRO_CONCAT(x, y) x##y
#define KOR_MACRO_CONCAT_EXPAND(x, y) KOR_MACRO_CONCAT(x,y)

#define KOR_MACRO_DOUBLE_CONCAT(x, y, z) KOR_MACRO_CONCAT_EXPAND(KOR_MACRO_CONCAT_EXPAND(x, y), z)

// Platform | Arithmetic Types
/////////////////////////////////

KOR_NAMESPACE_BEGIN

using int8 = KOR_PLATFORM_INT8;
using uint8 = KOR_PLATFORM_UINT8;
using int16 = KOR_PLATFORM_INT16;
using uint16 = KOR_PLATFORM_UINT16;
using int32 = KOR_PLATFORM_INT32;
using uint32 = KOR_PLATFORM_UINT32;
using int64 = KOR_PLATFORM_INT64;
using uint64 = KOR_PLATFORM_UINT64;
using wchar = KOR_PLATFORM_WCHAR;

#if KOR_USE_UNICODE
using tchar = wchar;
#else
using wchar = char;
#endif

KOR_NAMESPACE_END