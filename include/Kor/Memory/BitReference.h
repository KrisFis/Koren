// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"
#include "Kor/TypeTrait/Category.h"
#include "Kor/TypeTrait/Property.h"

KOR_NAMESPACE_BEGIN

// [ Bit Reference ]
// Mutable proxy reference to a single bit stored inside an unsigned integer word.
// * `mask` identifies the addressed bit and must be non-zero for meaningful access.
// * Assigning or setting updates only the masked bit, preserving all other bits in the word.
template<typename WordT>
class TBitReference
{
	static_assert(TIsInteger<WordT>::Value && TIsUnsigned<WordT>::Value,
		"WordT must be an unsigned integer type"
	);

public:
	using WordType = WordT;

	explicit constexpr TBitReference(WordType& data, WordType mask) noexcept;

	constexpr operator bool() const noexcept;

	constexpr TBitReference& operator=(bool value) noexcept;
	constexpr TBitReference& operator|=(bool value) noexcept;
	constexpr TBitReference& operator&=(bool value) noexcept;

	constexpr bool Get() const noexcept;
	constexpr void Set(bool value) noexcept;

private:
	WordType& _data;
	WordType _mask;
};

// [ Const Bit Reference ]
// Read-only proxy reference to a single bit stored inside an unsigned integer word.
// * `mask` identifies the addressed bit and must be non-zero for meaningful access.
// * Provides bool conversion and Get access without allowing mutation of the underlying word.
template<typename WordT>
class TConstBitReference
{
	static_assert(TIsInteger<WordT>::Value && TIsUnsigned<WordT>::Value,
		"WordT must be an unsigned integer type"
	);

public:
	using WordType = WordT;

	explicit constexpr TConstBitReference(const WordType& data, WordType mask) noexcept;

	constexpr operator bool() const noexcept;

	constexpr bool Get() const noexcept;

private:
	const WordType& _data;
	WordType _mask;
};

// Make Bit Reference
// Creates a mutable bit-reference proxy for the bit selected by `mask` in `data`.
// * Returned proxy can read and modify the masked bit of the referenced word.
template<typename WordT>
KOR_NODISCARD KOR_FORCEINLINE static constexpr auto MakeBitReference(WordT& data, WordT mask) noexcept
	-> TBitReference<WordT>
{
	return TBitReference<WordT>(data, mask);
}

// Make Const Bit Reference
// Creates a read-only bit-reference proxy for the bit selected by `mask` in `data`.
// * Returned proxy can read the masked bit without modifying the referenced word.
template<typename WordT>
KOR_NODISCARD KOR_FORCEINLINE static constexpr auto MakeBitReference(const WordT& data, WordT mask) noexcept
	-> TConstBitReference<WordT>
{
	return TConstBitReference<WordT>(data, mask);
}

#include "Kor/Memory/Detail/BitReference.inl"

KOR_NAMESPACE_END