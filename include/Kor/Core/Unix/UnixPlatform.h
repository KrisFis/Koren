// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

#include <stdint.h>

// Strings
////////////////////////////////////////////////

#define KOR_WCHAR_BYTES 4

// Types
////////////////////////////////////////////////

KOR_NAMESPACE_BEGIN

struct SUnixTypes
{
	typedef int8_t Int8;
	typedef uint8_t Uint8;

	typedef int16_t Int16;
	typedef uint16_t Uint16;

	typedef int32_t Int32;
	typedef uint32_t Uint32;

	typedef int64_t Int64;
	typedef uint64_t Uint64;

	typedef char Achar;
	typedef wchar_t Wchar;

	typedef char8_t Char8;
	typedef char16_t Char16;
	typedef char32_t Char32;
};

KOR_NAMESPACE_END