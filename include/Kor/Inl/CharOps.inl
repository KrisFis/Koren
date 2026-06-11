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
	return (c >= TCharConstant<CharType>::Zero) & (c <= TCharConstant<CharType>::Nine);
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
	return (c == TCharConstant<CharType>::Plus) | (c == TCharConstant<CharType>::Minus);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsWhitespace(CharType c) noexcept
{
	return (c == TCharConstant<CharType>::Space) | (c == TCharConstant<CharType>::Tab)
		| (c == TCharConstant<CharType>::LineFeed) | (c == TCharConstant<CharType>::CarriageReturn)
		| (c == TCharConstant<CharType>::VerticalTab) | (c == TCharConstant<CharType>::FormFeed);
}

template<typename CharType>
KOR_FORCEINLINE constexpr bool TCharOps<CharType>::IsControl(CharType c) noexcept
{
	return (uint32(UCharType(c)) <= uint32((CharType)'\x1f')) | (c == TCharConstant<CharType>::Delete);
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
	return (uint32(UCharType(c)) - uint32(TCharConstant<CharType>::LineFeed))
		<= uint32(TCharConstant<CharType>::CarriageReturn - TCharConstant<CharType>::LineFeed);
}

template<typename CharType>
KOR_FORCEINLINE constexpr int32 TCharOps<CharType>::ToInt(CharType c) noexcept
{
	if (IsDigit(c))			return int32(UCharType(c)) - int32('0');
	if (IsLowerAlpha(c))	return int32(UCharType(c)) - int32('a') + 10;
	if (IsUpperAlpha(c))	return int32(UCharType(c)) - int32('A') + 10;
	return KOR_INDEX_NONE;
}

template<typename CharType>
KOR_FORCEINLINE constexpr CharType TCharOps<CharType>::FromInt(int32 n) noexcept
{
	if (uint32(n) <= 9u) return (CharType)('0' + n);
	if (uint32(n) <= 35u) return (CharType)('a' + (n - 10));
	return TCharConstant<CharType>::Null;
}

template<typename CharType>
KOR_FORCEINLINE constexpr int32 TCharOps<CharType>::ToDigit(CharType c) noexcept
{
	return IsDigit(c) ? int32(UCharType(c)) - int32('0') : KOR_INDEX_NONE;
}

template<typename CharType>
KOR_FORCEINLINE constexpr CharType TCharOps<CharType>::FromDigit(int32 n) noexcept
{
	return (uint32(n) <= 9u) ? (CharType)('0' + n) : TCharConstant<CharType>::Null;
}

template<typename CharType>
KOR_FORCEINLINE constexpr int32 TCharOps<CharType>::ToHex(CharType c) noexcept
{
	if (IsDigit(c))								return int32(UCharType(c)) - int32('0');
	if (IsLowerAlpha(c) & (c <= (CharType)'f')) return int32(UCharType(c)) - int32('a') + 10;
	if (IsUpperAlpha(c) & (c <= (CharType)'F')) return int32(UCharType(c)) - int32('A') + 10;
	return KOR_INDEX_NONE;
}

template<typename CharType>
KOR_FORCEINLINE constexpr CharType TCharOps<CharType>::FromHex(int32 n) noexcept
{
	if (uint32(n) <= 9u) return (CharType)('0' + n);
	if (uint32(n) <= 15u) return (CharType)('a' + (n - 10));
	return TCharConstant<CharType>::Null;
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