// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
KOR_FORCEINLINE typename TStringView<CharT>::ConstIteratorType TStringView<CharT>::begin() const noexcept
{
	return _data;
}

template<typename CharT>
KOR_FORCEINLINE typename TString<CharT>::IteratorType TString<CharT>::begin() noexcept
{
	return _data.GetData();
}

template<typename CharT>
KOR_FORCEINLINE typename TString<CharT>::ConstIteratorType TString<CharT>::begin() const noexcept
{
	return _data.GetData();
}

template<typename CharT>
KOR_FORCEINLINE typename TStringView<CharT>::ConstIteratorType TStringView<CharT>::end() const noexcept
{
	return _data + _len;
}

template<typename CharT>
KOR_FORCEINLINE typename TString<CharT>::IteratorType TString<CharT>::end() noexcept
{
	return _data.GetData() + (_data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE typename TString<CharT>::ConstIteratorType TString<CharT>::end() const noexcept
{
	return _data.GetData() + (_data.GetNum() - 1);
}