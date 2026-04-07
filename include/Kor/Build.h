// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

// Build Type
///////////////////////////////////////////////////////////////
//
// Defines the active build mode. Exactly one must be set to 1.
// Set via compiler flags: -DKOR_BUILD_DEBUG=1 or -DKOR_BUILD_RELEASE=1
//
// Supported: Debug, Release
// Example:   KOR_BUILD_RELEASE
///////////////////////////////////////////////////////////////

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
///////////////////////////////////////////////////////////////
// Detected automatically from predefined compiler macros.
// GNUC is set alongside CLANG or GCC (they are not mutually exclusive).
//
// Supported: MSVC, GNUC, CLANG, GCC
// Example:   KOR_COMPILER_CLANG
///////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
	#define KOR_COMPILER_MSVC 1
#elif defined(__GNUC__)
	#define KOR_COMPILER_GNUC 1
	#if defined(__clang__)
		#define KOR_COMPILER_CLANG 1
	#else
		#define KOR_COMPILER_GCC 1
	#endif
#else
	#error "Unsupported compiler"
#endif

#ifndef KOR_COMPILER_MSVC
	#define KOR_COMPILER_MSVC 0
#endif

#ifndef KOR_COMPILER_GNUC
	#define KOR_COMPILER_GNUC 0
#endif

#ifndef KOR_COMPILER_CLANG
	#define KOR_COMPILER_CLANG 0
#endif

#ifndef KOR_COMPILER_GCC
	#define KOR_COMPILER_GCC 0
#endif

// Architecture
///////////////////////////////////////////////////////////////
// Detected automatically. Exactly one will be set to 1.
//
// Supported: 32, 64
// Example:   KOR_ARCHITECTURE_64
///////////////////////////////////////////////////////////////

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
///////////////////////////////////////////////////////////////
// Detected automatically. Exactly one will be set to 1.
//
// Supported: Windows, Linux, Apple
// Example:   KOR_PLATFORM_WINDOWS
///////////////////////////////////////////////////////////////

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

// Configuration
///////////////////////////////////////////////////////////////
// Optional overrides.
// Define before including this header to change defaults.
// All default to safe/conservative values.
///////////////////////////////////////////////////////////////

// Whether to use wide characters (wchar) as the default char type (tchar).
// Defaults to 1 on Windows, 0 elsewhere. See: Types section below.
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

// Whether global "new" and "delete" are redirected to Kor allocators. See: Memory.h
#ifndef KOR_NEW_DELETE
	#define KOR_NEW_DELETE 0
#endif

// Whether the library is wrapped in the "Kor" namespace.
// Disable only if namespace collisions require it.
#ifndef KOR_USE_NAMESPACE
	#define KOR_USE_NAMESPACE 1
#endif

// Namespace
///////////////////////////////////////////////////////////////
// Wraps library declarations in "Kor::" when KOR_USE_NAMESPACE=1.
// Use KOR_NAMESPACE_BEGIN / KOR_NAMESPACE_END around declarations,
// and KOR_NAMESPACE as a prefix in definitions.
///////////////////////////////////////////////////////////////

#if KOR_USE_NAMESPACE
#define KOR_NAMESPACE_BEGIN namespace Kor {
#define KOR_NAMESPACE_END }
#define KOR_NAMESPACE Kor::
#else
#define KOR_NAMESPACE_BEGIN
#define KOR_NAMESPACE_END
#define KOR_NAMESPACE
#endif

// Macros
///////////////////////////////////////////////////////////////
// General-purpose utility macros. No platform or config dependencies.
///////////////////////////////////////////////////////////////

// Macro utilities - expand, stringify, and concatenate with proper expansion order.
#define KOR_MACRO_EXPAND(x) x

#define KOR_MACRO_STRINGIFY(x) #x
#define KOR_MACRO_STRINGIFY_EXPAND(x) KOR_MACRO_STRINGIFY(x)

#define KOR_MACRO_CONCAT(x, y) x##y
#define KOR_MACRO_CONCAT_EXPAND(x, y) KOR_MACRO_CONCAT(x,y)

#define KOR_MACRO_DOUBLE_CONCAT(x, y, z) KOR_MACRO_CONCAT_EXPAND(KOR_MACRO_CONCAT_EXPAND(x, y), z)
#define KOR_MACRO_DOUBLE_CONCAT_EXPAND(x, y, z) KOR_MACRO_DOUBLE_CONCAT(x, y, z)

// Platform Primitives
///////////////////////////////////////////////////////////////
// Helpers for resolving platform-specific headers and type names.
// KOR_PLATFORM_NAME expands to the active platform identifier (e.g. Win32).
//
// Header resolution:
//   KOR_PLATFORM_HEADER(name)            - relative to current include root
//   KOR_PLATFORM_HEADER_FROM(root, name) - relative to explicit root
//
// Type name construction:
//   KOR_PLATFORM_TYPE(Name)                - e.g. Win32Name           (no prefix)
//   KOR_PLATFORM_TYPE_CUSTOM(Prefix, Name) - e.g. PrefixWin32Name     (custom prefix)
//   KOR_PLATFORM_STRUCT(Name)              - e.g. SWin32Name          (structs)
//   KOR_PLATFORM_CLASS(Name)               - e.g. CWin32Name          (classes)
//   KOR_PLATFORM_TEMPLATE(Name)            - e.g. TWin32Name          (templates)
//   KOR_PLATFORM_NAMESPACE(Name)           - e.g. NWin32Name          (namespaces)
//   KOR_PLATFORM_FUNC(Name)                - e.g. FWin32Name          (free functions)
///////////////////////////////////////////////////////////////

#if KOR_PLATFORM_WINDOWS
	#define KOR_PLATFORM_NAME Win32
#elif KOR_PLATFORM_LINUX
	#define KOR_PLATFORM_NAME Linux
#elif KOR_PLATFORM_APPLE
	#define KOR_PLATFORM_NAME Apple
#else
	#error "Unsupported platform"
#endif

#define KOR_PLATFORM_HEADER(name) KOR_MACRO_STRINGIFY_EXPAND(KOR_PLATFORM_NAME/KOR_MACRO_CONCAT_EXPAND(KOR_PLATFORM_NAME, name.h))
#define KOR_PLATFORM_HEADER_FROM(root, name) KOR_MACRO_STRINGIFY_EXPAND(root/KOR_PLATFORM_NAME/KOR_MACRO_CONCAT_EXPAND(KOR_PLATFORM_NAME, name.h))

#define KOR_PLATFORM_TYPE(name) KOR_MACRO_CONCAT(KOR_PLATFORM_NAME, name)
#define KOR_PLATFORM_TYPE_CUSTOM(prefix, name) KOR_MACRO_DOUBLE_CONCAT(prefix, KOR_PLATFORM_NAME, name)

#define KOR_PLATFORM_STRUCT(name) KOR_PLATFORM_TYPE_CUSTOM(S, name)
#define KOR_PLATFORM_CLASS(name) KOR_PLATFORM_TYPE_CUSTOM(C, name)
#define KOR_PLATFORM_ENUM(name) KOR_PLATFORM_TYPE_CUSTOM(E, name)
#define KOR_PLATFORM_TEMPLATE(name) KOR_PLATFORM_TYPE_CUSTOM(T, name)
#define KOR_PLATFORM_NAMESPACE(name) KOR_PLATFORM_TYPE_CUSTOM(N, name)
#define KOR_PLATFORM_FUNC(name) KOR_PLATFORM_TYPE_CUSTOM(F, name)