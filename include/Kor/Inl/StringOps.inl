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

template<typename CharType>
int32 TStringOps<CharType>::Length(const CharType* str) noexcept
{
	int32 len = 0; while (*str++) ++len; return len;
}

template<typename CharType>
template<int32 N>
KOR_FORCEINLINE int32 TStringOps<CharType>::Length(const CharType(&)[N]) noexcept
{
	return N;
}

#include "Kor/Inl/StringOps_Query.inl"
#include "Kor/Inl/StringOps_Compare.inl"
#include "Kor/Inl/StringOps_Transform.inl"
#include "Kor/Inl/StringOps_Find.inl"
#include "Kor/Inl/StringOps_Replace.inl"
#include "Kor/Inl/StringOps_Format.inl"
#include "Kor/Inl/StringOps_Convert.inl"
