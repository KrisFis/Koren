// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
template<ESearchCase Case>
int32 TStringOps<CharType>::Compare(const CharType* lhs, const CharType* rhs) noexcept
{
	using namespace Internal;

	while (*lhs && CharMayLower<Case>(*lhs) == CharMayLower<Case>(*rhs)) ++lhs, ++rhs;
	return (UCharType)CharMayLower<Case>(*lhs) - (UCharType)CharMayLower<Case>(*rhs);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::Compare(const CharType* lhs, const CharType* rhs, ESearchCase searchCase) noexcept
{
	return searchCase == ESearchCase::Sensitive
		? Compare<ESearchCase::Sensitive>(lhs, rhs)
		: Compare<ESearchCase::Insensitive>(lhs, rhs);
}

template<typename CharType>
template<ESearchCase Case>
int32 TStringOps<CharType>::Compare(const CharType* lhs, const CharType* rhs, int32 len) noexcept
{
	using namespace Internal;

	while (len-- > 0 && CharMayLower<Case>(*lhs) == CharMayLower<Case>(*rhs)) ++lhs, ++rhs;
	return len < 0 ? 0 : (UCharType)CharMayLower<Case>(*lhs) - (UCharType)CharMayLower<Case>(*rhs);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::Compare(const CharType* lhs, const CharType* rhs, int32 len, ESearchCase searchCase) noexcept
{
	return searchCase == ESearchCase::Sensitive
		? Compare<ESearchCase::Sensitive>(lhs, rhs, len)
		: Compare<ESearchCase::Insensitive>(lhs, rhs, len);
}

template<typename CharType>
template<ESearchCase Case, int32 len>
KOR_FORCEINLINE int32 TStringOps<CharType>::Compare(CharType(& lhs)[len], const CharType* rhs) noexcept
{
	return Compare<Case>(lhs, rhs, len);
}

template<typename CharType>
template<int32 len>
KOR_FORCEINLINE int32 TStringOps<CharType>::Compare(CharType(& lhs)[len], const CharType* rhs, ESearchCase searchCase) noexcept
{
	return Compare(lhs, rhs, len, searchCase);
}