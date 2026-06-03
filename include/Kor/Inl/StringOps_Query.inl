// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
int32 TStringOps<CharType>::Length(const CharType* str) noexcept
{
	int32 len = 0; while (*str++) ++len; return len;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::Length(const CharType(&)[N]) noexcept
{
	return N;
}

template<typename CharType>
template<ESearchDir Dir>
int32 TStringOps<CharType>::CountWhitespaces(const CharType* str) noexcept
{
	int32 result = 0;

	if constexpr (Dir == ESearchDir::Forward)
	{
		while (*str)
		{
			if (!CharOps::IsWhitespace(*str++)) break;
			++result;
		}
	}
	else
	{
		while (*str)
		{
			if (CharOps::IsWhitespace(*str++))
			{
				++result;
			}
			else
			{
				result = 0;
			}
		}
	}

	return result;
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::CountWhitespaces(const CharType* str, ESearchDir searchDir) noexcept
{
	return searchDir == ESearchDir::Forward 
		? CountWhitespaces<ESearchDir::Forward>(str)
		: CountWhitespaces<ESearchDir::Backward>(str);
}

template<typename CharType>
template<ESearchDir Dir>
int32 TStringOps<CharType>::CountWhitespaces(const CharType* str, int32 strLen) noexcept
{
	int32 result = 0;

	if constexpr (Dir == ESearchDir::Forward)
	{
		while (strLen-- > 0)
		{
			if (!CharOps::IsWhitespace(*str++)) break;
			++result;
		}
	}
	else
	{
		str += strLen - 1;
		while(strLen-- > 0)
		{
			if (!CharOps::IsWhitespace(*str--)) break;
			++result;
		}
	}

	return result;
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::CountWhitespaces(const CharType* str, int32 strLen, ESearchDir searchDir) noexcept
{
	return searchDir == ESearchDir::Forward 
		? CountWhitespaces<ESearchDir::Forward>(str, strLen)
		: CountWhitespaces<ESearchDir::Backward>(str, strLen);
}

template<typename CharType>
bool TStringOps<CharType>::IsAscii(const CharType* str) noexcept
{
	if constexpr (!TIsSame<CharType, achar>::Value)
	{
		while(*str)
		{
			if (!CharOps::IsAscii(*str++)) return false;
		}
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsAscii(const CharType* str, int32 strLen) noexcept
{
	if constexpr (!TIsSame<CharType, achar>::Value)
	{
		while(strLen-- > 0)
		{
			if (!CharOps::IsAscii(*str++)) return false;
		}
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsNumeric(const CharType* str) noexcept
{
	while(*str)
	{
		if (!CharOps::IsDigit(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsNumeric(const CharType* str, int32 strLen) noexcept
{
	while(strLen-- > 0)
	{
		if (!CharOps::IsDigit(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsWhitespace(const CharType* str) noexcept
{
	while(*str)
	{
		if (!CharOps::IsWhitespace(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsWhitespace(const CharType* str, int32 strLen) noexcept
{
	while(strLen-- > 0)
	{
		if (!CharOps::IsWhitespace(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsUpper(const CharType* str) noexcept
{
	while(*str)
	{
		if (!CharOps::IsUpper(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsUpper(const CharType* str, int32 strLen) noexcept
{
	while(strLen-- > 0)
	{
		if (!CharOps::IsUpper(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsLower(const CharType* str) noexcept
{
	while(*str)
	{
		if (!CharOps::IsLower(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsLower(const CharType* str, int32 strLen) noexcept
{
	while(strLen-- > 0)
	{
		if (!CharOps::IsLower(*str++)) return false;
	}

	return true;
}