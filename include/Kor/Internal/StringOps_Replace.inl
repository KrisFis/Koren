// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
template<ESearchCase Case>
int32 TStringOps<CharType>::Replace(CharType* str, const CharType* from, const CharType* to) noexcept
{
	CharType* buffer = Internal::GetBuffer<CharType, SCString::LARGE_BUFFER_SIZE>();

	const int32 fromLen = Length(from);
	const int32 toLen   = Length(to);

	int32 count = 0;

	CharType* dst = buffer;
	const CharType* src = str;

	while (*src)
	{
		int32 idx = Find<Case, ESearchDir::Forward>(src, from);
		if (idx < 0)
		{
			while (*src) *dst++ = *src++;
			break;
		}

		for (int32 i = 0; i < idx; ++i) *dst++ = *src++;
		for (int32 i = 0; i < toLen; ++i) *dst++ = to[i];
		src += fromLen;
		++count;
	}

	*dst = CharConstant::Null;
	Copy(str, buffer);
	return count;
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::Replace(CharType* str, const CharType* from, const CharType* to, ESearchCase searchCase) noexcept
{
	return searchCase == ESearchCase::Sensitive
		? Replace<ESearchCase::Sensitive>(str, from, to)
		: Replace<ESearchCase::Insensitive>(str, from, to);
}

template<typename CharType>
template<ESearchCase Case>
int32 TStringOps<CharType>::Replace(CharType* str, const CharType* from, const CharType* to, int32 len) noexcept
{
	CharType* buffer = Internal::GetBuffer<CharType, SCString::LARGE_BUFFER_SIZE>();

	const int32 fromLen = Length(from);
	const int32 toLen = Length(to);

	int32 count = 0;
	CharType* dst = buffer;
	const CharType* src = str;
	int32 remaining = len;

	while (remaining > 0)
	{
		int32 idx = Find<Case, ESearchDir::Forward>(src, from, remaining, fromLen);
		if (idx < 0)
		{
			for (int32 i = 0; i < remaining; ++i) *dst++ = *src++;
			break;
		}

		for (int32 i = 0; i < idx; ++i) *dst++ = *src++;
		for (int32 i = 0; i < toLen; ++i) *dst++ = to[i];
		src += fromLen;
		remaining -= idx + fromLen;
		++count;
	}

	*dst = CharConstant::Null;
	Copy(str, buffer, len);
	return count;
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::Replace(CharType* str, const CharType* from, const CharType* to, int32 len, ESearchCase searchCase) noexcept
{
	return searchCase == ESearchCase::Sensitive
		? Replace<ESearchCase::Sensitive>(str, from, to, len)
		: Replace<ESearchCase::Insensitive>(str, from, to, len);
}

template<typename CharType>
template<ESearchCase Case, int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::Replace(CharType(& str)[N], const CharType* from, const CharType* to) noexcept
{
	return Replace(str, from, to, N);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::Replace(CharType(& str)[N], const CharType* from, const CharType* to, ESearchCase searchCase) noexcept
{
	return Replace(str, from, to, N, searchCase);
}

template<typename CharType>
template<ESearchCase Case>
int32 TStringOps<CharType>::Replace(CharType* str, CharType from, CharType to) noexcept
{
	int32 result = 0;

	const CharType lf = CharMayLower<Case>(from);
	while (*str)
	{
		if (CharMayLower<Case>(*str) == lf)
		{
			++result;
			*str = to;
		}

		++str;
	}

	return result;
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::Replace(CharType* str, CharType from, CharType to, ESearchCase searchCase) noexcept
{
	return searchCase == ESearchCase::Sensitive
		? Replace<ESearchCase::Sensitive>(str, from, to)
		: Replace<ESearchCase::Insensitive>(str, from, to);
}

template<typename CharType>
template<ESearchCase Case>
int32 TStringOps<CharType>::Replace(CharType* str, CharType from, CharType to, int32 len) noexcept
{
	int32 result = 0;

	const CharType lf = CharMayLower<Case>(from);
	while (len-- > 0)
	{
		if (CharMayLower<Case>(*str) == lf)
		{
			++result;
			*str = to;
		}

		++str;
	}

	return result;
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::Replace(CharType* str, CharType from, CharType to, int32 len, ESearchCase searchCase) noexcept
{
	return searchCase == ESearchCase::Sensitive
		? Replace<ESearchCase::Sensitive>(str, from, to, len)
		: Replace<ESearchCase::Insensitive>(str, from, to, len);
}

template<typename CharType>
template<ESearchCase Case, int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::Replace(CharType(& str)[N], CharType from, CharType to) noexcept
{
	return Replace<Case>(str, from, to, N);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::Replace(CharType(& str)[N], CharType from, CharType to, ESearchCase searchCase) noexcept
{
	return Replace(str, from, to, N, searchCase);
}