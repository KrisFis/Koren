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
}

template<typename CharT>
template<typename StringT, typename ... ArgTypes>
void TString<CharT>::AppendFormat(StringT&& fmt, const ArgTypes&... args) noexcept
{
}