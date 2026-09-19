// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

// Build Type
// -------------------------------------------------------------------------
// Defines the active build mode. Exactly one must be set to 1.
// Set via compiler flags: -DKOR_BUILD_DEBUG=1 or -DKOR_BUILD_RELEASE=1
//
// Supported: Debug, Release
// Example: KOR_BUILD_RELEASE
// -------------------------------------------------------------------------

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
// -------------------------------------------------------------------------
// Detected automatically from predefined compiler macros.
//
// Supported: MSVC, CLANG, GCC
// Example: KOR_COMPILER_CLANG
// -------------------------------------------------------------------------

#if defined(_MSC_VER)
	#define KOR_COMPILER_MSVC 1
#elif defined(__clang__)
	#define KOR_COMPILER_CLANG 1
#elif defined(__GNUC__)
	#define KOR_COMPILER_GCC 1
#else
	#error "Unsupported compiler"
#endif

#ifndef KOR_COMPILER_MSVC
	#define KOR_COMPILER_MSVC 0
#endif

#ifndef KOR_COMPILER_CLANG
	#define KOR_COMPILER_CLANG 0
#endif

#ifndef KOR_COMPILER_GCC
	#define KOR_COMPILER_GCC 0
#endif

// Architecture
// -------------------------------------------------------------------------
// Detected automatically. Exactly one will be set to 1.
//
// Supported: 32, 64
// Example: KOR_ARCHITECTURE_64
// -------------------------------------------------------------------------

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

// CPP Version
// -------------------------------------------------------------------------

// cpp11 - Published 2011-03
#define KOR_CPP_11 (__cplusplus >= 201103L)
// cpp14 - Published 2014-02
#define KOR_CPP_14 (__cplusplus >= 201402L)
// cpp17 - Published 2017-03
#define KOR_CPP_17 (__cplusplus >= 201703L)
// cpp20 - Published 2020-02
#define KOR_CPP_20 (__cplusplus >= 202002L)
// cpp23 - Published 2023-02
#define KOR_CPP_23 (__cplusplus >= 202302L)

// Platform
// -------------------------------------------------------------------------
// Detected automatically
//
// Supported: Windows, Linux, Apple, Unix
// Example: KOR_PLATFORM_WINDOWS
// -------------------------------------------------------------------------

#if defined(_WIN32) || defined(_WIN64)
	#define KOR_PLATFORM_WINDOWS 1
#elif defined(__linux__)
	#define KOR_PLATFORM_LINUX 1
	#define KOR_PLATFORM_UNIX 1
#elif defined(__APPLE__)
	#define KOR_PLATFORM_APPLE 1
	#define KOR_PLATFORM_UNIX 1
#elif defined(__unix__) || defined(__unix)
	#define KOR_PLATFORM_UNIX 1
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

#ifndef KOR_PLATFORM_UNIX
	#define KOR_PLATFORM_UNIX 0
#endif

// Configuration
// -------------------------------------------------------------------------
// Optional overrides.
// Define before including this header to change defaults.
// All default to safe/conservative values.
// -------------------------------------------------------------------------

// Whether to use wide characters (wchar) as the default char type (tchar).
// Defaults to 1 on Windows, 0 elsewhere. See: Platform.h
#ifndef KOR_USE_UNICODE
	#define KOR_USE_UNICODE KOR_PLATFORM_WINDOWS
#endif

// Whether asserts are enabled. Defaults to debug builds only. See: Assert.h
#ifndef KOR_USE_ASSERT
	#define KOR_USE_ASSERT KOR_BUILD_DEBUG
#endif

// Whether memory allocations are tracked. Defaults to debug builds only. See: Memory.h
#ifndef KOR_TRACK_MEMORY
	#define KOR_TRACK_MEMORY KOR_BUILD_DEBUG
#endif

// Whether platform-specific default warnings are suppressed. See: <Platform>Build.h
#ifndef KOR_DEFAULT_WARNING_SUPPRESS
	#define KOR_DEFAULT_WARNING_SUPPRESS 1
