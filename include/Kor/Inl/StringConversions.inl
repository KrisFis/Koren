// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
int64 TStringView<CharT>::ToInt(int32 base) const noexcept
{
}

template<typename CharT>
int64 TString<CharT>::ToInt(int32 base) const noexcept
{
}

template<typename CharT>
uint64 TStringView<CharT>::ToUInt(int32 base) const noexcept
{
}

template<typename CharT>
uint64 TString<CharT>::ToUInt(int32 base) const noexcept
{
}

template<typename CharT>
double TStringView<CharT>::ToFloat() const noexcept
{
}

template<typename CharT>
double TString<CharT>::ToFloat() const noexcept
{
}

template<typename CharT>
template<typename OtherCharType>
TString<CharT> TString<CharT>::ConvertFrom(const OtherCharType* str) noexcept
{
}

template<typename CharT>
template<typename OtherCharType>
TString<CharT> TString<CharT>::ConvertFrom(const OtherCharType* str, SizeType length) noexcept
{
}

template<typename CharT>
template<typename OtherCharType, TSize N>
TString<CharT> TString<CharT>::ConvertFrom(const OtherCharType(& str)[N]) noexcept
{
}

template<typename CharT>
template<typename OtherCharType>
TString<CharT> TString<CharT>::ConvertFrom(const TString<OtherCharType>& str) noexcept
{
}

template<typename CharT>
template<typename OtherCharType>
TString<OtherCharType> TString<CharT>::ConvertTo() const noexcept
{
}

template<typename CharT>
TString<CharT> TString<CharT>::FromInt(int64 val) noexcept
{
}

template<typename CharT>
TString<CharT> TString<CharT>::FromUInt(uint64 val) noexcept
{
}

template<typename CharT>
template<EFloatFormat Format>
TString<CharT> TString<CharT>::FromFloat(double val, uint8 precision) noexcept
{
}

template<typename CharT>
TString<CharT> TString<CharT>::FromFloat(double val, uint8 precision, EFloatFormat format) noexcept
{
}