// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
typename TStringView<CharT>::ConstIteratorType TStringView<CharT>::begin() const noexcept
{
}

template<typename CharT>
typename TString<CharT>::IteratorType TString<CharT>::begin() noexcept
{
}

template<typename CharT>
typename TString<CharT>::ConstIteratorType TString<CharT>::begin() const noexcept
{
}

template<typename CharT>
typename TStringView<CharT>::ConstIteratorType TStringView<CharT>::end() const noexcept
{
}

template<typename CharT>
typename TString<CharT>::IteratorType TString<CharT>::end() noexcept
{
}

template<typename CharT>
typename TString<CharT>::ConstIteratorType TString<CharT>::end() const noexcept
{
}