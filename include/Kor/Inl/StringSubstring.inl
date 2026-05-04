// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
TStringView<CharT> TStringView<CharT>::Sub(SizeType start, SizeType length) const noexcept
{
}

template<typename CharT>
TString<CharT> TString<CharT>::Sub(SizeType start, SizeType length) const noexcept
{
}

template<typename CharT>
TStringView<CharT> TString<CharT>::SubView(SizeType start, SizeType length) const noexcept
{
}

template<typename CharT>
TStringView<CharT> TStringView<CharT>::Left(SizeType count) const noexcept
{
}

template<typename CharT>
TString<CharT> TString<CharT>::Left(SizeType count) const noexcept
{
}

template<typename CharT>
TStringView<CharT> TString<CharT>::LeftView(SizeType count) const noexcept
{
}

template<typename CharT>
TStringView<CharT> TStringView<CharT>::Right(SizeType count) const noexcept
{
}

template<typename CharT>
TString<CharT> TString<CharT>::Right(SizeType count) const noexcept
{
}

template<typename CharT>
TStringView<CharT> TString<CharT>::RightView(SizeType count) const noexcept
{
}

template<typename CharT>
TStringView<CharT> TString<CharT>::View() const noexcept
{
}