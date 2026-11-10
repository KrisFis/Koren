// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

// Platform Primitives
// -------------------------------------------------------------------------
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
// -------------------------------------------------------------------------

#if KOR_PLATFORM_WINDOWS
	#define KOR_PLATFORM_NAME Win32
#elif KOR_PLATFORM_LINUX
	#define KOR_PLATFORM_NAME Linux
#elif KOR_PLATFORM_APPLE
	#define KOR_PLATFORM_NAME Apple
#elif KOR_PLATFORM_UNIX
	#define KOR_PLATFORM_NAME Unix
#else
	#error "Unsupported platform"
#endif

#define KOR_PLATFORM_HEADER(name) KOR_PP_STRINGIFY_EXPAND(KOR_PLATFORM_NAME/KOR_PP_CONCAT_EXPAND(KOR_PLATFORM_NAME, name.h))
#define KOR_PLATFORM_HEADER_FROM(root, name) KOR_PP_STRINGIFY_EXPAND(root/KOR_PLATFORM_NAME/KOR_PP_CONCAT_EXPAND(KOR_PLATFORM_NAME, name.h))

#define KOR_PLATFORM_TYPE(name) KOR_PP_CONCAT(KOR_PLATFORM_NAME, name)
#define KOR_PLATFORM_TYPE_CUSTOM(prefix, name) KOR_PP_CONCAT3(prefix, KOR_PLATFORM_NAME, name)

#define KOR_PLATFORM_STRUCT(name) KOR_PLATFORM_TYPE_CUSTOM(S, name)
#define KOR_PLATFORM_CLASS(name) KOR_PLATFORM_TYPE_CUSTOM(C, name)
#define KOR_PLATFORM_ENUM(name) KOR_PLATFORM_TYPE_CUSTOM(E, name)
#define KOR_PLATFORM_TEMPLATE(name) KOR_PLATFORM_TYPE_CUSTOM(T, name)
#define KOR_PLATFORM_NAMESPACE(name) KOR_PLATFORM_TYPE_CUSTOM(N, name)
#define KOR_PLATFORM_FUNC(name) KOR_PLATFORM_TYPE_CUSTOM(F, name)

// Main platform include
// -------------------------------------------------------------------------

#include KOR_PLATFORM_HEADER_FROM(Kor/Core, Platform)

// Other macros
// -------------------------------------------------------------------------

#ifndef KOR_WCHAR_BYTES
	#error "Platform must define size of wide character"
#endif

#ifdef __cpp_char8_t
	#define KOR_CHAR8_NATIVE 1
#else
	#define KOR_CHAR8_IS_UTF8 0
#endif

#ifdef KOR_CHAR8_NATIVE
	#define KOR_TEXT_UTF8(text) u8 ## text
#else
	#define KOR_TEXT_UTF8(text) text
#endif

#define KOR_TEXT_UTF16(text) u ## text
#define KOR_TEXT_UTF32(text) U ## text
#define KOR_TEXT_WIDE(text) L ## text
#define KOR_TEXT_ANSI(text) text

#if KOR_USE_UNICODE
	#define KOR_TEXT(text) KOR_TEXT_WIDE(text)
#else
	#define KOR_TEXT(text) KOR_TEXT_ANSI(text)
#endif

#define KTEXT(text) KOR_TEXT(text)

// Sentinel and character constants.
#define KOR_INDEX_NONE -1

// Pointer arithmetic helpers

// Byte diff
#define KOR_PTR_DIFF(RetType, Ptr1, Ptr2) (RetType)((uintptr)Ptr1 - (uintptr)Ptr2)

// Typed diff
#define KOR_PTR_TYPED_DIFF(RetType, Ptr1, Ptr2) (RetType)(Ptr1 - Ptr2)

// Default heap alignment
// * Heap Alignment guarantees alignment sufficient for the strictest fundamental type
// * Strictest type is typically 2 pointer-widths
#if KOR_ARCHITECTURE_64
	#define KOR_DEFAULT_HEAP_ALIGNMENT 16
#else
	#define KOR_DEFAULT_HEAP_ALIGNMENT 8
#endif

// Types
// -------------------------------------------------------------------------

KOR_NAMESPACE_BEGIN

typedef KOR_PLATFORM_STRUCT(Types) SPlatformTypes;

// An 8-bit unsigned integer.
using int8 = SPlatformTypes::Int8;
// An 8-bit signed integer.
using uint8 = SPlatformTypes::Uint8;

// A 16-bit unsigned integer.
using uint16 = SPlatformTypes::Uint16;
// A 16-bit signed integer.
using int16 = SPlatformTypes::Int16;

// A 32-bit unsigned integer.
using uint32 = SPlatformTypes::Uint32;
// A 32-bit signed integer.
using int32 = SPlatformTypes::Int32;

// A 64-bit unsigned integer.
using uint64 = SPlatformTypes::Uint64;
// A 64-bit signed integer.
using int64 = SPlatformTypes::Int64;

// Pointer type
using intptr = SPlatformTypes::IntPtr;

// Unsigned pointer type
using uintptr = SPlatformTypes::UIntPtr;

// An ANSI character
using achar = SPlatformTypes::Achar;
// A wide character. Platform specific width (SEE KOR_WCHAR_BYTES)
using wchar = SPlatformTypes::Wchar;

// An 8-bit character containing a UTF8 (variable-width) code unit.
using char8 = SPlatformTypes::Char8;
// A 16-bit character containing a UTF16 (variable-width) code unit.
using char16 = SPlatformTypes::Char16;
// A 32-bit character containing a UTF32 (fixed-width) code unit.
using char32 = SPlatformTypes::Char32;

// Either achar or wchar
#if KOR_USE_UNICODE
typedef wchar tchar;
#else
typedef char tchar;
#endif

#define KOR_BUFFER_SIZE_SMALL (1024)
#define KOR_BUFFER_SIZE_LARGE (4096)

#if KOR_ALLOW_STATIC_TESTS
namespace Detail
{
	static_assert(sizeof(uint8) == 1, "uint8 type size test failed.");
	static_assert(sizeof(uint16) == 2, "uint16 type size test failed.");
	static_assert(sizeof(uint32) == 4, "uint32 type size test failed.");
	static_assert(sizeof(uint64) == 8, "uint64 type size test failed.");

	static_assert(uint8(-1) > uint8(0), "uint8 type sign test failed.");
	static_assert(uint16(-1) > uint16(0), "uint16 type sign test failed.");
	static_assert(uint32(-1) > uint32(0), "uint32 type sign test failed.");
	static_assert(uint64(-1) > uint64(0), "uint64 type sign test failed.");

	static_assert(sizeof(achar) == 1, "achar type size test failed.");
	static_assert(int32(achar(-1)) == -1, "achar type sign test failed.");

	static_assert(sizeof(wchar) == 2 || sizeof(wchar) == 4, "wchar type size test failed.");

	static_assert(sizeof(char8)  == 1, "char8 type size test failed.");
	static_assert(sizeof(char16) == 2, "char16 type size test failed.");
	static_assert(sizeof(char32) == 4, "char32 type size test failed.");

	static_assert(sizeof(wchar) == KOR_WCHAR_BYTES, "wchar type size test failed.");
}
#endif

KOR_NAMESPACE_END