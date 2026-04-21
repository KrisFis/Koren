// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
constexpr CharType TCharOps<CharType>::MaxCodePoint() noexcept
{
	if constexpr (TIsSame<CharType, achar>::Value) return (CharType)0x7f;
	else
	{
		static_assert(TIsCharacterFixedWidth<CharType>::Value, "Unsupported character type");
		return TLimits<CharType>::Max;
	}
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsAscii(CharType c) noexcept
{
	if constexpr (TIsSame<CharType, achar>::Value) return true;
	else return uint32(UCharType(c)) <= uint32((CharType)0x7f);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsDigit(CharType c) noexcept
{
	return (c >= (CharType)'0') & (c <= (CharType)'9');
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsUpperAlpha(CharType c) noexcept
{
	return (c >= (CharType)'A') & (c <= (CharType)'Z');
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsLowerAlpha(CharType c) noexcept
{
	return (c >= (CharType)'a') & (c <= (CharType)'z');
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsAlpha(CharType c) noexcept
{
	return IsUpperAlpha(c) | IsLowerAlpha(c);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsUpper(CharType c) noexcept
{
	return IsUpperAlpha(c);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsLower(CharType c) noexcept
{
	return IsLowerAlpha(c);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsAlnum(CharType c) noexcept
{
	return IsAlpha(c) | IsDigit(c);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsHexDigit(CharType c) noexcept
{
	return IsDigit(c)
	       | ((c >= (CharType)'a') & (c <= (CharType)'f'))
	       | ((c >= (CharType)'A') & (c <= (CharType)'F'));
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsSign(CharType c) noexcept
{
	return (c == (CharType)'+') | (c == (CharType)'-');
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsWhitespace(CharType c) noexcept
{
	return (c == CharConstant::Space) | (c == CharConstant::Tab)
	       | (c == CharConstant::LineFeed) | (c == CharConstant::CarriageReturn)
	       | (c == CharConstant::VerticalTab) | (c == CharConstant::FormFeed);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsControl(CharType c) noexcept
{
	return (uint32(UCharType(c)) <= uint32((CharType)'\x1f')) | (c == CharConstant::Delete);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsPrint(CharType c) noexcept
{
	return (c >= (CharType)' ') & (c <= (CharType)'~');
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsGraph(CharType c) noexcept
{
	return (c > (CharType)' ') & (c <= (CharType)'~');
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsPunct(CharType c) noexcept
{
	return IsGraph(c) & !IsAlnum(c);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsLinebreak(CharType c) noexcept
{
	return (uint32(UCharType(c)) - uint32(CharConstant::LineFeed))
	       <= uint32(CharConstant::CarriageReturn - CharConstant::LineFeed);
}

template<typename CharType>
KOR_FORCEINLINE constexpr int32 TCharOps<CharType>::DigitToInt(CharType c) noexcept
{
	return IsDigit(c) ? int32(UCharType(c)) - int32('0') : KOR_INDEX_NONE;
}

template<typename CharType>
KOR_FORCEINLINE constexpr int32 TCharOps<CharType>::HexToInt(CharType c) noexcept
{
	if (IsDigit(c)) return int32(UCharType(c)) - int32('0');
	if (IsLowerAlpha(c) & (c <= (CharType)'f')) return int32(UCharType(c)) - int32('a') + 10;
	if (IsUpperAlpha(c) & (c <= (CharType)'F')) return int32(UCharType(c)) - int32('A') + 10;
	return KOR_INDEX_NONE;
}

template<typename CharType>
KOR_FORCEINLINE constexpr int32 TCharOps<CharType>::IntToDigit(int32 n) noexcept
{
	return (uint32(n) <= 9u) ? (CharType)('0' + n) : CharConstant::Null;
}

template<typename CharType>
KOR_FORCEINLINE constexpr CharType TCharOps<CharType>::IntToHex(int32 n, bool bUpper) noexcept
{
	if (uint32(n) <= 9u) return (CharType)('0' + n);
	if (uint32(n) <= 15u) return (CharType)((bUpper ? 'A' : 'a') + (n - 10));
	return CharConstant::Null;
}

template<typename CharType>
KOR_FORCEINLINE constexpr CharType TCharOps<CharType>::ToUpper(CharType c) noexcept
{
	return (CharType)(UCharType(c) - ((uint32(UCharType(c)) - 'a' < 26u) << 5));
}

template<typename CharType>
KOR_FORCEINLINE constexpr CharType TCharOps<CharType>::ToLower(CharType c) noexcept
{
	return (CharType)(UCharType(c) + ((uint32(UCharType(c)) - 'A' < 26u) << 5));
}