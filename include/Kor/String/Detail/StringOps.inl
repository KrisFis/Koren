// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Detail
{
	template<ESearchCase Case, typename CharT>
	KOR_FORCEINLINE static CharT CharMayLower(CharT c)
	{
		if constexpr (Case == ESearchCase::Insensitive) return TCharOps<CharT>::ToLower(c);
		else return c;
	}

	// TODO: Replace with smarter buffer
	template<typename CharType>
	KOR_FORCEINLINE static CharType* GetScratchBuffer() noexcept
	{
		thread_local CharType buffer[KOR_BUFFER_SIZE_LARGE];
		return buffer;
	}
}

// Primitive inspection
#include "Kor/String/Detail/StringOps_Query.inl"
#include "Kor/String/Detail/StringOps_Compare.inl"

// Transformation
#include "Kor/String/Detail/StringOps_Find.inl"
#include "Kor/String/Detail/StringOps_Transform.inl"
#include "Kor/String/Detail/StringOps_Replace.inl"

// High-level output
#include "Kor/String/Detail/StringOps_Format.inl"
#include "Kor/String/Detail/StringOps_Convert.inl"
