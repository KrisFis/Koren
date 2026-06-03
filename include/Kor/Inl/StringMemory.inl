// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
KOR_FORCEINLINE TString<CharT> TString<CharT>::Copy() const noexcept
{
	return TString(*this);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Fill(SizeType length, CharType val) noexcept
{
	_data.Resize(length + 1);
	SMemory::FillTyped(_data.GetData(), val, length);
	_data[length] = Constant::Null;
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Reset() noexcept
{
	_data.Reset();
	_data.Add(Constant::Null);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Empty() noexcept
{
	_data.Empty();
	_data.Add(Constant::Null);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Reserve(SizeType num) noexcept
{
	_data.Reserve(num + 1); // +1 for null terminator
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::ShrinkToFit() noexcept
{
	_data.ShrinkToFit();
}