#endif

// Whether the library is wrapped in the "Kor" namespace.
// Disable only if namespace collisions require it.
#ifndef KOR_USE_NAMESPACE
	#define KOR_USE_NAMESPACE 1
#endif

// Namespace
// -------------------------------------------------------------------------
// Wraps library declarations in "Kor" when KOR_USE_NAMESPACE=1.
// Use KOR_NAMESPACE_BEGIN / KOR_NAMESPACE_END around declarations,
// and KOR_NAMESPACE::as a prefix in definitions.
// -------------------------------------------------------------------------

#if KOR_USE_NAMESPACE
#define KOR_NAMESPACE Kor
#define KOR_NAMESPACE_BEGIN namespace KOR_NAMESPACE {
#define KOR_NAMESPACE_END }
#else
#define KOR_NAMESPACE
#define KOR_NAMESPACE_BEGIN
#define KOR_NAMESPACE_END
#endif

// Macros
// -------------------------------------------------------------------------
// General-purpose utility macros. No platform or config dependencies.
// -------------------------------------------------------------------------

// Forces an extra rescan pass; needed to work around MSVC's legacy __VA_ARGS__ expansion bug.
#define KOR_PP_EXPAND(x) x

// Turns x into a string literal (optionally expands first)
#define KOR_PP_STRINGIFY(x) #x
#define KOR_PP_STRINGIFY_EXPAND(x) KOR_PP_STRINGIFY(x)

// Pastes x and y into one token (optionally expands first)
#define KOR_PP_CONCAT(x, y) x##y
#define KOR_PP_CONCAT_EXPAND(x, y) KOR_PP_CONCAT(x,y)

// Pastes three tokens together (optionally expands first)
#define KOR_PP_CONCAT3(x, y, z) KOR_PP_CONCAT_EXPAND(KOR_PP_CONCAT_EXPAND(x, y), z)
#define KOR_PP_CONCAT3_EXPAND(x, y, z) KOR_PP_CONCAT3(x, y, z)

// Appends a unique per-expansion suffix to prefix (per-TU counter if available, else line number).
#if defined(__COUNTER__)
	#define KOR_PP_UNIQUE_NAME(prefix) KOR_PP_CONCAT_EXPAND(prefix, __COUNTER__)
#else
	#define KOR_PP_UNIQUE_NAME(prefix) KOR_PP_CONCAT_EXPAND(prefix, __LINE__)
#endif

// NARG \ ARG_NUM
// Gets number of arguments passed to it
// **IMPORTANT**: Number of args should be within 1 and 64
// * Discovered by Jens Gustedt / Laurent Deniau
// -------------------------------------------------------------------------

#define KOR_PP_ARG_NUM(...) KOR_PP_NARG_IMPL(__VA_ARGS__, KOR_PP_RSEQ_N())
#define KOR_PP_NARG(...) KOR_PP_NARG_IMPL(__VA_ARGS__, KOR_PP_RSEQ_N())

// Re-splits __VA_ARGS__ before selection, working around MSVC's legacy expansion bug.
#define KOR_PP_NARG_IMPL(...) KOR_PP_ARG_N_IMPL(__VA_ARGS__)

#define KOR_PP_ARG_N_IMPL( \
		_1, _2, _3, _4, _5, _6, _7, _8, _9,_10,	\
		_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
		_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,\
		_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
		_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,\
		_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,\
		_61,_62,_63,N,...) N

#define KOR_PP_RSEQ_N_IMPL() \
		63,62,61,60,					\
		59,58,57,56,55,54,53,52,51,50,	\
		49,48,47,46,45,44,43,42,41,40,	\
		39,38,37,36,35,34,33,32,31,30,	\
		29,28,27,26,25,24,23,22,21,20,	\
		19,18,17,16,15,14,13,12,11,10,	\
		9,8,7,6,5,4,3,2,1,0