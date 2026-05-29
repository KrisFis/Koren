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
KOR_FORCEINLINE_DEBUG TStringView<CharT>::TStringView(const CharType* text) noexcept
{
	KOR_ASSERT_DEBUG(text);
	_data = text;
	_len = SOps::Length(text);
}

template<typename CharT>
KOR_FORCEINLINE_DEBUG TString<CharT>::TString(const CharType* text) noexcept
{
	KOR_ASSERT_DEBUG(text);
	_data = DataType(text, SOps::Length(text) + 1); // +1 includes null terminator
}

template<typename CharT>
KOR_FORCEINLINE_DEBUG TStringView<CharT>::TStringView(const CharType* text, SizeType length) noexcept
{
	KOR_ASSERT_DEBUG(text && length > 0);

	_data = text;
	_len = length;
}

template<typename CharT>
KOR_FORCEINLINE_DEBUG TString<CharT>::TString(const CharType* text, SizeType length) noexcept
{
	KOR_ASSERT_DEBUG(text && length > 0);
	_data = DataType(text, length + 1);
}

template<typename CharT>
template<TSize N>
KOR_FORCEINLINE TStringView<CharT>::TStringView(const CharType(& text)[N]) noexcept
	: _data(text)
	, _len(N - 1) // -1 excludes null terminator
{}

template<typename CharT>
template<TSize N>
KOR_FORCEINLINE TString<CharT>::TString(const CharType(& text)[N]) noexcept
	: _data(text, N) // N includes null terminator
{}

template<typename CharT>
KOR_FORCEINLINE_DEBUG TString<CharT>::TString(TString<CharT>::SizeType length, CharType val) noexcept
	: _data(length + 1) // +1 includes null terminator
{
	KOR_ASSERT_DEBUG(length > 0);

	SOps::Fill(_data.GetData(), val, length);
	_data[length] = Constant::Null;
}

template<typename CharT>
KOR_FORCEINLINE const TString<CharT>& TString<CharT>::GetEmpty() noexcept
{
	static TString instance = {};
	return instance;
}
