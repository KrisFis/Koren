// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
constexpr TStringView<CharT> TStringView<CharT>::GetEmpty() noexcept
{
}

template<typename CharT>
constexpr const TString<CharT>& TString<CharT>::GetEmpty() noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::IsValidIndex(SizeType idx) const noexcept
{
}

template<typename CharT>
bool TString<CharT>::IsValidIndex(SizeType idx) const noexcept
{
}

template<typename CharT>
bool TStringView<CharT>::IsEmpty() const noexcept
{
}

template<typename CharT>
bool TString<CharT>::IsEmpty() const noexcept
{
}

template<typename CharT>
typename TStringView<CharT>::SizeType TStringView<CharT>::GetLength() const noexcept
{
}

template<typename CharT>
typename TString<CharT>::SizeType TString<CharT>::GetLength() const noexcept
{
}

template<typename CharT>
const typename TStringView<CharT>::CharType* TStringView<CharT>::GetChars() const noexcept
{
}

template<typename CharT>
const typename TString<CharT>::CharType* TString<CharT>::GetChars() const noexcept
{
}

template<typename CharT>
typename TString<CharT>::CharType* TString<CharT>::GetChars() noexcept
{
}

template<typename CharT>
const typename TString<CharT>::DataType& TString<CharT>::GetData() const noexcept
{
}

#include "Kor/Inl/StringConstructors.inl"
#include "Kor/Inl/StringConversions.inl"
#include "Kor/Inl/StringFormatting.inl"
#include "Kor/Inl/StringIterators.inl"
#include "Kor/Inl/StringMemory.inl"
#include "Kor/Inl/StringMutation.inl"
#include "Kor/Inl/StringOperators.inl"
#include "Kor/Inl/StringQuery.inl"
#include "Kor/Inl/StringSplit.inl"
#include "Kor/Inl/StringSubstring.inl"

// Container traits
// -------------------------------------------------------------------------

template<typename T>
struct TContainerTypeTraits<TStringView<T>> : TContainerTypeTraits<void>
{
	using Type = TStringView<T>; // optional

	using ElementType = typename Type::CharType;
	using AllocatorType = void;

	enum
	{
		IsContainer = true,
		IsDynamic = false,
		InlineMemory = true
	};
};

template<typename T>
struct TContainerTypeTraits<TString<T>> : TContainerTypeTraits<void>
{
	using Type = TString<T>; // optional

	using ElementType = typename Type::CharType;
	using AllocatorType = typename Type::DataType::AllocatorType;

	enum
	{
		IsContainer = true,
		IsDynamic = true,
		InlineMemory = true
	};
};

// Archive operator<< && operator>>
// -------------------------------------------------------------------------

template<typename CharType>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const TStringView<CharType>& str)
{
	ar.Write(str.GetChars(), str.GetLength());
	return ar;
}

template<typename CharType>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const TString<CharType>& str)
{
	ar.Write(str.GetChars(), str.GetLength());
	return ar;
}

template<typename CharType>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, TStringView<CharType>& str)
{
	typename TString<CharType>::DataType newData;
	ar >> newData;
	str = TString<CharType>(Move(newData));
	return ar;
}

template<typename CharType>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, TString<CharType>& str)
{
	typename TString<CharType>::DataType newData;
	ar >> newData;
	str = TString<CharType>(Move(newData));
	return ar;
}