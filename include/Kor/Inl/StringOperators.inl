// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.
#pragma once // silence tooling

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::operator==(const TStringView& other) const noexcept
{
	return Equals<ESearchCase::Sensitive>(other);
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::operator==(const TString& other) const noexcept
{
	return Equals<ESearchCase::Sensitive>(other);
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::operator!=(const TStringView& other) const noexcept
{
	return !(*this == other);
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::operator!=(const TString& other) const noexcept
{
	return !(*this == other);
}

template<typename CharT>
KOR_FORCEINLINE const CharT* TStringView<CharT>::operator*() const noexcept
{
	return _data;
}

template<typename CharT>
KOR_FORCEINLINE const CharT* TString<CharT>::operator*() const noexcept
{
	return *_data;
}

template<typename CharT>
KOR_FORCEINLINE CharT* TString<CharT>::operator*() noexcept
{
	return *_data;
}

template<typename CharT>
KOR_FORCEINLINE_DEBUG CharT TStringView<CharT>::operator[](SizeType idx) const noexcept
{
	KOR_ASSERT_DEBUG(idx >= 0 && idx < _len);
	return _data[idx];
}

template<typename CharT>
KOR_FORCEINLINE_DEBUG CharT TString<CharT>::operator[](SizeType idx) const noexcept
{
	KOR_ASSERT_DEBUG(idx >= 0 && idx < _data.GetNum() - 1);
	return _data[idx];
}

template<typename CharT>
KOR_FORCEINLINE TString<CharT>& TString<CharT>::operator=(const TString& other) noexcept
{
	if (this != &other)
	{
		_data = other._data;
	}
	return *this;
}

template<typename CharT>
KOR_FORCEINLINE TString<CharT>& TString<CharT>::operator=(TString&& other) noexcept
{
	if (this != &other)
	{
		_data = Move(other._data);
	}
	return *this;
}

template<typename CharT>
TString<CharT> TString<CharT>::operator+(const TString& other) const noexcept
{
	TString result(Init::Zero);
	const int32 thisLen = _data.GetNum() - 1;
	const int32 otherLen = other._data.GetNum() - 1;

	result._data.Resize(thisLen + otherLen + 1);
	SOps::Copy(*result._data, *_data, thisLen);
	SOps::Copy(*result._data + thisLen, *other._data, otherLen);
	result._data[thisLen + otherLen] = Constant::Null;

	return result;
}

template<typename CharT>
KOR_FORCEINLINE TString<CharT> TString<CharT>::operator+(TString&& other) const noexcept
{
	return operator+(other);
}

template<typename CharT>
TString<CharT>& TString<CharT>::operator+=(const TString& other) noexcept
{
	const int32 thisLen = _data.GetNum() - 1;
	const int32 otherLen = other._data.GetNum() - 1;

	_data.Resize(thisLen + otherLen + 1);
	SOps::Copy(*_data + thisLen, *other._data, otherLen);
	_data[thisLen + otherLen] = Constant::Null;

	return *this;
}

template<typename CharT>
KOR_FORCEINLINE TString<CharT>& TString<CharT>::operator+=(TString&& other) noexcept
{
	return operator+=(other);
}

template<typename CharT>
TString<CharT> TString<CharT>::operator/(const TString& other) const noexcept
{
	const int32 thisLen = _data.GetNum() - 1;
	const int32 otherLen = other._data.GetNum() - 1;

	TString result(Init::Zero);
	result._data.Resize(thisLen + 1 + otherLen + 1);

	SOps::Copy(*result._data, *_data, thisLen);
	result._data[thisLen] = Constant::Slash;

	SOps::Copy(* result._data + thisLen + 1, *other._data, otherLen);
	result._data[thisLen + 1 + otherLen] = Constant::Null;

	return result;
}

template<typename CharT>
TString<CharT>& TString<CharT>::operator/=(const TString& other) noexcept
{
	const int32 thisLen = _data.GetNum() - 1;
	const int32 otherLen = other._data.GetNum() - 1;

	_data.Resize(thisLen + 1 + otherLen + 1);

	_data[thisLen] = Constant::Slash;
	SOps::Copy(*_data + thisLen + 1, *other._data, otherLen);
	_data[thisLen + 1 + otherLen] = Constant::Null;

	return *this;
}