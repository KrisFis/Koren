// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"

KOR_NAMESPACE_BEGIN

template<typename CharType>
struct TUnixStringOps
{
	static_assert(TIsSame<CharType, achar>::Value || TIsSame<CharType, wchar>::Value,
		"Unix string operations only work on ANSI and WCHAR types.");

	template<typename... ArgT>
	KOR_FORCEINLINE static int32 Snprintf(achar* str, const CharType* fmt, TSize len, const ArgT&... args) noexcept
	{
		if constexpr (TIsSame<CharType, achar>::Value)
		{
			return snprintf(str, len, fmt, args...);
		}
		else
		{
			return swprintf(str, len, fmt, args...);
		}
	}
};

KOR_NAMESPACE_END

