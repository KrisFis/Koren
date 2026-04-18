// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
template<typename... ArgT>
KOR_FORCEINLINE int32 TStringOps<CharType>::Format(CharType* str, const CharType* fmt, const ArgT&... args) noexcept
{
	if constexpr (sizeof(CharType) > 1)
	{
		return 0;

		// TODO: Add support for wide printf
		// return swprintf(
		// 	reinterpret_cast<wchar_t*>(str),
		// 	SCString::LARGE_BUFFER_SIZE,
		// 	reinterpret_cast<const wchar_t*>(fmt),
		// 	args...
		// );
	}
	else
	{
		return sprintf(
			reinterpret_cast<achar*>(str),
			reinterpret_cast<const achar*>(fmt),
			args...
		);
	}
}

template<typename CharType>
template<typename... ArgT>
KOR_FORCEINLINE int32 TStringOps<CharType>::Format(CharType* str, const CharType* fmt, int32 len, const ArgT&... args) noexcept
{
	if constexpr (sizeof(CharType) > 1)
	{
		return 0;
		// TODO: Add support for wide printf
		// return swprintf(
		// 	reinterpret_cast<wchar_t*>(str),
		// 	len,
		// 	reinterpret_cast<const wchar_t*>(fmt),
		// 	args...
		// );
	}
	else
	{
		return snprintf(
			reinterpret_cast<achar*>(str),
			len,
			reinterpret_cast<const achar*>(fmt),
			args...
		);
	}
}

template<typename CharType>
template<int32 N, typename ... ArgT>
KOR_FORCEINLINE int32 TStringOps<CharType>::Format(CharType(& str)[N], const CharType* fmt, const ArgT&... args) noexcept
{
	return Format(str, N, fmt, args... );
}