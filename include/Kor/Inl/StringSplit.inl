// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
bool TStringView<CharT>::Split(const TStringView& delim, TStringView* outLeft, TStringView* outRight) const noexcept
{
	const SizeType thisLen = _len;
	const SizeType delimLen = delim._len;

	const SizeType foundIdx = SOps::template Find<Case, Dir>(_data, delim._data, thisLen, delimLen);

	if (foundIdx == KOR_INDEX_NONE)
	{
		if (outLeft) *outLeft = *this;
		if (outRight) *outRight = TStringView();
		return false;
	}

	if (outLeft) *outLeft = TStringView(_data, foundIdx);
	if (outRight) *outRight = TStringView(_data + foundIdx + delimLen, thisLen - foundIdx - delimLen);
	return true;
}

template<typename CharT>
template<ESearchCase Case, ESearchDir Dir>
bool TString<CharT>::Split(const TString& delim, TString* outLeft, TString* outRight) const noexcept
{
	const SizeType thisLen = _data.GetNum() - 1;
	const SizeType delimLen = delim._data.GetNum() - 1;

	const SizeType foundIdx = SOps::template Find<Case, Dir>(*_data, *delim._data, thisLen, delimLen);

	if (foundIdx == KOR_INDEX_NONE)
	{
		if (outLeft) *outLeft = *this;
		if (outRight) *outRight = TString();
		return false;
	}

	if (outLeft) *outLeft = TString(*_data, foundIdx);
	if (outRight) *outRight = TString(*_data + foundIdx + delimLen, thisLen - foundIdx - delimLen);
	return true;
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::Split(const TStringView& delim, TStringView* outLeft, TStringView* outRight, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
	if (searchCase == ESearchCase::Sensitive)
	{
		return searchDir == ESearchDir::Forward
			? Split<ESearchCase::Sensitive, ESearchDir::Forward>(delim, outLeft, outRight)
			: Split<ESearchCase::Sensitive, ESearchDir::Backward>(delim, outLeft, outRight);
	}
	else
	{
		return searchDir == ESearchDir::Forward
			? Split<ESearchCase::Insensitive, ESearchDir::Forward>(delim, outLeft, outRight)
			: Split<ESearchCase::Insensitive, ESearchDir::Backward>(delim, outLeft, outRight);
	}
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::Split(const TString& delim, TString* outLeft, TString* outRight, ESearchCase searchCase, ESearchDir searchDir) const noexcept
{
	if (searchCase == ESearchCase::Sensitive)
	{
		return searchDir == ESearchDir::Forward
			? Split<ESearchCase::Sensitive, ESearchDir::Forward>(delim, outLeft, outRight)
			: Split<ESearchCase::Sensitive, ESearchDir::Backward>(delim, outLeft, outRight);
	}
	else
	{
		return searchDir == ESearchDir::Forward
			? Split<ESearchCase::Insensitive, ESearchDir::Forward>(delim, outLeft, outRight)
			: Split<ESearchCase::Insensitive, ESearchDir::Backward>(delim, outLeft, outRight);
	}
}

template<typename CharT>
template<ESearchCase Case>
TArray<TStringView<CharT>> TStringView<CharT>::SplitToArray(const TStringView& delim, bool discardEmpty) const noexcept
{
	TArray<TStringView> result;

	const SizeType delimLen = delim._len;
	const CharT* cursor = _data;
	SizeType remaining = _len;

	while (remaining > 0)
	{
		const SizeType foundIdx = SOps::template Find<Case, ESearchDir::Forward>(cursor, delim._data, remaining, delimLen);

		if (foundIdx == KOR_INDEX_NONE)
		{
			// Remainder is the last token
			if (!discardEmpty || remaining > 0)
			{
				result.Add(TStringView(cursor, remaining));
			}
			break;
		}

		if (!discardEmpty || foundIdx > 0)
		{
			result.Add(TStringView(cursor, foundIdx));
		}

		cursor += foundIdx + delimLen;
		remaining -= foundIdx + delimLen;
	}

	return result;
}

template<typename CharT>
template<ESearchCase Case>
TArray<TString<CharT>> TString<CharT>::SplitToArray(const TString& delim, bool discardEmpty) const noexcept
{
	TArray<TString> result;

	const SizeType delimLen = delim._data.GetNum() - 1;
	const CharT* cursor = *_data;
	SizeType remaining = _data.GetNum() - 1;

	while (remaining > 0)
	{
		const SizeType foundIdx = SOps::template Find<Case, ESearchDir::Forward>(cursor, *delim._data, remaining, delimLen);

		if (foundIdx == KOR_INDEX_NONE)
		{
			if (!discardEmpty || remaining > 0)
			{
				result.Add(TString(cursor, remaining));
			}
			break;
		}

		if (!discardEmpty || foundIdx > 0)
		{
			result.Add(TString(cursor, foundIdx));
		}

		cursor += foundIdx + delimLen;
		remaining -= foundIdx + delimLen;
	}

	return result;
}

template<typename CharT>
KOR_FORCEINLINE TArray<TStringView<CharT>> TStringView<CharT>::SplitToArray(const TStringView& delim, bool discardEmpty, ESearchCase searchCase) const noexcept
{
	return searchCase == ESearchCase::Sensitive
		? SplitToArray<ESearchCase::Sensitive>(delim, discardEmpty)
		: SplitToArray<ESearchCase::Insensitive>(delim, discardEmpty);
}

template<typename CharT>
KOR_FORCEINLINE TArray<TString<CharT>> TString<CharT>::SplitToArray(const TString& delim, bool discardEmpty, ESearchCase searchCase) const noexcept
{
	return searchCase == ESearchCase::Sensitive
		? SplitToArray<ESearchCase::Sensitive>(delim, discardEmpty)
		: SplitToArray<ESearchCase::Insensitive>(delim, discardEmpty);
}