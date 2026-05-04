// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
bool TStringView<CharT>::operator==(const TStringView& other) const noexcept
{
}

template<typename CharT>
bool TString<CharT>::operator==(const TString& other) const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::operator!=(const TStringView& other) const noexcept
{
}

template<typename CharT>
bool TString<CharT>::operator!=(const TString& other) const noexcept
{
}

template<typename CharT>
const typename TStringView<CharT>::CharType* TStringView<CharT>::operator*() const noexcept
{
}

template<typename CharT>
const typename TString<CharT>::CharType* TString<CharT>::operator*() const noexcept
{
}

template<typename CharT>
typename TString<CharT>::CharType* TString<CharT>::operator*() noexcept
{
}

template<typename CharT>
typename TStringView<CharT>::CharType TStringView<CharT>::operator[](SizeType idx) const noexcept
{
}

template<typename CharT>
typename TString<CharT>::CharType TString<CharT>::operator[](SizeType idx) const noexcept
{
}

template<typename CharT>
TString<CharT>& TString<CharT>::operator=(const TString& other) noexcept
{
}

template<typename CharT>
TString<CharT>& TString<CharT>::operator=(TString&& other) noexcept
{
}

template<typename CharT>
TString<CharT> TString<CharT>::operator+(const TString& other) const noexcept
{
}

template<typename CharT>
TString<CharT> TString<CharT>::operator+(TString&& other) const noexcept
{
}

template<typename CharT>
TString<CharT>& TString<CharT>::operator+=(const TString& other) noexcept
{
}

template<typename CharT>
TString<CharT>& TString<CharT>::operator+=(TString&& other) noexcept
{
}

template<typename CharT>
TString<CharT> TString<CharT>::operator/(const TString& other) const noexcept
{
}

template<typename CharT>
TString<CharT>& TString<CharT>::operator/=(const TString& other) noexcept
{
}