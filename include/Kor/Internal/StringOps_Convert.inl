// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

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
			if      (cp <= 0x7F)    count += 1;
			else if (cp <= 0x7FF)   count += 2;
			else if (cp <= 0xFFFF)  count += 3;
			else if (cp <= 0x10FFFF)count += 4;
			else                    count += 3; // U+FFFD fallback
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
			if      (byte == 0)              break;
			else if (byte <= 0x7F)           { i += 1; }
			else if ((byte & 0xE0) == 0xC0)  { i += 2; }
			else if ((byte & 0xF0) == 0xE0)  { i += 3; }
			else if ((byte & 0xF8) == 0xF0)  { i += 4; }
			else                             { i += 1; } // invalid byte, still 1 output
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
			toStr[i] = (ToCharType)(CharType)str[i];
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
			toStr[i] = str[i] > (CharType)TLimits<ToCharType>::Max
				? ToCharConstant::Bogus
				: (ToCharType)(CharType)str[i];
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
			const uint32 cp = (uint32)(CharType)str[i];

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
			if (cp > (uint32)TLimits<ToCharType>::Max)
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
KOR_FORCEINLINE int32 TStringOps<CharType>::ToInt32(const CharType* str, int32 base) noexcept
{
	return Internal::StrToInt<int32>(str, nullptr, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::ToInt32(const CharType* str, const CharType*& outEnd, int32 base) noexcept
{
	return Internal::StrToInt<int32>(str, &outEnd, base);
}

template<typename CharType>
KOR_FORCEINLINE int64 TStringOps<CharType>::ToInt64(const CharType* str, int32 base) noexcept
{
	return Internal::StrToInt<int64>(str, nullptr, base);
}

template<typename CharType>
KOR_FORCEINLINE int64 TStringOps<CharType>::ToInt64(const CharType* str, const CharType*& outEnd, int32 base) noexcept
{
	return Internal::StrToInt<int64>(str, &outEnd, base);
}

template<typename CharType>
KOR_FORCEINLINE uint32 TStringOps<CharType>::ToUInt32(const CharType* str, int32 base) noexcept
{
	return Internal::StrToInt<uint32>(str, nullptr, base);
}

template<typename CharType>
KOR_FORCEINLINE uint32 TStringOps<CharType>::ToUInt32(const CharType* str, const CharType*& outEnd, int32 base) noexcept
{
	return Internal::StrToInt<uint32>(str, &outEnd, base);
}

template<typename CharType>
KOR_FORCEINLINE uint64 TStringOps<CharType>::ToUInt64(const CharType* str, int32 base) noexcept
{
	return Internal::StrToInt<uint64>(str, nullptr, base);
}

template<typename CharType>
KOR_FORCEINLINE uint64 TStringOps<CharType>::ToUInt64(const CharType* str, const CharType*& outEnd, int32 base) noexcept
{
	return Internal::StrToInt<uint64>(str, &outEnd, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromInt32(CharType* str, int32 value, int32 base) noexcept
{
	return Internal::IntToStr(str, value, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromInt32(CharType* str, int32 value, int32 len, int32 base) noexcept
{
	return Internal::IntToStr(str, value, len, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromInt64(CharType* str, int64 value, int32 base) noexcept
{
	return Internal::IntToStr(str, value, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromInt64(CharType* str, int64 value, int32 len, int32 base) noexcept
{
	return Internal::IntToStr(str, value, len, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromUInt32(CharType* str, uint32 value, int32 base) noexcept
{
	return Internal::IntToStr(str, value, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromUInt32(CharType* str, uint32 value, int32 len, int32 base) noexcept
{
	return Internal::IntToStr(str, value, len, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromUInt64(CharType* str, uint64 value, int32 base) noexcept
{
	return Internal::IntToStr(str, value, base);
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromUInt64(CharType* str, uint64 value, int32 len, int32 base) noexcept
{
	return Internal::IntToStr(str, value, len, base);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromInt32(CharType (&str)[N], int32 value, int32 base) noexcept
{
	return FromInt32(str, value, N, base);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromInt64(CharType (&str)[N], int64 value, int32 base) noexcept
{
	return FromInt64(str, value, N, base);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromUInt32(CharType (&str)[N], uint32 value, int32 base) noexcept
{
	return FromUInt32(str, value, N, base);
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromUInt64(CharType (&str)[N], uint64 value, int32 base) noexcept
{
	return FromUInt64(str, value, N, base);
}

template<typename CharType>
KOR_FORCEINLINE double TStringOps<CharType>::ToFloat(const CharType* str) noexcept
{
	// IMPLEMENT PlatformStringOps

	if constexpr (TIsSame<CharType, achar>::Value)
	{
		return strtod(str, nullptr);
	}
	else if constexpr (TIsSame<CharType, wchar>::Value)
	{
		return wcstod(str, nullptr);
	}
	else if constexpr (TIsSame<CharType, char8>::Value)
	{
		return strtod(reinterpret_cast<const achar*>(str), nullptr);
	}
	else
	{
		static_assert(sizeof(CharType) == 0, "FromFloat not supported for this character type");
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
KOR_FORCEINLINE double TStringOps<CharType>::ToFloat(const CharType* str, const CharType*& outEnd) noexcept
{
	// IMPLEMENT PlatformStringOps

	if constexpr (TIsSame<CharType, achar>::Value)
	{
		return strtod(str, &outEnd);
	}
	else if constexpr (TIsSame<CharType, wchar>::Value)
	{
		return wcstod(str, &outEnd);
	}
	else if constexpr (TIsSame<CharType, char8>::Value)
	{
		achar* end = nullptr;
		const double result = strtod(reinterpret_cast<const achar*>(str), &end);
		outEnd = reinterpret_cast<const CharType*>(end);
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
	return FromFloat(str, value, SCString::MAX_BUFFER_SIZE_DOUBLE, precision);
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
KOR_FORCEINLINE int32 TStringOps<CharType>::FromFloat(CharType* str, double value, int32 len, int32 precision) noexcept
{
	if constexpr (TIsSame<CharType, achar>::Value)
	{
		const achar* fmtStr =
			Format == EFloatFormat::Fixed		? KOR_TEXT_ANSI("%.*f") :
			Format == EFloatFormat::Scientific	? KOR_TEXT_ANSI("%.*e") : KOR_TEXT_ANSI("%.*g");
		return Internal::Format(str, fmtStr, len, precision, value);
	}
	else if constexpr (TIsSame<CharType, wchar>::Value)
	{
		const wchar* fmtStr =
			Format == EFloatFormat::Fixed		? KOR_TEXT_WIDE("%.*f") :
			Format == EFloatFormat::Scientific	? KOR_TEXT_WIDE("%.*e") : KOR_TEXT_WIDE("%.*g");
		return Internal::Format(str, fmtStr, len, precision, value);
	}
	else if constexpr (TIsSame<CharType, wchar>::Value)
	{
		const achar* fmtStr =
			Format == EFloatFormat::Fixed		? KOR_TEXT_ANSI("%.*f") :
			Format == EFloatFormat::Scientific	? KOR_TEXT_ANSI("%.*e") : KOR_TEXT_ANSI("%.*g");
		return Internal::Format(reinterpret_cast<achar*>(str), fmtStr, len, precision, value);
	}
	else
	{
		static_assert(sizeof(CharType) == 0, "FromFloat not supported for this character type");
		return KOR_INDEX_NONE;
	}
}

template<typename CharType>
KOR_FORCEINLINE int32 TStringOps<CharType>::FromFloat(CharType* str, double value, int32 len, int32 precision, EFloatFormat format) noexcept
{
	switch (format)
	{
		case EFloatFormat::Fixed:
			return FromFloat<EFloatFormat::Fixed>(str, value, len, precision);
		case EFloatFormat::Scientific:
			return FromFloat<EFloatFormat::Scientific>(str, value, len, precision);
		default:
			return FromFloat<EFloatFormat::Auto>(str, value, len, precision);
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