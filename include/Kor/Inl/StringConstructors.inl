// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
constexpr TStringView<CharT>::TStringView(const CharType* text) noexcept
{
}

template<typename CharT>
TString<CharT>::TString(const CharType* text) noexcept
{
}

template<typename CharT>
constexpr TStringView<CharT>::TStringView(const CharType* text, SizeType length) noexcept
{
}

template<typename CharT>
TString<CharT>::TString(const CharType* text, SizeType length) noexcept
{
}

template<typename CharT>
template<TSize N>
constexpr TStringView<CharT>::TStringView(const CharType(& text)[N]) noexcept
{
}

template<typename CharT>
template<TSize N>
TString<CharT>::TString(const CharType(& text)[N]) noexcept
{
}

template<typename CharT>
TString<CharT>::TString(SizeType length, CharType val) noexcept
{
}