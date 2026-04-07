// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Build.h"
#include KOR_PLATFORM_HEADER_FROM(Kor, Platform)

KOR_NAMESPACE_BEGIN

using SPlatformTypes = KOR_PLATFORM_STRUCT(Types);

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

/// An ANSI character
typedef SPlatformTypes::Achar	achar;
/// A wide character. Platform specific width (SEE KOR_WCHAR_IS_UTF16)
typedef SPlatformTypes::Wchar	wchar;

/// An 8-bit character containing a UTF8 (variable-width) code unit.
typedef SPlatformTypes::Char8	char8;
/// A 16-bit character containing a UTF16 (variable-width) code unit.
typedef SPlatformTypes::Char16	char16;
/// A 32-bit character containing a UTF32 (fixed-width) code unit.
typedef SPlatformTypes::Char32	char32;

/// Either ANSICHAR or WIDECHAR
#if KOR_USE_UNICODE
typedef wchar tchar;
#else
typedef char tchar;
#endif

// Type macros
///////////////////////////////////////////////////////////////

// Sentinel and character constants.
#define KOR_INDEX_NONE -1

// Pointer arithmetic helpers
#define KOR_PTR_DIFF(RetType, Ptr1, Ptr2) static_cast<RetType>(Ptr1 -Ptr2)

// Prefer KTEXT
#define KOR_TEXT_WIDE(text) L ## text
#define KOR_TEXT_ANSI(text) text

#if KOR_USE_UNICODE
#define KOR_TEXT(text) KOR_TEXT_WIDE(text)
#define KOR_CHAR_NEWLINE KOR_CHAR_NEWLINE_WIDE
#else
#define KOR_TEXT(text) KOR_TEXT_ANSI(text)
#define KOR_CHAR_NEWLINE KOR_CHAR_NEWLINE_ANSI
#endif

#define KTEXT(text) KOR_TEXT(text)

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