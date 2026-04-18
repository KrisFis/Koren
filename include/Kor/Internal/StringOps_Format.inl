// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
template<typename ... ArgT>
KOR_FORCEINLINE int32 TStringOps<CharType>::Format(CharType* str, const CharType* fmt, ArgT&&... args) noexcept
{
	if constexpr (sizeof(CharType) > 1)
	{
		return swprintf(
			reinterpret_cast<wchar_t*>(str),
			reinterpret_cast<const wchar_t*>(fmt),
			Forward<ArgT>(args)...
		);
	}
	else
	{
		return sprintf(
			reinterpret_cast<achar*>(str),
			reinterpret_cast<const achar*>(fmt),
			Forward<ArgT>(args)...
		);
	}
}

template<typename CharType>
template<typename ... ArgT>
KOR_FORCEINLINE int32 TStringOps<CharType>::Format(CharType* str, const CharType* fmt, int32 len, ArgT&&... args) noexcept
{
	if constexpr (sizeof(CharType) > 1)
	{
		return swprintf(
			reinterpret_cast<wchar_t*>(str),
			len,
			reinterpret_cast<const wchar_t*>(fmt),
			Forward<ArgT>(args)...
		);
	}
	else
	{
		return sprintf(
			reinterpret_cast<achar*>(str),
			len,
			reinterpret_cast<const achar*>(fmt),
			Forward<ArgT>(args)...
		);
	}
}

template<typename CharType>
template<int32 N, typename ... ArgT>
KOR_FORCEINLINE int32 TStringOps<CharType>::Format(CharType(& str)[N], const CharType* fmt, ArgT&&... args) noexcept
{
	return Format(str, N, fmt, Forward<ArgT>(args)... );
}