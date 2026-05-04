// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
bool TStringView<CharT>::Split(const TStringView& delim, TStringView* outLeft, TStringView* outRight) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
bool TString<CharT>::Split(const TString& delim, TString* outLeft, TString* outRight) const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::Split(const TStringView& delim, TStringView* outLeft, TStringView* outRight, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
bool TString<CharT>::Split(const TString& delim, TString* outLeft, TString* outRight, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
TArray<TStringView<CharT>> TStringView<CharT>::SplitToArray(const TStringView& delim, bool discardEmpty) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
TArray<TString<CharT>> TString<CharT>::SplitToArray(const TString& delim, bool discardEmpty) const noexcept
{
}

template<typename CharT>
TArray<TStringView<CharT>> TStringView<CharT>::SplitToArray(const TStringView& delim, bool discardEmpty, ESearchCase searchCase) const noexcept
{
}

template<typename CharT>
TArray<TString<CharT>> TString<CharT>::SplitToArray(const TString& delim, bool discardEmpty, ESearchCase searchCase) const noexcept
{
}