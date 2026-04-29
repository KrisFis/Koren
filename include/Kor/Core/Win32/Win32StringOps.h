// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"

#include <stdio.h>
#include <wchar.h>

KOR_NAMESPACE_BEGIN

template<typename CharType>
struct TWin32StringOps
{
	static_assert(TIsSame<CharType, achar>::Value || TIsSame<CharType, wchar>::Value,
		"Win32 string operations only work on ANSI and WCHAR types.");

	template<typename... ArgT>
	KOR_FORCEINLINE int32 static Snprintf(CharType* str, const CharType* fmt, TSize len, const ArgT&... args) noexcept
	{
		if constexpr (TIsSame<CharType, achar>::Value)
		{
			return _snprintf(str, len, fmt, args...);
		}
		else
		{
			return _snwprintf(str, len, fmt, args...);
		}
	}

	KOR_FORCEINLINE static double Strtod(const CharType* str, const CharType** end) noexcept
	{
		if constexpr (TIsSame<CharType, achar>::Value)
		{
			return strtod(str, const_cast<CharType**>(end)); // Weird quirk of C++
		}
		else
		{
			return wcstod(str, const_cast<CharType**>(end)); // Weird quirk of C++
		}
	}
};

KOR_NAMESPACE_END