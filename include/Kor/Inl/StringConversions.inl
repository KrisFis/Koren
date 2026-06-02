// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
TString<CharT> TString<CharT>::FromInt(int64 value, int32 base) noexcept
{
	TString result(Init::Zero);
	result._data.Resize(SMemory::BUFFER_SIZE_INT64_MAX);

	const int32 convLen = SOps::FromInt(*result._data, value, SMemory::BUFFER_SIZE_INT64_MAX, base);
	KOR_ASSERT_DEBUG(convLen > 0);

	result._data.Resize(convLen + 1);
	result._data[convLen] = Constant::Null;

	return result;
}

template<typename CharT>
TString<CharT> TString<CharT>::FromUInt(uint64 value, int32 base) noexcept
{
	TString result(Init::Zero);
	result._data.Resize(SMemory::BUFFER_SIZE_INT64_MAX);

	const int32 convLen = SOps::FromUInt(*result._data, value, SMemory::BUFFER_SIZE_INT64_MAX, base);
	KOR_ASSERT_DEBUG(convLen > 0);

	result._data.Resize(convLen + 1);
	result._data[convLen] = Constant::Null;

	return result;
}

template<typename CharT>
KOR_FORCEINLINE int64 TStringView<CharT>::ToInt(int32 base) const noexcept
{
	return SOps::ToInt(_data, base);
}

template<typename CharT>
KOR_FORCEINLINE int64 TString<CharT>::ToInt(int32 base) const noexcept
{
	return SOps::ToInt(*_data, base);
}

template<typename CharT>
KOR_FORCEINLINE uint64 TStringView<CharT>::ToUInt(int32 base) const noexcept
{
	return SOps::ToUInt(_data, base);
}

template<typename CharT>
KOR_FORCEINLINE uint64 TString<CharT>::ToUInt(int32 base) const noexcept
{
	return SOps::ToUInt(*_data, base);
}

template<typename CharT>
template<EFloatFormat Format>
TString<CharT> TString<CharT>::FromFloat(double value, uint8 precision) noexcept
{
	TString result(Init::Zero);
	result._data.Resize(SMemory::BUFFER_SIZE_DOUBLE_MAX);

	const int32 convLen = SOps::template FromFloat<Format>(*result._data, value, SMemory::BUFFER_SIZE_DOUBLE_MAX, precision);
	KOR_ASSERT_DEBUG(convLen > 0);

	result._data.Resize(convLen + 1);
	result._data[convLen] = Constant::Null;

	return result;
}

template<typename CharT>
TString<CharT> TString<CharT>::FromFloat(double value, uint8 precision, EFloatFormat format) noexcept
{
	switch (format)
	{
		case EFloatFormat::Fixed:
			return FromFloat<EFloatFormat::Fixed>(value, precision);
		case EFloatFormat::Scientific:
			return FromFloat<EFloatFormat::Scientific>(value, precision);
		default:
			return FromFloat<EFloatFormat::Auto>(value, precision);
	}
}

template<typename CharT>
KOR_FORCEINLINE double TStringView<CharT>::ToFloat() const noexcept
{
	return SOps::ToFloat(_data);
}

template<typename CharT>
KOR_FORCEINLINE double TString<CharT>::ToFloat() const noexcept
{
	return SOps::ToFloat(*_data);
}

template<typename CharT>
template<typename OtherCharType>
TString<CharT> TString<CharT>::ConvertFrom(const OtherCharType* str, SizeType length) noexcept
{
	const SizeType convLen = TStringOps<OtherCharType>::template ConvertedLength<CharT>(str, length);
	KOR_ASSERT_DEBUG(convLen > 0);

	TString result(Init::Zero);
	result._data.Resize(convLen + 1);

	TStringOps<OtherCharType>::template Convert<CharT>(str, *result._data, length);
	result._data[convLen] = Constant::Null;

	return result;
}

template<typename CharT>
template<typename OtherCharType>
KOR_FORCEINLINE TString<CharT> TString<CharT>::ConvertFrom(const TString<OtherCharType>& str) noexcept
{
	return ConvertFrom(*str._data, str._data.GetNum() - 1);
}

template<typename CharT>
template<typename OtherCharType>
KOR_FORCEINLINE TString<OtherCharType> TString<CharT>::ConvertTo() const noexcept
{
	return TString<OtherCharType>::ConvertFrom(*_data, _data.GetNum() - 1);
}