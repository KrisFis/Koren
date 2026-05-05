// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::IsAscii() const noexcept
{
	return SOps::IsAscii(_data, _len);
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::IsAscii() const noexcept
{
	return SOps::IsAscii(*_data, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::IsNumeric() const noexcept
{
	return SOps::IsNumeric(_data, _len);
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::IsNumeric() const noexcept
{
	return SOps::IsNumeric(*_data, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::IsWhitespace() const noexcept
{
	return SOps::IsWhitespace(_data, _len);
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::IsWhitespace() const noexcept
{
	return SOps::IsWhitespace(*_data, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::IsUpper() const noexcept
{
	return SOps::IsUpper(_data, _len);
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::IsUpper() const noexcept
{
	return SOps::IsUpper(*_data, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::IsLower() const noexcept
{
	return SOps::IsLower(_data, _len);
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::IsLower() const noexcept
{
	return SOps::IsLower(*_data, _data.GetNum() - 1);
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE int32 TStringView<CharT>::Compare(const TStringView& other) const noexcept
{
	return SOps::Compare<Case>(_data, other._data, _len);
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE int32 TString<CharT>::Compare(const TString& other) const noexcept
{
	return SOps::Compare<Case>(*_data, *other._data, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE int32 TStringView<CharT>::Compare(const TStringView& other, ESearchCase searchCase) const noexcept
{
	return SOps::Compare(_data, other._data, _len, searchCase);
}

template<typename CharT>
KOR_FORCEINLINE int32 TString<CharT>::Compare(const TString& other, ESearchCase searchCase) const noexcept
{
	return SOps::Compare(*_data, *other._data, _data.GetNum() - 1, searchCase);
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE bool TStringView<CharT>::Equals(const TStringView& other) const noexcept
{
	return Compare<Case>(other) == 0;
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE bool TString<CharT>::Equals(const TString& other) const noexcept
{
	return Compare<Case>(other) == 0;
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::Equals(const TStringView& other, ESearchCase searchCase) const noexcept
{
	return Compare(other, searchCase) == 0;
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::Equals(const TString& other, ESearchCase searchCase) const noexcept
{
	return Compare(other, searchCase) == 0;
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE_DEBUG bool TStringView<CharT>::StartsWith(const TStringView& val) const noexcept
{
	if (val._len > _len) return false;
	return SOps::Compare<Case>(_data, val._data, val._len) == 0;
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE_DEBUG bool TString<CharT>::StartsWith(const TString& val) const noexcept
{
	const int32 len = _data.GetNum() - 1;
	const int32 valLen = val._data.GetNum() - 1;

	if (valLen > len) return false;
	return SOps::Compare<Case>(*_data, *val._data, valLen) == 0;
}

template<typename CharT>
KOR_FORCEINLINE_DEBUG bool TStringView<CharT>::StartsWith(const TStringView& val, ESearchCase searchCase) const noexcept
{
	if (val._len > _len) return false;
	return SOps::Compare(_data, val._data, val._len, searchCase) == 0;
}

template<typename CharT>
KOR_FORCEINLINE_DEBUG bool TString<CharT>::StartsWith(const TString& val, ESearchCase searchCase) const noexcept
{
	const int32 len = _data.GetNum() - 1;
	const int32 valLen = val._data.GetNum() - 1;

	if (valLen > len) return false;
	return SOps::Compare(*_data, *val._data, valLen, searchCase) == 0;
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE_DEBUG bool TStringView<CharT>::EndsWith(const TStringView& val) const noexcept
{
	if (val._len > _len) return false;
	return SOps::Compare<Case>(_data + (_len - val._len), val._data, val._len) == 0;
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE_DEBUG bool TString<CharT>::EndsWith(const TString& val) const noexcept
{
	const int32 len = _data.GetNum() - 1;
	const int32 valLen = val._data.GetNum() - 1;

	if (valLen > len) return false;
	return SOps::Compare<Case>(*_data + (len - valLen), *val._data, valLen) == 0;
}

template<typename CharT>
KOR_FORCEINLINE_DEBUG bool TStringView<CharT>::EndsWith(const TStringView& val, ESearchCase searchCase) const noexcept
{
	if (val._len > _len) return false;
	return SOps::Compare(_data + (_len - val._len), val._data, val._len, searchCase) == 0;
}

template<typename CharT>
KOR_FORCEINLINE_DEBUG bool TString<CharT>::EndsWith(const TString& val, ESearchCase searchCase) const noexcept
{
	const int32 len = _data.GetNum() - 1;
	const int32 valLen = val._data.GetNum() - 1;

	if (valLen > len) return false;
	return SOps::Compare(*_data + (len - valLen), *val._data, valLen, searchCase) == 0;
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
KOR_FORCEINLINE bool TStringView<CharT>::Contains(const TStringView& val) const noexcept
{
	return SOps::Find<Case, Dir>(_data, val._data, _len, val._len) != KOR_INDEX_NONE;
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
KOR_FORCEINLINE bool TString<CharT>::Contains(const TString& val) const noexcept
{
	const int32 len = _data.GetNum() - 1;
	const int32 valLen = val._data.GetNum() - 1;

	return SOps::Find<Case, Dir>(*_data, *val._data, len, valLen) != KOR_INDEX_NONE;
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::Contains(const TStringView& val, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
	return SOps::Find(_data, val._data, _len, val._len, searchCase, searchDir) != KOR_INDEX_NONE;
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::Contains(const TString& val, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
	const int32 len = _data.GetNum() - 1;
	const int32 valLen = val._data.GetNum() - 1;

	return SOps::Find(*_data, *val._data, len, valLen, searchCase, searchDir) != KOR_INDEX_NONE;
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE bool TStringView<CharT>::ContainsAt(const TStringView& val, SizeType index) const noexcept
{
	if (index > _len) return false;
	return SOps::Compare<Case>(_data + index, val._data, val._len) == 0;
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE bool TString<CharT>::ContainsAt(const TString& val, SizeType index) const noexcept
{
	const int32 len = _data.GetNum() - 1;
	const int32 valLen = val._data.GetNum() - 1;

	if (index > len) return false;
	return SOps::Compare<Case>(*_data + index, *val._data, valLen) == 0;
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::ContainsAt(const TStringView& val, SizeType index, ESearchCase searchCase) const noexcept
{
	if (index > _len) return false;
	return SOps::Compare(_data + index, val._data, val._len, searchCase) == 0;
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::ContainsAt(const TString& val, SizeType index, ESearchCase searchCase) const noexcept
{
	const int32 len = _data.GetNum() - 1;
	const int32 valLen = val._data.GetNum() - 1;

	if (index > len) return false;
	return SOps::Compare(*_data + index, *val._data, valLen, searchCase) == 0;
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
KOR_FORCEINLINE typename TStringView<CharT>::SizeType TStringView<CharT>::Find(const TStringView& val) const noexcept
{
	return SOps::Find<Case, Dir>(_data, val._data, _len, val._len);
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
KOR_FORCEINLINE typename TString<CharT>::SizeType TString<CharT>::Find(const TString& val) const noexcept
{
	return SOps::Find<Case, Dir>(*_data, *val._data, _data.GetNum() - 1, val._data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE typename TStringView<CharT>::SizeType TStringView<CharT>::Find(const TStringView& val, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
	return SOps::Find(_data, val, _len, val._len, searchCase, searchDir);
}

template<typename CharT>
KOR_FORCEINLINE typename TString<CharT>::SizeType TString<CharT>::Find(const TString& val, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
	return SOps::Find(*_data, *val._data, _data.GetNum() - 1, val._data.GetNum() - 1, searchCase, searchDir);
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
KOR_FORCEINLINE typename TStringView<CharT>::SizeType TStringView<CharT>::Find(CharType c) const noexcept
{
	return SOps::Find<Case, Dir>(_data, c, _len);
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
KOR_FORCEINLINE typename TString<CharT>::SizeType TString<CharT>::Find(CharType c) const noexcept
{
	return SOps::Find<Case, Dir>(*_data, c, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE typename TStringView<CharT>::SizeType TStringView<CharT>::Find(CharType c, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
	return SOps::Find(_data, c, _len, searchCase, searchDir);
}

template<typename CharT>
KOR_FORCEINLINE typename TString<CharT>::SizeType TString<CharT>::Find(CharType c, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
	return SOps::Find(*_data, c, _data.GetNum() - 1, searchCase, searchDir);
}