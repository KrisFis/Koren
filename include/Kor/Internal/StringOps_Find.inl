// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
template<ESearchCase Case, ESearchDir Dir>
int32 TStringOps<CharType>::Find(const CharType* str, const CharType* substr) noexcept
{
	using namespace Internal;

	if constexpr (Dir == ESearchDir::Forward)
	{
		for (int32 i = 0; str[i]; ++i)
		{
			if (CharMayLower<Case>(str[i]) == CharMayLower<Case>(*substr))
			{
				int32 s = i, sub = 0;
				while (str[s] && substr[sub] && CharMayLower<Case>(str[s]) == CharMayLower<Case>(substr[sub]))
				{
					++s, ++sub;
				}
				if (!substr[sub]) return i;
			}
		}
		return KOR_INDEX_NONE;
	}
	else
	{
		int32 last = -1;
		for (int32 i = 0; str[i]; ++i)
		{
			if (CharMayLower<Case>(str[i]) == CharMayLower<Case>(*substr))
			{
				int32 s = i, sub = 0;
				while (str[s] && substr[sub] && CharMayLower<Case>(str[s]) == CharMayLower<Case>(substr[sub]))
				{
					++s, ++sub;
				}
				if (!substr[sub]) last = i;
			}
		}
		return last;
	}
}

template<typename CharType>
int32 TStringOps<CharType>::Find(const CharType* str, const CharType* substr, ESearchCase searchCase, ESearchDir searchDir) noexcept
{
	if (searchCase == ESearchCase::Sensitive)
	{
		return searchDir == ESearchDir::Forward
			? Find<ESearchCase::Sensitive, ESearchDir::Forward>(str, substr)
			: Find<ESearchCase::Sensitive, ESearchDir::Backward>(str, substr);
	}
	else
	{
		return searchDir == ESearchDir::Forward
			? Find<ESearchCase::Insensitive, ESearchDir::Forward>(str, substr)
			: Find<ESearchCase::Insensitive, ESearchDir::Backward>(str, substr);
	}
}

