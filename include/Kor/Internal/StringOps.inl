// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Internal
{
	template<typename CharT, ESearchCase Case>
	KOR_FORCEINLINE static CharT CharMayLower(CharT c)
	{
		if constexpr (Case == ESearchCase::Insensitive) return TCharOps<CharT>::ToLower(c);
		else return c;
	}

	template<typename CharType, uint16 Size>
	KOR_FORCEINLINE static CharType* GetBuffer() noexcept
	{
		thread_local CharType buffer[Size];
		return buffer;
	}

	template<typename IntT, typename CharT>
	static IntT StrToInt(const CharT* str, const CharT** outEnd, int32 base) noexcept
	{
		// CRT version removes leading whitespaces
		// while (CharOps::IsWhitespace(*str)) ++str;

		IntT result = 0;
		bool negative = false;

		if (*str == TCharConstant<CharT>::Plus) { ++str; }
		else if constexpr (TIsSigned<IntT>::Value)
		{
			if (*str == TCharConstant<CharT>::Minus) { negative = true; ++str; }
		}

		while (*str)
		{
			const int32 digit = TCharOps<CharT>::ToDigit(*str, base);
			if (digit < 0) break;
			result = result * base + digit;
			++str;
		}

		if (outEnd) *outEnd = str;

		if constexpr (TIsSigned<IntT>::Value)
		{
			return negative ? -result : result;
		}
		else
		{
			return result;
		}
	}

	template<typename IntT, typename CharT>
	static int32 IntToStr(CharT* str, IntT value, int32 base = 10) noexcept
	{
		if (value == 0)
		{
			str[0] = TCharConstant<CharT>::Zero;
			str[1] = TCharConstant<CharT>::Null;
			return 1;
		}

		bool negative = false;
		if constexpr (TIsSigned<IntT>::Value)
			if (value < 0) { negative = true; value = -value; }

		CharT* buff = GetBuffer<CharT, 64>();
		CharT* cur = buff + 64;
		*--cur = TCharConstant<CharT>::Null;

		while (value > 0)
		{
			*--cur = TCharOps<CharT>::FromDigit(value % base, base);
			value /= base;
		}

		if (negative)
			*--cur = TCharConstant<CharT>::Minus;

		const int32 len = (int32)((buff + 64) - cur - 1);
		Copy(str, cur, len);
		str[len] = TCharConstant<CharT>::Null;
		return len;
	}

	template<typename IntT, typename CharT>
	static int32 IntToStr(CharT* str, IntT value, int32 len, int32 base = 10) noexcept
	{
		if (len <= 0) return KOR_INDEX_NONE;

		if (value == 0)
		{
			if (len < 2) return KOR_INDEX_NONE;
			str[0] = TCharConstant<CharT>::Zero;
			str[1] = TCharConstant<CharT>::Null;
			return 1;
		}

		bool negative = false;
		if constexpr (TIsSigned<IntT>::Value)
			if (value < 0) { negative = true; value = -value; }

		CharT* end = str + len - 1;
		*end = TCharConstant<CharT>::Null;

		CharT* cur = end;
		while (value > 0)
		{
			if (cur <= str) return KOR_INDEX_NONE;
			*--cur = TCharOps<CharT>::FromDigit(value % base, base);
			value /= base;
		}

		if (negative)
		{
			if (cur <= str) return KOR_INDEX_NONE;
			*--cur = TCharConstant<CharT>::Minus;
		}

		const int32 result = (int32)(end - cur);
		if (cur != str)
			Copy(str, cur, result);

		str[result] = TCharConstant<CharT>::Null;
		return result;
	}
}

template<typename CharType>
int32 TStringOps<CharType>::Length(const CharType* str) noexcept
{
	int32 len = 0; while (*str) ++len; return len;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::Length(const CharType(&)[N]) noexcept
{
	return N;
}

#include "Kor/Internal/StringOps_Query.inl"
#include "Kor/Internal/StringOps_Compare.inl"
#include "Kor/Internal/StringOps_Transform.inl"
#include "Kor/Internal/StringOps_Find.inl"
#include "Kor/Internal/StringOps_Replace.inl"
#include "Kor/Internal/StringOps_Format.inl"
#include "Kor/Internal/StringOps_Convert.inl"