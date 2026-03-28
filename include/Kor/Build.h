// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Build type
// * Supported: Debug, Release
// * Example: BUILD_RELEASE
/////////////////////////////////

#ifndef BUILD_RELEASE
	#define BUILD_RELEASE 0
#endif

#ifndef BUILD_DEBUG
	#define BUILD_DEBUG 0
#endif

#if BUILD_RELEASE == BUILD_DEBUG
	#error "Please specify 'BUILD_RELEASE=1' or 'BUILD_DEBUG=1'"
#endif

// Compiler
// * Supported: MSVC, GNUC, GNUC_CLANG, GNUC_INTEL, GNUC_GCC
// * Example: COMPILER_GNUC_CLANG
/////////////////////////////////

#if defined(_MSC_VER)
	#define COMPILER_MSVC 1
#elif defined(__llvm__) && defined(__clang__)
	#define COMPILER_CLANG 1
#elif defined(__INTEL_COMPILER)
	#define COMPILER_INTEL 1
#elif defined(__GNUC__)
	#define COMPILER_GCC 1
#else
	#error "Unsupported compiler"
#endif

#ifndef COMPILER_MSVC
	#define COMPILER_MSVC 0
#endif

#ifndef COMPILER_CLANG
	#define COMPILER_CLANG 0
#endif

#ifndef COMPILER_INTEL
	#define COMPILER_INTEL 0
#endif

#ifndef COMPILER_GCC
	#define COMPILER_GCC 0
#endif

// Architecture
// * Supported: 64, 32
// * Example: ARCHITECTURE_64
/////////////////////////////////

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
	#define ARCHITECTURE_64 1
#elif defined(_WIN32) || defined(__i386__)
	#define ARCHITECTURE_32 1
#else
	#error "Unsupported architecture"
#endif

#ifndef ARCHITECTURE_32
	#define ARCHITECTURE_32 0
#endif

#ifndef ARCHITECTURE_64
	#define ARCHITECTURE_64 0
#endif

// Platform
// * Supported: Windows, Linux
// * Example: PLATFORM_WINDOWS
/////////////////////////////////

#if defined(_WIN32) || defined(_WIN64)
	#define PLATFORM_WINDOWS 1
#elif defined(__linux__)
	#define PLATFORM_LINUX 1
#elif defined(__APPLE__)
	#define PLATFORM_APPLE 1
#else
	#error "Unsupported platform"
#endif

#ifndef PLATFORM_WINDOWS
	#define PLATFORM_WINDOWS 0
#endif

#ifndef PLATFORM_LINUX
	#define PLATFORM_LINUX 0
#endif

#ifndef PLATFORM_APPLE
	#define PLATFORM_APPLE 0
#endif

// Optional
// * Configuration
/////////////////////////////////

// Whether we should use unicode
#ifndef ASTD_USE_UNICODE
	#define ASTD_USE_UNICODE PLATFORM_WINDOWS
#endif

// Whether we should allow checks in build. See Check.h
#ifndef ASTD_DO_CHECKS
	#define ASTD_DO_CHECKS BUILD_DEBUG
#endif

// Whether we want to track SMemory allocations, See Memory.h
#ifndef ASTD_TRACK_MEMORY
	#define ASTD_TRACK_MEMORY BUILD_DEBUG
#endif

// Whether we want ASTD to suppress default build warnings defined by platform. See <Platform>Build.h
#ifndef ASTD_DEFAULT_WARNING_SUPPRESS
	#define ASTD_DEFAULT_WARNING_SUPPRESS 1
#endif

// Whether we want "new" and "delete" to use ASTD memory alloc. See Memory.h
#ifndef ASTD_NEW_DELETE
	#define ASTD_NEW_DELETE 0
#endif

// Platform
/////////////////////////////////

#include "ASTD/_internal/BuildPlatform.h"

// Other
// * Post platform types/forwards and helpers
/////////////////////////////////

#define PTR_DIFF(Ptr1, Ptr2) static_cast<int64>(Ptr1 - Ptr2)
#define PTR_DIFF_TYPED(RetType, Ptr1, Ptr2) static_cast<RetType>(Ptr1 -Ptr2)

#ifdef TEXT
	#undef TEXT
#endif

#define ANSITEXT(text) text
#define WIDETEXT(text) L ## text

#if ASTD_USE_UNICODE
	#define TEXT(text) WIDETEXT(text)
	typedef wchar tchar;
#else
	#define TEXT(text) ANSITEXT(text)
	typedef char tchar;
#endif

#define INDEX_NONE -1
#define CHAR_TERM '\0'
#define CHAR_SLASH '/'
#define CHAR_NEWLINE '\n'

#define STRINGIFY(x) #x
#define EXPAND(x) x

#define CONCAT(x, y) x##y
#define CONCAT_EXPAND(x, y) CONCAT(x,y)

#define DOUBLE_CONCAT(x, y, z) CONCAT_EXPAND(CONCAT_EXPAND(x, y), z)