template<typename CharType>
template<ESearchCase Case, ESearchDir Dir>
int32 TStringOps<CharType>::Find(const CharType* str, const CharType* substr, int32 len, int32 subLen) noexcept
{
	using namespace Internal;

	if (subLen > len) return KOR_INDEX_NONE;

	if constexpr (Dir == ESearchDir::Forward)
	{
		const int32 limit = len - subLen;
		for (int32 i = 0; i <= limit; ++i)
		{
			int32 sub = 0;
			while (sub < subLen && CharMayLower<Case>(str[i + sub]) == CharMayLower<Case>(substr[sub]))
			{
				++sub;
			}
			if (sub == subLen) return i;
		}
		return KOR_INDEX_NONE;
	}
	else
	{
		const int32 limit = len - subLen;
		for (int32 i = limit; i >= 0; --i)
		{
			int32 sub = 0;
			while (sub < subLen && CharMayLower<Case>(str[i + sub]) == CharMayLower<Case>(substr[sub]))
			{
				++sub;
			}
			if (sub == subLen) return i;
		}
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
int32 TStringOps<CharType>::Find(const CharType* str, const CharType* substr, int32 len, int32 subLen, ESearchCase searchCase, ESearchDir searchDir) noexcept
{
	if (searchCase == ESearchCase::Sensitive)
	{
		return searchDir == ESearchDir::Forward
			? Find<ESearchCase::Sensitive, ESearchDir::Forward>(str, substr, len, subLen)
			: Find<ESearchCase::Sensitive, ESearchDir::Backward>(str, substr, len, subLen);
	}
	else
	{
		return searchDir == ESearchDir::Forward
			? Find<ESearchCase::Insensitive, ESearchDir::Forward>(str, substr, len, subLen)
			: Find<ESearchCase::Insensitive, ESearchDir::Backward>(str, substr, len, subLen);
	}
}

template<typename CharType>
template<ESearchCase Case, ESearchDir Dir, int32 N, int32 R>
KOR_FORCEINLINE int32 TStringOps<CharType>::Find(const CharType(& str)[N], const CharType(& subStr)[R]) noexcept
{
	return Find<Case, Dir>(str, subStr, N, R);
}

template<typename CharType>
template<int32 N, int32 R>
KOR_FORCEINLINE int32 TStringOps<CharType>::Find(const CharType(& str)[N], const CharType(& subStr)[R], ESearchCase searchCase, ESearchDir searchDir) noexcept
{
	return Find(str, subStr, N, R, searchCase, searchDir);
}

template<typename CharType>
template<ESearchCase Case, ESearchDir Dir>
int32 TStringOps<CharType>::Find(const CharType* str, CharType c) noexcept
{
	using namespace Internal;

	const CharType lc = CharMayLower<Case>(c);

	if constexpr (Dir == ESearchDir::Forward)
	{
		for (int32 i = 0; str[i]; ++i)
		{
			if (CharMayLower<Case>(str[i]) == lc) return i;
		}
		return KOR_INDEX_NONE;
	}
	else
	{
		int32 last = -1;
		for (int32 i = 0; str[i]; ++i)
		{
			if (CharMayLower<Case>(str[i]) == lc) last = i;
		}
		return last;
	}
}

template<typename CharType>
int32 TStringOps<CharType>::Find(const CharType* str, CharType c, ESearchCase searchCase, ESearchDir searchDir) noexcept
{
	if (searchCase == ESearchCase::Sensitive)
	{
		return searchDir == ESearchDir::Forward
			? Find<ESearchCase::Sensitive, ESearchDir::Forward>(str, c)
			: Find<ESearchCase::Sensitive, ESearchDir::Backward>(str, c);
	}
	else
	{
		return searchDir == ESearchDir::Forward
			? Find<ESearchCase::Insensitive, ESearchDir::Forward>(str, c)
			: Find<ESearchCase::Insensitive, ESearchDir::Backward>(str, c);
	}
}

template<typename CharType>
template<ESearchCase Case, ESearchDir Dir>
int32 TStringOps<CharType>::Find(const CharType* str, CharType c, int32 len) noexcept
{
	using namespace Internal;

	const CharType lc = CharMayLower<Case>(c);

	if constexpr (Dir == ESearchDir::Forward)
	{
		for (int32 i = 0; i < len; ++i)
		{
			if (CharMayLower<Case>(str[i]) == lc) return i;
		}
		return KOR_INDEX_NONE;
	}
	else
	{
		for (int32 i = len - 1; i >= 0; --i)
		{
			if (CharMayLower<Case>(str[i]) == lc) return i;
		}
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
int32 TStringOps<CharType>::Find(const CharType* str, CharType c, int32 len, ESearchCase searchCase, ESearchDir searchDir) noexcept
{
	if (searchCase == ESearchCase::Sensitive)
	{
		return searchDir == ESearchDir::Forward
			? Find<ESearchCase::Sensitive, ESearchDir::Forward>(str, c, len)
			: Find<ESearchCase::Sensitive, ESearchDir::Backward>(str, c, len);
	}
	else
	{
		return searchDir == ESearchDir::Forward
			? Find<ESearchCase::Insensitive, ESearchDir::Forward>(str, c, len)
			: Find<ESearchCase::Insensitive, ESearchDir::Backward>(str, c, len);
	}
}

template<typename CharType>
template<ESearchCase Case, ESearchDir Dir, int32 N>
int32 TStringOps<CharType>::Find(const CharType(& str)[N], CharType c) noexcept
{
	return Find<Case, Dir>(str, c, N);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::Find(const CharType(& str)[N], CharType c, ESearchCase searchCase, ESearchDir searchDir) noexcept
{
	return Find(str, c, N, searchCase, searchDir);
}