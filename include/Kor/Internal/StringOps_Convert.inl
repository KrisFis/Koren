// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

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