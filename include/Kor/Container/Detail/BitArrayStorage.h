// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Container/Minimal.h"

#include "Kor/Container/Detail/ArrayStorage.h"

KOR_NAMESPACE_BEGIN

namespace Detail
{
	template<typename WordT, typename AllocatorT>
	class TBitArrayStorage : public TArrayStorage<WordT, AllocatorT>
	{
		using Super = TArrayStorage<WordT, AllocatorT>;
		using Super::ElementType; // private: discourage use, prefer WordType

	public:
		using WordType = WordT;
		using typename Super::AllocatorTraits;
		using typename Super::AllocatorOps;
		using typename Super::AllocatorType;
		using typename Super::SizeType;

		// Constants
		// -------------------------------------------------------------------------

		static constexpr SizeType BitsPerWord = sizeof(WordType) * SMemoryConstant::b_PER_BYTE;

		// Fields
		// -------------------------------------------------------------------------

		using Super::Allocator;
		using Super::Data;
		using Super::Num;
		using Super::ReservedNum;

		// Constructors
		// -------------------------------------------------------------------------

		using Super::Super;

		// Ops
		// -------------------------------------------------------------------------

		static constexpr SizeType ToWordNum(SizeType bitNum) noexcept
		{
			return bitNum > 0
				? ((bitNum + BitsPerWord - 1) / BitsPerWord)
				: 0;
		}

		static constexpr SizeType ToWordIndex(SizeType idx) noexcept
		{
			return idx / BitsPerWord;
		}

		static constexpr SizeType ToBitIndex(SizeType idx) noexcept
		{
			return idx % BitsPerWord;
		}

		// Mutation
		// -------------------------------------------------------------------------

		void MaskUnusedBits() noexcept
		{
			KOR_ASSERT(Num > 0);

			const SizeType usedBitsInLastWord = Num % BitsPerWord;
			if (usedBitsInLastWord == 0) return;

			const SizeType lastWordIdx = ToWordNum(Num) - 1;
			const WordType mask = (WordType(1) << usedBitsInLastWord) - 1;
			Data[lastWordIdx] &= mask;
		}
	};
}

KOR_NAMESPACE_END