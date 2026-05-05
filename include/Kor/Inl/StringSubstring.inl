// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
TStringView<CharT> TStringView<CharT>::Sub(SizeType start, SizeType length) const noexcept
{
	KOR_ASSERT_DEBUG(start + length <= _len);
	return TStringView(_data + start, length);
}

template<typename CharT>
TString<CharT> TString<CharT>::Sub(SizeType start, SizeType length) const noexcept
{
	const SizeType len = _data.GetNum() - 1;
	KOR_ASSERT_DEBUG(start + length <= len);
	return TString(_data.GetData() + start, length);
}

template<typename CharT>
TStringView<CharT> TString<CharT>::SubView(SizeType start, SizeType length) const noexcept
{
	const SizeType len = _data.GetNum() - 1;
	KOR_ASSERT_DEBUG(start + length <= len);
	return TStringView<CharT>(_data.GetData() + start, length);
}

template<typename CharT>
TStringView<CharT> TStringView<CharT>::Left(SizeType count) const noexcept
{
	KOR_ASSERT_DEBUG(count <= _len);
	return TStringView(_data, count);
}

template<typename CharT>
TString<CharT> TString<CharT>::Left(SizeType count) const noexcept
{
	KOR_ASSERT_DEBUG(count <= _data.GetNum() - 1);
	return TString(_data.GetData(), count);
}

template<typename CharT>
TStringView<CharT> TString<CharT>::LeftView(SizeType count) const noexcept
{
	KOR_ASSERT_DEBUG(count <= _data.GetNum() - 1);
	return TStringView<CharT>(_data.GetData(), count);
}

template<typename CharT>
TStringView<CharT> TStringView<CharT>::Right(SizeType count) const noexcept
{
	KOR_ASSERT_DEBUG(count <= _len);
	return TStringView(_data + (_len - count), count);
}

template<typename CharT>
TString<CharT> TString<CharT>::Right(SizeType count) const noexcept
{
	const SizeType len = _data.GetNum() - 1;
	KOR_ASSERT_DEBUG(count <= len);
	return TString(_data.GetData() + (len - count), count);
}

template<typename CharT>
TStringView<CharT> TString<CharT>::RightView(SizeType count) const noexcept
{
	const SizeType len = _data.GetNum() - 1;
	KOR_ASSERT_DEBUG(count <= len);
	return TStringView<CharT>(_data.GetData() + (len - count), count);
}

template<typename CharT>
TStringView<CharT> TString<CharT>::View() const noexcept
{
	return TStringView<CharT>(_data.GetData(), _data.GetNum() - 1);
}