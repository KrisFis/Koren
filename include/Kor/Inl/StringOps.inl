// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Internal
{
	template<typename CharT, ESearchCase Case>
	KOR_FORCEINLINE static CharT CharMayLower(CharT c)
	{
		if constexpr (Case == ESearchCase::Insensitive) return TCharOps<CharT>::ToLower(c);
		else return c;
	}

	// TODO: Replace with smarter buffer
	template<typename CharType>
	KOR_FORCEINLINE static CharType* GetScratchBuffer() noexcept
	{
		thread_local CharType buffer[SMemory::BUFFER_SIZE_LARGE];
		return buffer;
	}
}

// Primitive inspection
#include "Kor/Inl/StringOps_Query.inl"
#include "Kor/Inl/StringOps_Compare.inl"

// Transformation
#include "Kor/Inl/StringOps_Find.inl"
#include "Kor/Inl/StringOps_Transform.inl"
#include "Kor/Inl/StringOps_Replace.inl"

// High-level output
#include "Kor/Inl/StringOps_Format.inl"
#include "Kor/Inl/StringOps_Convert.inl"
