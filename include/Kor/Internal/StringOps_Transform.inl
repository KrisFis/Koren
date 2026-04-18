// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharType>
void TStringOps<CharType>::ToUpper(CharType* str) noexcept
{
	while(*str)
	{
		*str = CharOps::ToUpper(*str);

		// Compiler dependent order of read
		// *(str++) = CharOps::ToUpper(*str);

		++str;
	}
}

template<typename CharType>
void TStringOps<CharType>::ToUpper(CharType* str, int32 len) noexcept
{
	while(len-- > 0)
	{
		*str = CharOps::ToUpper(*str);

		// Compiler dependent order of read
		// *(str++) = CharOps::ToUpper(*str);

		++str;
	}
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE void TStringOps<CharType>::ToUpper(CharType(& str)[N]) noexcept
{
	ToUpper(str, N);
}

template<typename CharType>
void TStringOps<CharType>::ToLower(CharType* str) noexcept
{
	while(*str)
	{
		*str = CharOps::ToLower(*str);
		++str;
	}
}

template<typename CharType>
void TStringOps<CharType>::ToLower(CharType* str, int32 len) noexcept
{
	while(len-- > 0)
	{
		*str = CharOps::ToLower(*str);
		++str;
	}
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE void TStringOps<CharType>::ToLower(CharType(& str)[N]) noexcept
{
	return ToLower(str, N);
}

template<typename CharType>
void TStringOps<CharType>::Fill(CharType* str, CharType c, int32 len) noexcept
{
	while(len-- > 0) *str = c;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE void TStringOps<CharType>::Fill(CharType(& str)[N], CharType c) noexcept
{
	return Fill(str, c, N);
}

template<typename CharType>
void TStringOps<CharType>::Copy(CharType* dest, const CharType* src) noexcept
{
	while((*(dest++) = *(src++)));
}

template<typename CharType>
void TStringOps<CharType>::Copy(CharType* dest, const CharType* src, int32 len) noexcept
{
	if (len <= 0) return;
	while (len-- > 0) { *(dest++) = *(src++); }
	*dest = CharConstant::Null;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE void TStringOps<CharType>::Copy(CharType(& dest)[N], const CharType* src) noexcept
{
	Copy(dest, src, N);
}

template<typename CharType>
void TStringOps<CharType>::Concatenate(CharType* dest, const CharType* src) noexcept
{
	dest += Length(dest); // moves at '\0' character
	while(*src) { *(dest++) = *(src++); }
	*dest = CharConstant::Null; // ensure '\0' to the end
}

template<typename CharType>
void TStringOps<CharType>::Concatenate(CharType* dest, const CharType* src, int32 len) noexcept
{
	dest += Length(dest); // moves at '\0' character
	while(len-- > 0) { *(dest++) = *(src++); }
	*dest = CharConstant::Null; // ensure '\0' to the end
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE void TStringOps<CharType>::Concatenate(CharType* dest, const CharType(& src)[N]) noexcept
{
	return Concatenate(dest, src, N);
}