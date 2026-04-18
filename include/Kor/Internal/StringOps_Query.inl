// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
bool TStringOps<CharType>::IsAscii(const CharType* str) noexcept
{
	if constexpr (!TIsSame<CharType, achar>::Value)
	{
		while(*str)
		{
			if (!CharOps::IsAscii(*(str++))) return false;
		}
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsAscii(const CharType* str, int32 len) noexcept
{
	if constexpr (!TIsSame<CharType, achar>::Value)
	{
		while(len-- > 0)
		{
			if (!CharOps::IsAscii(*(str++))) return false;
		}
	}

	return true;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE bool TStringOps<CharType>::IsAscii(const CharType(& str)[N]) noexcept
{
	return IsAscii(str, N);
}

template<typename CharType>
bool TStringOps<CharType>::IsNumeric(const CharType* str) noexcept
{
	while(*str)
	{
		if (!CharOps::IsDigit(*(str++))) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsNumeric(const CharType* str, int32 len) noexcept
{
	while(len-- > 0)
	{
		if (!CharOps::IsDigit(*(str++))) return false;
	}

	return true;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE bool TStringOps<CharType>::IsNumeric(const CharType(& str)[N]) noexcept
{
	return IsNumeric(str, N);
}

template<typename CharType>
bool TStringOps<CharType>::IsUpper(const CharType* str) noexcept
{
	while(*str)
	{
		if (!CharOps::IsUpper(*(str++))) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsUpper(const CharType* str, int32 len) noexcept
{
	while(len-- > 0)
	{
		if (!CharOps::IsUpper(*(str++))) return false;
	}

	return true;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE bool TStringOps<CharType>::IsUpper(const CharType(& str)[N]) noexcept
{
	return IsUpper(str, N);
}

template<typename CharType>
bool TStringOps<CharType>::IsLower(const CharType* str) noexcept
{
	while(*str)
	{
		if (!CharOps::IsLower(*(str++))) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsLower(const CharType* str, int32 len) noexcept
{
	while(len-- > 0)
	{
		if (!CharOps::IsLower(*(str++))) return false;
	}

	return true;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE bool TStringOps<CharType>::IsLower(const CharType(& str)[N]) noexcept
{
	return IsLower(str, N);
}