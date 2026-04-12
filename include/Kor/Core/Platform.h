// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

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
#elif KOR_PLATFORM_UNIX
	#define KOR_PLATFORM_NAME Unix
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

// Main platform include
///////////////////////////////////////////////////////////////

#include KOR_PLATFORM_HEADER_FROM(Kor/Core, Platform)

// Type macros
///////////////////////////////////////////////////////////////

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
#define KOR_PTR_DIFF(RetType, Ptr1, Ptr2) static_cast<RetType>(Ptr1 -Ptr2)

KOR_NAMESPACE_BEGIN

typedef KOR_PLATFORM_STRUCT(Types) SPlatformTypes;

// Integer types
///////////////////////////////////////////////////////////////

// An 8-bit unsigned integer.
typedef SPlatformTypes::Int8	int8;
// An 8-bit signed integer.
typedef SPlatformTypes::Uint8	uint8;

// A 16-bit unsigned integer.
typedef SPlatformTypes::Uint16	uint16;
// A 16-bit signed integer.
typedef SPlatformTypes::Int16	int16;

// A 32-bit unsigned integer.
typedef SPlatformTypes::Uint32	uint32;
// A 32-bit signed integer.
typedef SPlatformTypes::Int32	int32;

// A 64-bit unsigned integer.
typedef SPlatformTypes::Uint64	uint64;
// A 64-bit signed integer.
typedef SPlatformTypes::Int64	int64;

// Character types
///////////////////////////////////////////////////////////////

// An ANSI character
typedef SPlatformTypes::Achar	achar;
// A wide character. Platform specific width (SEE KOR_WCHAR_BYTES)
typedef SPlatformTypes::Wchar	wchar;

// An 8-bit character containing a UTF8 (variable-width) code unit.
typedef SPlatformTypes::Char8	char8;
// A 16-bit character containing a UTF16 (variable-width) code unit.
typedef SPlatformTypes::Char16	char16;
// A 32-bit character containing a UTF32 (fixed-width) code unit.
typedef SPlatformTypes::Char32	char32;

// Either achar or wchar
#if KOR_USE_UNICODE
typedef wchar tchar;
#else
typedef char tchar;
#endif

namespace Internal::TypeTests
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

KOR_NAMESPACE_END