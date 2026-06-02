// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Internal
{
	template<typename IntT, typename CharT>
	static IntT Atoi(const CharT* str, const CharT** outEnd, int32 base) noexcept
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
			const int32 digit = TCharOps<CharT>::ToInt(*str);
			if (digit < 0 || digit >= base) break;

			result = result * base + digit;
			++str;
		}

		if (outEnd) *outEnd = str;

		if constexpr (TIsSigned<IntT>::Value) return negative ? -result : result;
		else return result;
	}

	template<typename IntT, typename CharT>
	KOR_FORCEINLINE static IntT Atoi(const CharT* str, int32 base) noexcept
	{
		const CharT* end = nullptr;
		return Atoi<IntT>(str, &end, base);
	}

	template<typename IntT, typename CharT>
	KOR_FORCEINLINE static IntT Atoi(const CharT* str, int32& outLen, int32 base) noexcept
	{
		const CharT* end = nullptr;
		const IntT result = Atoi<IntT>(str, &end, base);
		outLen = KOR_PTR_DIFF(int32, end, str);
		return result;
	}

	template<typename IntT, typename CharT>
	static int32 Itoa(CharT* str, IntT value, int32 maxLen, int32 base) noexcept
	{
		if (maxLen <= 0) return KOR_INDEX_NONE;

		if (value == 0)
		{
			if (maxLen < 2) return KOR_INDEX_NONE;
			str[0] = TCharConstant<CharT>::Zero;
			str[1] = TCharConstant<CharT>::Null;
			return 1;
		}

		bool negative = false;
		if constexpr (TIsSigned<IntT>::Value)
		{
			if (value < 0) { negative = true; value = -value; }
		}

		CharT* end = str + maxLen - 1;
		*end = TCharConstant<CharT>::Null;

		CharT* cur = end;
		while (value > 0)
		{
			if (cur <= str) return KOR_INDEX_NONE;
			*--cur = TCharOps<CharT>::FromInt(value % base);
			value /= base;
		}

		if (negative)
		{
			if (cur <= str) return KOR_INDEX_NONE;
			*--cur = TCharConstant<CharT>::Minus;
		}

		const int32 result = KOR_PTR_DIFF(int32, end, cur);
		if (cur != str)
		{
			SMemory::Copy(str, cur, result);
		}

		str[result] = TCharConstant<CharT>::Null;
		return result;
	}

	template<typename IntT, typename CharT>
	static int32 Itoa(CharT* str, IntT value, int32 base) noexcept
	{
		return Itoa(str, value, SMemory::BUFFER_SIZE_LARGE, base);
	}
}

