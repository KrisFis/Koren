// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
KOR_FORCEINLINE constexpr TStringView<CharT>::TStringView() noexcept
	: _data(&Constant::Null)
	, _len(0)
{}

template<typename CharT>
KOR_FORCEINLINE constexpr TString<CharT>::TString() noexcept
	: _data(&Constant::Null, 1)
{}

template<typename CharT>
KOR_FORCEINLINE TStringView<CharT>::TStringView(const CharType* text) noexcept
	: _data(text)
	, _len(SOps::Length(text))
{}

template<typename CharT>
KOR_FORCEINLINE TString<CharT>::TString(const CharType* text) noexcept
	: _data(text, SOps::Length(text) + 1) // +1 includes null terminator
{}

template<typename CharT>
KOR_FORCEINLINE constexpr TStringView<CharT>::TStringView(const CharType* text, SizeType length) noexcept
	: _data(text)
	, _len(length)
{}

template<typename CharT>
KOR_FORCEINLINE TString<CharT>::TString(const CharType* text, SizeType length) noexcept
	: _data(text, length + 1)
{}

template<typename CharT>
template<TSize N>
KOR_FORCEINLINE constexpr TStringView<CharT>::TStringView(const CharType(& text)[N]) noexcept
	: _data(text)
	, _len(N - 1) // -1 excludes null terminator
{}

template<typename CharT>
template<TSize N>
KOR_FORCEINLINE TString<CharT>::TString(const CharType(& text)[N]) noexcept
	: _data(text, N) // N includes null terminator
{}

template<typename CharT>
TString<CharT>::TString(SizeType length, CharType val) noexcept
	: _data(length + 1) // +1 includes null terminator
{
	SMemory::FillTyped(_data.GetData(), val, length);
	_data[length] = Constant::Null;
}

template<typename CharT>
KOR_FORCEINLINE constexpr const TString<CharT>& TString<CharT>::GetEmpty() noexcept
{
	static TString instance = {};
	return instance;
}
