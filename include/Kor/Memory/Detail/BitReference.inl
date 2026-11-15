// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename WordT>
KOR_FORCEINLINE constexpr TBitReference<WordT>::TBitReference(WordType& data, WordType mask) noexcept
	: _data(data)
	, _mask(mask)
{}

template<typename WordT>
KOR_NODISCARD KOR_FORCEINLINE constexpr TBitReference<WordT>::operator bool() const noexcept
{
	return Get();
}

template<typename WordT>
KOR_FORCEINLINE constexpr TBitReference<WordT>& TBitReference<WordT>::operator=(bool value) noexcept
{
	Set(value);
	return *this;
}

template<typename WordT>
KOR_FORCEINLINE constexpr TBitReference<WordT>& TBitReference<WordT>::operator|=(bool value) noexcept
{
	if (value) { _data |= _mask; }
	return *this;
}

template<typename WordT>
KOR_FORCEINLINE constexpr TBitReference<WordT>& TBitReference<WordT>::operator&=(bool value) noexcept
{
	if (!value) { _data &= ~_mask; }
	return *this;
}

template<typename WordT>
KOR_FORCEINLINE constexpr bool TBitReference<WordT>::Get() const noexcept
{
	return (_data & _mask) != 0;
}

template<typename WordT>
KOR_FORCEINLINE constexpr void TBitReference<WordT>::Set(bool value) noexcept
{
	if (value) { _data |= _mask; }
	else { _data &= ~_mask; }
}

template<typename WordT>
KOR_FORCEINLINE constexpr TConstBitReference<WordT>::TConstBitReference(const WordType& data, WordType mask) noexcept
	: _data(data)
	, _mask(mask)
{}

template<typename WordT>
KOR_NODISCARD KOR_FORCEINLINE constexpr TConstBitReference<WordT>::operator bool() const noexcept
{
	return Get();
}

template<typename WordT>
KOR_FORCEINLINE constexpr bool TConstBitReference<WordT>::Get() const noexcept
{
	return (_data & _mask) != 0;
}