template<typename CharType>
template<typename ToCharType>
int32 TStringOps<CharType>::ConvertedLength(const CharType* str, int32 len) noexcept
{
	static_assert(TIsCharacter<ToCharType>::Value, "Destination Character Type is not a character type");

	if (len <= 0) [[ unlikely ]] return KOR_INDEX_NONE;

	// 1) Compatible
	// 2) Fixed -> Fixed - always 1:1
	// -------------------------------------------------------------------------
	if constexpr (TIsCharacterCompatible<CharType, ToCharType>::Value
		|| (TIsCharacterFixedWidth<CharType>::Value && TIsCharacterFixedWidth<ToCharType>::Value))
	{
		return len;
	}
	// 3) Fixed -> UTF8 (encode) - variable expansion
	// -------------------------------------------------------------------------
	else if constexpr (TIsCharacterFixedWidth<CharType>::Value
		&& TIsCharacterVariableWidth<ToCharType>::Value)
	{
		int32 count = 0;
		for (int32 i = 0; i < len; ++i)
		{
			const uint32 cp = (uint32)(UCharType)str[i];
			if (cp <= 0x7F) count += 1;
			else if (cp <= 0x7FF) count += 2;
			else if (cp <= 0xFFFF) count += 3;
			else if (cp <= 0x10FFFF)count += 4;
			else count += 3; // U+FFFD fallback
		}
		return count;
	}
	// 4) UTF8 -> Fixed (decode) - one codepoint per output unit
	// -------------------------------------------------------------------------
	else if constexpr (TIsCharacterVariableWidth<CharType>::Value
		&& TIsCharacterFixedWidth<ToCharType>::Value)
	{
		int32 count = 0;
		int32 i = 0;
		while (i < len)
		{
			const uint8 byte = (uint8)str[i];
			if (byte == 0) break;
			else if (byte <= 0x7F) { i += 1; }
			else if ((byte & 0xE0) == 0xC0) { i += 2; }
			else if ((byte & 0xF0) == 0xE0) { i += 3; }
			else if ((byte & 0xF8) == 0xF0) { i += 4; }
			else { i += 1; } // invalid byte, still 1 output
			count++;
		}
		return count;
	}
	// 5) UTF8 -> UTF8 - same count
	// -------------------------------------------------------------------------
	else if constexpr (TIsCharacterVariableWidth<CharType>::Value
		&& TIsCharacterVariableWidth<ToCharType>::Value)
	{
		return len;
	}
	else
	{
		static_assert(sizeof(ToCharType) == 0, "Unsupported ConvertedLength combination");
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
template<typename ToCharType, TSize N>
KOR_FORCEINLINE int32 TStringOps<CharType>::ConvertedLength(const CharType (&str)[N]) noexcept
{
	return ConvertedLength(str, N);
}

template<typename CharType>
template<typename ToCharType>
int32 TStringOps<CharType>::Convert(const CharType* str, ToCharType* toStr, int32 len) noexcept
{
	static_assert(TIsCharacter<ToCharType>::Value, "Destination Character Type is not a character type");

	using ToCharConstant = TCharConstant<ToCharType>;
	using ToCharOps = TCharOps<ToCharType>;

	if (len <= 0) [[ unlikely ]] return KOR_INDEX_NONE;

	// 1) Compatible types - memcopy
	// * ANSI->UTF8, WCHAR->CHAR16/CHAR32 (platform-dependent)
	// -------------------------------------------------------------------------
	if constexpr (TIsCharacterCompatible<CharType, ToCharType>::Value)
	{
		SMemory::Copy(toStr, str, sizeof(CharType) * (len + 1));
		return len;
	}
	// 2) Fixed -> Fixed (upcast, no data loss possible)
	// * ANSI->CHAR16, ANSI->CHAR32, CHAR16->CHAR32
	// -------------------------------------------------------------------------
	else if constexpr (TIsCharacterFixedWidth<CharType>::Value
		&& TIsCharacterFixedWidth<ToCharType>::Value
		&& sizeof(ToCharType) >= sizeof(CharType))
	{
		for (int32 i = 0; i < len + 1; ++i)
		{
			toStr[i] = (ToCharType)(UCharType)str[i];
		}
		return len;
	}
	// 3) Fixed -> Fixed (downcast, data loss possible)
	// * CHAR32->CHAR16, CHAR32->ANSI, CHAR16->ANSI
	// -------------------------------------------------------------------------
	else if constexpr (TIsCharacterFixedWidth<CharType>::Value
		&& TIsCharacterFixedWidth<ToCharType>::Value
		&& sizeof(ToCharType) < sizeof(CharType))
	{
		for (int32 i = 0; i < len + 1; ++i)
		{
			toStr[i] = str[i] > (UCharType)ToCharOps::MaxCodePoint()
				? ToCharConstant::Bogus
				: (ToCharType)(UCharType)str[i];
		}
		return len;
	}
	// 4) Fixed -> UTF8 (encode)
	// * ANSI->UTF8, CHAR16->UTF8, CHAR32->UTF8
	// -------------------------------------------------------------------------
	else if constexpr (TIsCharacterFixedWidth<CharType>::Value
		&& TIsCharacterVariableWidth<ToCharType>::Value)
	{
		int32 written = 0;
		for (int32 i = 0; i < len + 1; ++i)
		{
			const uint32 cp = (uint32)(UCharType)str[i];

			if (cp == 0) // null terminator
			{
				toStr[written++] = (ToCharType)0;
				break;
			}
			else if (cp <= 0x7F)
			{
				toStr[written++] = (ToCharType)cp;
			}
			else if (cp <= 0x7FF)
			{
				toStr[written++] = (ToCharType)(0xC0 | (cp >> 6));
				toStr[written++] = (ToCharType)(0x80 | (cp & 0x3F));
			}
			else if (cp <= 0xFFFF)
			{
				toStr[written++] = (ToCharType)(0xE0 | (cp >> 12));
				toStr[written++] = (ToCharType)(0x80 | ((cp >> 6) & 0x3F));
				toStr[written++] = (ToCharType)(0x80 | (cp & 0x3F));
			}
			else if (cp <= 0x10FFFF)
			{
				toStr[written++] = (ToCharType)(0xF0 | (cp >> 18));
				toStr[written++] = (ToCharType)(0x80 | ((cp >> 12) & 0x3F));
				toStr[written++] = (ToCharType)(0x80 | ((cp >> 6) & 0x3F));
				toStr[written++] = (ToCharType)(0x80 | (cp & 0x3F));
			}
			else // bogus codepoint
			{
				// U+FFFD encoded as UTF8: 0xEF 0xBF 0xBD
				toStr[written++] = (ToCharType)0xEF;
				toStr[written++] = (ToCharType)0xBF;
				toStr[written++] = (ToCharType)0xBD;
			}
		}
		return written - 1; // exclude null terminator
	}
	// 5) UTF8 -> Fixed (decode)
	// * UTF8->ANSI, UTF8->CHAR16, UTF8->CHAR32
	// -------------------------------------------------------------------------
	else if constexpr (TIsCharacterVariableWidth<CharType>::Value
		&& TIsCharacterFixedWidth<ToCharType>::Value)
	{
		int32 written = 0;
		int32 i = 0;
		while (i < len)
		{
			const uint8 byte = (uint8)str[i];
			uint32 cp = 0;

			if (byte == 0) // null terminator
			{
				toStr[written++] = (ToCharType)0;
				break;
			}
			else if (byte <= 0x7F) // 1 byte
			{
				cp = byte;
				i += 1;
			}
			else if ((byte & 0xE0) == 0xC0) // 2 bytes
			{
				if (i + 1 >= len) { toStr[written++] = ToCharConstant::Bogus; i++; continue; }
				cp = ((byte & 0x1F) << 6) | ((uint8)str[i+1] & 0x3F);
				i += 2;
			}
			else if ((byte & 0xF0) == 0xE0) // 3 bytes
			{
				if (i + 2 >= len) { toStr[written++] = ToCharConstant::Bogus; i++; continue; }
				cp = ((byte & 0x0F) << 12)
					| (((uint8)str[i+1] & 0x3F) << 6)
					| ((uint8)str[i+2] & 0x3F);
				i += 3;
			}
			else if ((byte & 0xF8) == 0xF0) // 4 bytes
			{
				if (i + 3 >= len) { toStr[written++] = ToCharConstant::Bogus; i++; continue; }
				cp = ((byte & 0x07) << 18)
					| (((uint8)str[i+1] & 0x3F) << 12)
					| (((uint8)str[i+2] & 0x3F) << 6)
					| ((uint8)str[i+3] & 0x3F);
				i += 4;
			}
			else // invalid byte
			{
				toStr[written++] = ToCharConstant::Bogus;
				i++;
				continue;
			}

			// Now emit codepoint into ToCharType
			if (cp > (uint32)ToCharOps::MaxCodePoint())
			{
				toStr[written++] = ToCharConstant::Bogus;
			}
			else
			{
				toStr[written++] = (ToCharType)cp;
			}
		}

		if (written == 0 || toStr[written - 1] != (ToCharType)0)
		{
			toStr[written++] = ToCharConstant::Null;
		}

		return written - 1; // exclude null terminator
	}
	else
	{
		static_assert(sizeof(ToCharType) == 0, "Unsupported StringConvert combination");
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
template<typename ToCharType, TSize N>
KOR_FORCEINLINE int32 TStringOps<CharType>::Convert(const CharType (&str)[N], ToCharType* toStr) noexcept
{
	return Convert(str, toStr, N);
}

template<typename CharType>
KOR_FORCEINLINE int64 TStringOps<CharType>::ToInt(const CharType* str, int32 base) noexcept
{
	return Internal::Atoi<int64>(str, base);
}

template<typename CharType>
KOR_FORCEINLINE int64 TStringOps<CharType>::ToInt(const CharType* str, int32& outLen, int32 base) noexcept
{
	return Internal::Atoi<int64>(str, outLen, base);
}

template<typename CharType>
KOR_FORCEINLINE uint64 TStringOps<CharType>::ToUInt(const CharType* str, int32 base) noexcept
{
	return Internal::Atoi<uint64>(str, base);
}

template<typename CharType>
KOR_FORCEINLINE uint64 TStringOps<CharType>::ToUInt(const CharType* str, int32& outLen, int32 base) noexcept
{
	return Internal::Atoi<uint64>(str, outLen, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromInt(CharType* str, int64 value, int32 base) noexcept
{
	return Internal::Itoa(str, value, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromInt(CharType* str, int64 value, int32 maxLen, int32 base) noexcept
{
	return Internal::Itoa(str, value, maxLen, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromUInt(CharType* str, uint64 value, int32 base) noexcept
{
	return Internal::Itoa(str, value, SMemory::BUFFER_SIZE_LARGE, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromUInt(CharType* str, uint64 value, int32 maxLen, int32 base) noexcept
{
	return Internal::Itoa(str, value, maxLen, base);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromInt(CharType (&str)[N], int64 value, int32 base) noexcept
{
	return FromInt(str, value, N, base);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromUInt(CharType (&str)[N], uint64 value, int32 base) noexcept
{
	return FromUInt(str, value, N, base);
}

template<typename CharType>
KOR_FORCEINLINE double TStringOps<CharType>::ToFloat(const CharType* str) noexcept
{
	if constexpr (TIsSame<CharType, achar>::Value || TIsSame<CharType, char8>::Value)
	{
		const achar* end = nullptr;
		return SPlatformAnsiStringOps::Strtod(reinterpret_cast<const achar*>(str), &end);
	}
	else if constexpr (TIsSame<CharType, wchar>::Value)
	{
		const wchar* end = nullptr;
		return SPlatformWideStringOps::Strtod(str, &end);
	}
	else
	{
		static_assert(sizeof(CharType) == 0, "FromFloat not supported for this character type");
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
KOR_FORCEINLINE double TStringOps<CharType>::ToFloat(const CharType* str, int32& outLen) noexcept
{
	// IMPLEMENT PlatformStringOps

	if constexpr (TIsSame<CharType, achar>::Value || TIsSame<CharType, char8>::Value)
	{
		achar* end = nullptr;
		const double result = SPlatformAnsiStringOps::Strtod(str, &end);
		outLen = KOR_PTR_DIFF(int32, end, str);
		return result;
	}
	else if constexpr (TIsSame<CharType, wchar>::Value)
	{
		wchar* end = nullptr;
		const double result = SPlatformWideStringOps::Strtod(str, &end);
		outLen = KOR_PTR_DIFF(int32, end, str);
		return result;
	}
	else
	{
		static_assert(sizeof(CharType) == 0, "FromFloat not supported for this character type");
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
template<EFloatFormat Format>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromFloat(CharType* str, double value, int32 precision) noexcept
{
	return FromFloat(str, value, SMemory::BUFFER_SIZE_DOUBLE_MAX, precision);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromFloat(CharType* str, double value, int32 precision, EFloatFormat format) noexcept
{
	switch (format)
	{
		case EFloatFormat::Fixed:
			return FromFloat<EFloatFormat::Fixed>(str, value, precision);
		case EFloatFormat::Scientific:
			return FromFloat<EFloatFormat::Scientific>(str, value, precision);
		default:
			return FromFloat<EFloatFormat::Auto>(str, value, precision);
	}
}

template<typename CharType>
template<EFloatFormat Format>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromFloat(CharType* str, double value, int32 maxLen, int32 precision) noexcept
{
	if constexpr (TIsSame<CharType, achar>::Value || TIsSame<CharType, char8>::Value)
	{
		const achar* fmtStr =
			Format == EFloatFormat::Fixed		? KOR_TEXT_ANSI("%.*f") :
			Format == EFloatFormat::Scientific	? KOR_TEXT_ANSI("%.*e") : KOR_TEXT_ANSI("%.*g");
		return SPlatformAnsiStringOps::Snprintf(reinterpret_cast<achar*>(str), fmtStr, maxLen, precision, value);
	}
	else if constexpr (TIsSame<CharType, wchar>::Value)
	{
		const wchar* fmtStr =
			Format == EFloatFormat::Fixed		? KOR_TEXT_WIDE("%.*f") :
			Format == EFloatFormat::Scientific	? KOR_TEXT_WIDE("%.*e") : KOR_TEXT_WIDE("%.*g");
		return SPlatformWideStringOps::Snprintf(str, fmtStr, maxLen, precision, value);
	}
	else
	{
		static_assert(sizeof(CharType) == 0, "FromFloat not supported for this character type");
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromFloat(CharType* str, double value, int32 maxLen, int32 precision, EFloatFormat format) noexcept
{
	switch (format)
	{
		case EFloatFormat::Fixed:
			return FromFloat<EFloatFormat::Fixed>(str, value, maxLen, precision);
		case EFloatFormat::Scientific:
			return FromFloat<EFloatFormat::Scientific>(str, value, maxLen, precision);
		default:
			return FromFloat<EFloatFormat::Auto>(str, value, maxLen, precision);
	}
}

template<typename CharType>
template<EFloatFormat Format, int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromFloat(CharType(& str)[N], double value, int32 precision) noexcept
{
	return FromFloat<Format>(str, value, N, precision);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromFloat(CharType(& str)[N], double value, int32 precision, EFloatFormat format) noexcept
{
	return FromFloat(str, value, N, precision, format);
}