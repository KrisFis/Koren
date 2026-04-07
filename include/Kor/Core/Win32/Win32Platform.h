// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

// Windows setup
////////////////////////////////////////////////////////////////////////

#if KOR_USE_UNICODE && !defined(UNICODE)
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

// Strings
////////////////////////////////////////////////

#define KOR_WCHAR_BYTES 2

// Types
////////////////////////////////////////////////

KOR_NAMESPACE_BEGIN

struct SWin32Types
{
	typedef __int8 Int8;
	typedef unsigned __int8 Uint8;

	typedef __int16 Int16;
	typedef unsigned __int16 Uint16;

	typedef __int32 Int32;
	typedef unsigned __int32 Uint32;

	typedef __int64 Int64;
	typedef unsigned __int64 Uint64;

	typedef char Achar;
	typedef wchar_t Wchar;

	typedef char8_t Char8;
	typedef char16_t Char16;
	typedef char32_t Char32;
};

KOR_NAMESPACE_END