// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename CharT>
KOR_FORCEINLINE bool TStringView<CharT>::IsValidIndex(SizeType idx) const noexcept
{
	return SMath::IsWithin(idx, 0, _len);
}

template<typename CharT>
KOR_FORCEINLINE bool TString<CharT>::IsValidIndex(SizeType idx) const noexcept
{
	return SMath::IsWithin(idx, 0, _data.GetNum() - 1);
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
#include "Kor/Inl/StringConstructors.inl"
#include "Kor/Inl/StringMemory.inl"

// Core operations
#include "Kor/Inl/StringOperators.inl"
#include "Kor/Inl/StringMutation.inl"

// Inspection
#include "Kor/Inl/StringQuery.inl"
#include "Kor/Inl/StringSubstring.inl"

// Higher-level processing
#include "Kor/Inl/StringSplit.inl"
#include "Kor/Inl/StringFormatting.inl"
#include "Kor/Inl/StringConversions.inl"

// Iteration
#include "Kor/Inl/StringIterators.inl"

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