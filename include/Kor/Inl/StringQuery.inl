// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
bool TStringView<CharT>::IsAscii() const noexcept
{
}

template<typename CharT>
bool TString<CharT>::IsAscii() const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::IsNumeric() const noexcept
{
}

template<typename CharT>
bool TString<CharT>::IsNumeric() const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::IsWhitespace() const noexcept
{
}

template<typename CharT>
bool TString<CharT>::IsWhitespace() const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::IsUpper() const noexcept
{
}

template<typename CharT>
bool TString<CharT>::IsUpper() const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::IsLower() const noexcept
{
}

template<typename CharT>
bool TString<CharT>::IsLower() const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
int32 TStringView<CharT>::Compare(const TStringView& other) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
int32 TString<CharT>::Compare(const TString& other) const noexcept
{
}

template<typename CharT>
int32 TStringView<CharT>::Compare(const TStringView& other, ESearchCase searchCase) const noexcept
{
}

template<typename CharT>
int32 TString<CharT>::Compare(const TString& other, ESearchCase searchCase) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
bool TStringView<CharT>::Equals(const TStringView& other) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
bool TString<CharT>::Equals(const TString& other) const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::Equals(const TStringView& other, ESearchCase searchCase) const noexcept
{
}

template<typename CharT>
bool TString<CharT>::Equals(const TString& other, ESearchCase searchCase) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
bool TStringView<CharT>::StartsWith(const TStringView& val) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
bool TString<CharT>::StartsWith(const TString& val) const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::StartsWith(const TStringView& val, ESearchCase searchCase) const noexcept
{
}

template<typename CharT>
bool TString<CharT>::StartsWith(const TString& val, ESearchCase searchCase) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
bool TStringView<CharT>::EndsWith(const TStringView& val) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case>
bool TString<CharT>::EndsWith(const TString& val) const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::EndsWith(const TStringView& val, ESearchCase searchCase) const noexcept
{
}

template<typename CharT>
bool TString<CharT>::EndsWith(const TString& val, ESearchCase searchCase) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
bool TStringView<CharT>::Contains(const TStringView& val) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
bool TString<CharT>::Contains(const TString& val) const noexcept
{
}

template<typename CharT>
bool TString<CharT>::Contains(const TString& val, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::Contains(const TStringView& val, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
bool TStringView<CharT>::ContainsFrom(const TStringView& val, SizeType index) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
bool TString<CharT>::ContainsFrom(const TString& val, SizeType index) const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::ContainsFrom(const TStringView& val, SizeType index, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
bool TString<CharT>::ContainsFrom(const TString& val, SizeType index, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
typename TStringView<CharT>::SizeType TStringView<CharT>::Find(const TStringView& val) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
typename TString<CharT>::SizeType TString<CharT>::Find(const TString& val) const noexcept
{
}

template<typename CharT>
typename TStringView<CharT>::SizeType TStringView<CharT>::Find(const TStringView& val, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
typename TString<CharT>::SizeType TString<CharT>::Find(const TString& val, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
typename TStringView<CharT>::SizeType TStringView<CharT>::Find(CharType c) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
typename TString<CharT>::SizeType TString<CharT>::Find(CharType c) const noexcept
{
}

template<typename CharT>
typename TStringView<CharT>::SizeType TStringView<CharT>::Find(CharType c, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
typename TString<CharT>::SizeType TString<CharT>::Find(CharType c, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
typename TStringView<CharT>::SizeType TStringView<CharT>::FindFrom(const TStringView& val, SizeType index) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
typename TString<CharT>::SizeType TString<CharT>::FindFrom(const TString& val, SizeType index) const noexcept
{
}

template<typename CharT>
typename TStringView<CharT>::SizeType TStringView<CharT>::FindFrom(const TStringView& val, SizeType index, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
typename TString<CharT>::SizeType TString<CharT>::FindFrom(const TString& val, SizeType index, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
typename TStringView<CharT>::SizeType TStringView<CharT>::FindFrom(CharType c, SizeType index) const noexcept
{
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
typename TString<CharT>::SizeType TString<CharT>::FindFrom(CharType c, SizeType index) const noexcept
{
}

template<typename CharT>
typename TStringView<CharT>::SizeType TStringView<CharT>::FindFrom(CharType c, SizeType index, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}

template<typename CharT>
typename TString<CharT>::SizeType TString<CharT>::FindFrom(CharType c, SizeType index, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
}