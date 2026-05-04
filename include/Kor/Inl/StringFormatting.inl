// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Internal
{
	template<typename CharT, typename FmtT>
	struct TTStringFormatTraits
	{
		static_assert(TIsCharacter<CharT>::Value, "CharT must be character type");

	private:
		using RawFmtT = typename TRemoveConstReference<FmtT>::Type;
		using PureFmt = typename TPure<FmtT>::Type;

	public:
		enum
		{
			IsCString = (TIsPointer<RawFmtT>::Value && TIsSame<PureFmt, CharT>::Value),
			IsCArray = (TIsArray<RawFmtT>::Value && TIsSame<PureFmt, CharT>::Value),

			IsTString = TIsSame<PureFmt, TString<CharT>>::Value,

			IsValid = IsCArray || IsCString || IsTString
		};
	};
}

template<typename CharT>
template<typename FmtT, typename ... VarTypes>
TString<CharT> TString<CharT>::Format(const FmtT& fmt, const VarTypes&... args) noexcept
{
	using StringTraits = Internal::TTStringFormatTraits<CharT, FmtT>;
	static_assert(sizeof...(VarTypes) > 0, "No arguments provided. Use construction from fmt directly instead");
	static_assert(StringTraits::IsValid, "Format variable is not valid");

	const CharType* format = nullptr;
	if constexpr (StringTraits::IsTString)                                { format = fmt.GetChars(); }
	else if constexpr (StringTraits::IsCArray || StringTraits::IsCString) { format = fmt; }

	// Pre-allocate 512 chars — covers the majority of format strings without reallocation
	constexpr int32 InlineBufferSize = 512;

	TString result;
	result._data.Resize(InlineBufferSize);

	int32 written = SOps::Format(result._data.GetData(), format, result._data.GetNum(), args...);
	if (written < 0) return TString::GetEmpty();

	if (written >= result._data.GetNum())
	{
		// Buffer was too small — resize to exact size and reformat
		result._data.Resize(written + 1);
		KOR_ASSERT(SOps::Format(result._data.GetData(), format, result._data.GetNum(), args...) == written);
	}
	else
	{
		// Common path — shrink to actual size
		result._data.Resize(written + 1);
	}

	// MSVC wide does not null terminate on truncation, ensure null terminator explicitly
	result._data[written] = Constant::Null;
	return result;
}

template<typename CharT>
template<typename StringT, typename ... ArgTypes>
KOR_FORCEINLINE void TString<CharT>::AppendFormat(const StringT& fmt, const ArgTypes&... args) noexcept
{
	Append(TString::Format(fmt, args...));
}