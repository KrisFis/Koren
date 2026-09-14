// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::IsValidIndex(SizeType idx) const noexcept
{
	return SMathOps::IsWithin(idx, 0, _len);
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::IsValidIndex(SizeType idx) const noexcept
{
	return SMathOps::IsWithin(idx, 0, _data.GetNum() - 1);
}

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::IsEmpty() const noexcept
{
	return _len == 0;
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::IsEmpty() const noexcept
{
	return _data.IsEmpty();
}

template<typename CharT>
KOR_FORCEINLINE typename TStringView<CharT>::SizeType TStringView<CharT>::GetLength() const noexcept
{
	return _len;
}

template<typename CharT>
KOR_FORCEINLINE typename TString<CharT>::SizeType TString<CharT>::GetLength() const noexcept
{
	return _data.GetNum() - 1;
}

template<typename CharT>
KOR_FORCEINLINE const typename TStringView<CharT>::CharType* TStringView<CharT>::GetChars() const noexcept
{
	return _data;
}

template<typename CharT>
KOR_FORCEINLINE const typename TString<CharT>::CharType* TString<CharT>::GetChars() const noexcept
{
	return *_data;
}

template<typename CharT>
KOR_FORCEINLINE typename TString<CharT>::CharType* TString<CharT>::GetChars() noexcept
{
	return _data;
}

template<typename CharT>
KOR_FORCEINLINE const typename TString<CharT>::DataType& TString<CharT>::GetData() const noexcept
{
	return *_data;
}

// Lifecycle
#include "Kor/String/Detail/StringConstructors.inl"
#include "Kor/String/Detail/StringMemory.inl"

// Core operations
#include "Kor/String/Detail/StringOperators.inl"
#include "Kor/String/Detail/StringMutation.inl"

// Inspection
#include "Kor/String/Detail/StringQuery.inl"
#include "Kor/String/Detail/StringSubstring.inl"

// Higher-level processing
#include "Kor/String/Detail/StringSplit.inl"
#include "Kor/String/Detail/StringFormatting.inl"
#include "Kor/String/Detail/StringConversions.inl"

// Iteration
#include "Kor/String/Detail/StringIterators.inl"

// Container traits
// -------------------------------------------------------------------------

template<typename T>
struct TContainerTraits<TStringView<T>>
	: TContainerTraitsBase<TStringView<T>>
{
	using ElementType = T;
	using AllocatorType = void;
	using SizeType = int32;

	enum { InlineMemory = true };
};

template<typename T>
struct TContainerTraits<TString<T>>
	: TContainerTraitsBase<TString<T>>
{
	using ElementType = T;
	using AllocatorType = typename TContainerTraits<typename TString<T>::DataType>::AllocatorType;
	using SizeType = typename TContainerTraits<typename TString<T>::DataType>::SizeType;

	enum { InlineMemory = true };
};

// Archive operator<< && operator>>
// -------------------------------------------------------------------------

template<typename CharType>
KOR_FORCEINLINE_DEBUG static SArchive& operator<<(SArchive& ar, const TStringView<CharType>& str)
{
	ar.Write(str.GetChars(), str.GetLength());
	return ar;
}

template<typename CharType>
KOR_FORCEINLINE_DEBUG static SArchive& operator<<(SArchive& ar, const TString<CharType>& str)
{
	ar.Write(str.GetChars(), str.GetLength());
	return ar;
}

template<typename CharType>
KOR_FORCEINLINE_DEBUG static SArchive& operator>>(SArchive& ar, TStringView<CharType>& str)
{
	typename TString<CharType>::DataType newData;
	ar >> newData;
	str = TString<CharType>(Move(newData));
	return ar;
}

template<typename CharType>
KOR_FORCEINLINE_DEBUG static SArchive& operator>>(SArchive& ar, TString<CharType>& str)
{
	typename TString<CharType>::DataType newData;
	ar >> newData;
	str = TString<CharType>(Move(newData));
	return ar;
}