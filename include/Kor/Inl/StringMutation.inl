// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
void TString<CharT>::Append(const TString& other) noexcept
{
}

template<typename CharT>
void TString<CharT>::Append(TString&& other) noexcept
{
}

template<typename CharT>
void TString<CharT>::Append(const CharType* other, SizeType num) noexcept
{
}
template<typename CharT>
void TString<CharT>::ToUpper() noexcept
{
}

template<typename CharT>
void TString<CharT>::ToLower() noexcept
{
}

template<typename CharT>
void TString<CharT>::TrimStart() noexcept
{
}

template<typename CharT>
void TString<CharT>::TrimEnd() noexcept
{
}

template<typename CharT>
void TString<CharT>::Trim() noexcept
{
}

template<typename CharT>
void TString<CharT>::ChopRight(SizeType idx) noexcept
{
}

template<typename CharT>
void TString<CharT>::ChopLeft(SizeType idx) noexcept
{
}

template<typename CharT>
void TString<CharT>::ChopRange(SizeType firstIdx, SizeType secondIdx) noexcept
{
}

template<typename CharT>
void TString<CharT>::Insert(SizeType idx, const TString& other) noexcept
{
}

template<typename CharT>
void TString<CharT>::Remove(SizeType idx, SizeType count) noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
void TString<CharT>::Replace(const TString& from, const TString& to) noexcept
{
}

template<typename CharT>
void TString<CharT>::Replace(const TString& from, const TString& to, ESearchCase searchCase) noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
void TString<CharT>::Replace(CharType from, CharType to) noexcept
{
}

template<typename CharT>
void TString<CharT>::Replace(CharType from, CharType to, ESearchCase searchCase) noexcept
{
}