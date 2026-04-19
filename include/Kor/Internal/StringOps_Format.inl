// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
template<typename... ArgT>
KOR_FORCEINLINE int32 TStringOps<CharType>::Format(CharType* str, const CharType* fmt, int32 len, const ArgT&... args) noexcept
{
	if constexpr (TIsSame<CharType, achar>::Value)
	{
		return Internal::Format(str, fmt, len, args...);
	}
	else if constexpr (TIsSame<CharType, wchar>::Value)
	{
		return Internal::Format(str, fmt, len, args...);
	}
	else if constexpr (TIsSame<CharType, char8>::Value)
	{
		return Internal::Format(reinterpret_cast<achar*>(str), reinterpret_cast<const achar*>(fmt), len, args...);
	}
	else
	{
		static_assert(sizeof(CharType) == 0, "Format not supported for this character type");
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
template<int32 N, typename ... ArgT>
KOR_FORCEINLINE int32 TStringOps<CharType>::Format(CharType(&str)[N], const CharType* fmt, const ArgT&... args) noexcept
{
	return Format(static_cast<CharType*>(str), fmt, N, args... );
}
