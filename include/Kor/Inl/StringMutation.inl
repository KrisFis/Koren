// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Append(const TString& other) noexcept
{
	_data.Pop(); // remove null terminator
	_data.Append(other._data); // includes other's null terminator
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Append(TString&& other) noexcept
{
	_data.Pop(); // remove null terminator
	_data.Append(Move(other._data)); // includes other's null terminator
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Append(const CharType* other, SizeType num) noexcept
{
	_data.Pop(); // remove null terminator
	_data.Append(other, num);
	_data.Add(Constant::Null);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::ToUpper() noexcept
{
	return SOps::ToUpper(*_data, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::ToLower() noexcept
{
	return SOps::ToLower(*_data, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::TrimStart() noexcept
{
	const int32 count = SOps::template CountWhitespaces<ESearchDir::Forward>(*_data, _data.GetNum() - 1);
	return ChopLeft(count);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::TrimEnd() noexcept
{
	const int32 count = SOps::template CountWhitespaces<ESearchDir::Backward>(*_data, _data.GetNum() - 1);
	return ChopRight(count);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Trim() noexcept
{
	TrimStart();
	TrimEnd();
}

template<typename CharT>
void TString<CharT>::ChopRight(SizeType idx) noexcept
{
	// Remove everything from idx to end (excluding null terminator)
	KOR_ASSERT_DEBUG(idx >= 0 && idx < _data.GetNum() - 1);
	const SizeType count = _data.GetNum() - 1 - idx;
	_data.RemoveAt(idx, count);
	_data[_data.GetNum() - 1] = Constant::Null;
}

template<typename CharT>
void TString<CharT>::ChopLeft(SizeType idx) noexcept
{
	// Remove everything before idx
	KOR_ASSERT_DEBUG(idx >= 0 && idx < _data.GetNum() - 1);
	_data.RemoveAt(0, idx);
	// null terminator survives since we removed from the front
}

template<typename CharT>
void TString<CharT>::ChopRange(SizeType firstIdx, SizeType secondIdx) noexcept
{
	KOR_ASSERT_DEBUG(firstIdx >= 0 && secondIdx >= firstIdx && secondIdx < _data.GetNum() - 1);
	_data.RemoveAt(firstIdx, secondIdx - firstIdx + 1);
	_data[_data.GetNum() - 1] = Constant::Null;
}

template<typename CharT>
void TString<CharT>::Insert(SizeType idx, const TString& other) noexcept
{
	KOR_ASSERT_DEBUG(idx >= 0 && idx < _data.GetNum() - 1);
	// Insert without the other's null terminator
	_data.Insert(idx, *other._data, other._data.GetNum() - 1);
	_data[_data.GetNum() - 1] = Constant::Null;
}

template<typename CharT>
void TString<CharT>::Remove(SizeType idx, SizeType count) noexcept
{
	KOR_ASSERT_DEBUG(idx >= 0 && idx + count <= _data.GetNum() - 1);
	_data.RemoveAt(idx, count);
	_data[_data.GetNum() - 1] = Constant::Null;
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE_DEBUG void TString<CharT>::Replace(const TString& from, const TString& to) noexcept
{
	SOps::template Replace<Case>(*_data, *from._data, *to._data, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Replace(const TString& from, const TString& to, ESearchCase searchCase) noexcept
{
	if (searchCase == ESearchCase::Sensitive) Replace<ESearchCase::Sensitive>(from, to);
	else Replace<ESearchCase::Insensitive>(from, to);
}

template<typename CharT>
template<ESearchCase Case>
KOR_FORCEINLINE void TString<CharT>::Replace(CharType from, CharType to) noexcept
{
	SOps::template Replace<Case>(*_data, from, to, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE void TString<CharT>::Replace(CharType from, CharType to, ESearchCase searchCase) noexcept
{
	if (searchCase == ESearchCase::Sensitive) Replace<ESearchCase::Sensitive>(from, to);
	else return Replace<ESearchCase::Insensitive>(from, to);
}