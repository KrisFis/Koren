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
int32 TStringOps<CharType>::CountWhitespaces(const CharType* str, ESearchDir searchDir) noexcept
{
	return searchDir == ESearchDir::Forward 
		? CountWhitespaces<ESearchDir::Forward>(str)
		: CountWhitespaces<ESearchDir::Backward>(str);
}

template<typename CharType>
template<ESearchDir Dir>
int32 TStringOps<CharType>::CountWhitespaces(const CharType* str, int32 len) noexcept
{
	int32 result = 0;

	if constexpr (Dir == ESearchDir::Forward)
	{
		while (len-- > 0)
		{
			if (!CharOps::IsWhitespace(*str++)) break;
			++result;
		}
	}
	else
	{
		str += len - 1;
		while(len-- > 0)
		{
			if (!CharOps::IsWhitespace(*str--)) break;
			++result;
		}
	}

	return result;
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::CountWhitespaces(const CharType* str, int32 len, ESearchDir searchDir) noexcept
{
	return searchDir == ESearchDir::Forward 
		? CountWhitespaces<ESearchDir::Forward>(str, len)
		: CountWhitespaces<ESearchDir::Backward>(str, len);
}

template<typename CharType>
template<ESearchDir Dir, int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::CountWhitespaces(const CharType (&str)[N]) noexcept
{
	return CountWhitespaces<Dir>(str, N);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::CountWhitespaces(const CharType (&str)[N], ESearchDir searchDir) noexcept
{
	return CountWhitespaces(str, N, searchDir);
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
bool TStringOps<CharType>::IsAscii(const CharType* str, int32 len) noexcept
{
	if constexpr (!TIsSame<CharType, achar>::Value)
	{
		while(len-- > 0)
		{
			if (!CharOps::IsAscii(*str++)) return false;
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
		if (!CharOps::IsDigit(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsNumeric(const CharType* str, int32 len) noexcept
{
	while(len-- > 0)
	{
		if (!CharOps::IsDigit(*str++)) return false;
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
bool TStringOps<CharType>::IsWhitespace(const CharType* str) noexcept
{
	while(*str)
	{
		if (!CharOps::IsWhitespace(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsWhitespace(const CharType* str, int32 len) noexcept
{
	while(len-- > 0)
	{
		if (!CharOps::IsWhitespace(*str++)) return false;
	}

	return true;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE bool TStringOps<CharType>::IsWhitespace(const CharType (&str)[N]) noexcept
{
	return IsWhitespace(str, N);
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
bool TStringOps<CharType>::IsUpper(const CharType* str, int32 len) noexcept
{
	while(len-- > 0)
	{
		if (!CharOps::IsUpper(*str++)) return false;
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
		if (!CharOps::IsLower(*str++)) return false;
	}

	return true;
}

template<typename CharType>
bool TStringOps<CharType>::IsLower(const CharType* str, int32 len) noexcept
{
	while(len-- > 0)
	{
		if (!CharOps::IsLower(*str++)) return false;
	}

	return true;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE bool TStringOps<CharType>::IsLower(const CharType(& str)[N]) noexcept
{
	return IsLower(str, N);
}