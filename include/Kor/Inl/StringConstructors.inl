// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
KOR_FORCEINLINE constexpr TStringView<CharT>::TStringView() noexcept
	: _data(&Constant::Null)
	, _len(0)
{}

template<typename CharT>
KOR_FORCEINLINE constexpr TStringView<CharT>::TStringView(Init::SZero) noexcept
	: _data(nullptr)
	, _len(0)
{}

template<typename CharT>
KOR_FORCEINLINE constexpr TStringView<CharT>::TStringView(Init::SNoInit) noexcept
	: _data(nullptr)
	, _len(KOR_INDEX_NONE)
{}

template<typename CharT>
KOR_FORCEINLINE constexpr TString<CharT>::TString() noexcept
	: _data(&Constant::Null, 1)
{}

template<typename CharT>
KOR_FORCEINLINE constexpr TString<CharT>::TString(Init::SZero) noexcept
	: _data(Init::Zero)
{}

template<typename CharT>
KOR_FORCEINLINE constexpr TString<CharT>::TString(Init::SNoInit) noexcept
	: _data(Init::NoInit)
{}

template<typename CharT>
KOR_FORCEINLINE TStringView<CharT>::TStringView(const CharType* text) noexcept
{
	if (text)
	{
		_data = text;
		_len = SOps::Length(text);
	}
	else
	{
		_data = &Constant::Null;
		_len = 0;
	}
}

template<typename CharT>
KOR_FORCEINLINE TString<CharT>::TString(const CharType* text) noexcept
{
	if (text)
	{
		_data = DataType(text, SOps::Length(text) + 1);
	}
	else
	{
		_data = DataType(&Constant::Null, 1);
	}
}

template<typename CharT>
KOR_FORCEINLINE TStringView<CharT>::TStringView(const CharType* text, SizeType length) noexcept
{
	if (text && length > 0)
	{
		_data = text;
		_len = length;
	}
	else
	{
		_data = &Constant::Null;
		_len = 0;
	}
}

template<typename CharT>
KOR_FORCEINLINE TString<CharT>::TString(const CharType* text, SizeType length) noexcept
{
	if (text && length > 0)
	{
		_data = DataType(text, length + 1);
		_data[length] = Constant::Null;
	}
	else
	{
		_data = DataType(&Constant::Null, 1);
	}
}

template<typename CharT>
template<TSize N>
KOR_FORCEINLINE TStringView<CharT>::TStringView(const CharType(& text)[N]) noexcept
	: _data(static_cast<const CharType*>(text))
	, _len(N - 1)
{}

template<typename CharT>
template<TSize N>
KOR_FORCEINLINE TString<CharT>::TString(const CharType(& text)[N]) noexcept
	: _data(static_cast<const CharType*>(text), N)
{}

template<typename CharT>
KOR_FORCEINLINE TString<CharT>::TString(SizeType length, CharType val) noexcept
{
	if (length > 0)
	{
		_data = DataType(length + 1);
		SOps::Fill(_data.GetData(), val, length);
		_data[length] = Constant::Null;
	}
	else
	{
		_data = DataType(&Constant::Null, 1);
	}
}

template<typename CharT>
KOR_FORCEINLINE const TString<CharT>& TString<CharT>::GetEmpty() noexcept
{
	static TString instance = {};
	return instance;
}
