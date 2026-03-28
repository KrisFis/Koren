// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/Math.h"
#include "Kor/Memory.h"

#include KOR_PLATFORM_HEADER(CString)

KOR_NAMESPACE_BEGIN

typedef KOR_PLATFORM_PREFIXED_TYPE(S, PlatformCString) SPlatformCString;
struct SCString : public SPlatformCString
{
	static constexpr uint16 SMALL_BUFFER_SIZE = 1024;
	static constexpr uint16 LARGE_BUFFER_SIZE = 4096;
	static constexpr uint16 MAX_BUFFER_SIZE_INT32 = 33;
	static constexpr uint16 MAX_BUFFER_SIZE_INT64 = 65;
	static constexpr uint16 MAX_BUFFER_SIZE_DOUBLE = 309+40; // _CVTBUFSIZE

	// Gets length of a string
	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static uint32 GetLength(const CharType* str)
	{
		const CharType* current = str;
		while(*current != KOR_CHAR_TERM)
			++current;

		return static_cast<uint32>(current - str);
	}

	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static CharType* ToUpper(CharType* str, uint32 maxLen = TLimits<uint32>::Max)
	{
		CharType* current = str;
		while(*current != KOR_CHAR_TERM && maxLen > 0)
		{
			*current = SCString::ToUpperChar(*current);
			++current;
			--maxLen;
		}

		return str;
	}

	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static CharType* ToLower(CharType* str, uint32 maxLen = TLimits<uint32>::Max)
	{
		CharType* current = str;
		while(*current != KOR_CHAR_TERM && maxLen > 0)
		{
			*current = SCString::ToLowerChar(*current);
			++current;
			--maxLen;
		}

		return str;
	}

	// Copies contents of one string to the other
	// * String memory should not overlap
	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static CharType* Copy(CharType* dest, const CharType* src, uint32 maxLen = TLimits<uint32>::Max)
	{
		return SMemory::CopyTyped(
			dest,
			src,
			SMath::Min(GetLength(src), maxLen)
		);
	}

	// Copies contents of one string to the other
	// * String memory can overlap
	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static CharType* Move(CharType* dest, const CharType* src, uint32 maxLen = TLimits<uint32>::Max)
	{
		return SMemory::MoveTyped(
			dest,
			src,
			SMath::Min(GetLength(src), maxLen)
		);
	}

	// When:
	// * Lhs bigger -> -1
	// * Rhs bigger -> 1
	// * Equals -> 0
	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static int32 CompareLength(const CharType* lhs, const CharType* rhs)
	{
		while(*lhs != KOR_CHAR_TERM && *rhs != KOR_CHAR_TERM)
		{
			++lhs;
			++rhs;
		}

		return (*lhs == KOR_CHAR_TERM && *rhs == KOR_CHAR_TERM)
			? 0
			: (*lhs == KOR_CHAR_TERM ? 1 : -1);
	}

	// When:
	// * Value bigger -> -1
	// * TestLen bigger -> 1
	// * Equals -> 0
	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static int32 CompareLength(const CharType* val, uint32 testLen)
	{
		while (*val != KOR_CHAR_TERM && testLen > 0)
		{
			++val;
			--testLen;
		}

		return (*val == KOR_CHAR_TERM && testLen == 0)
			? 0
			: (*val == KOR_CHAR_TERM ? 1 : -1);
	}

	// Compares contents of two strings
	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static int32 Compare(
		const CharType* lhs,
		const CharType* rhs,
		bool caseSensitive = true,
		bool fromStart = true,
		uint32 maxLen = TLimits<uint32>::Max)
	{
		if (!fromStart)
		{
			const uint32 lhsLen = GetLength(lhs);
			const uint32 rhsLen = GetLength(rhs);

			// iterate only up to shortest of the strings to not go under the allocation
			maxLen = SMath::Min(maxLen, SMath::Min(lhsLen, rhsLen));

			// move in character before KOR_CHAR_TERM
			lhs += lhsLen - 1;
			rhs += rhsLen - 1;
		}

		return CompareImpl(lhs, rhs, caseSensitive, !fromStart, maxLen);
	}

	// Finds the first occurrence of the substring "test" in the string "str" and returns ptr
	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static const CharType* Find(
		const CharType* str,
		const CharType* test,
		bool caseSensitive = true,
		bool fromStart = true,
		uint32 maxLen = TLimits<uint32>::Max)
	{
		const uint32 testLen = GetLength(test);
		if (testLen == 0) return str;

		const uint32 strLen = GetLength(str);
		if (testLen >= strLen) return nullptr;

		if (!fromStart)
		{
			// move in character before KOR_CHAR_TERM
			str += strLen - 1;
		}

		maxLen = SMath::Min(maxLen, strLen);
		while (maxLen > 0)
		{
			if (CompareImpl(str, test, caseSensitive, !fromStart, testLen) == 0)
				return str;

			if (fromStart)
			{
				++str;
			}
			else
			{
				--str;
			}
			--maxLen;
		}

		return nullptr;
	}

	// Finds the first occurrence of the substring "test" in the string "str" and returns index
	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	static int32 FindIndex(
		const CharType* str,
		const CharType* test,
		bool caseSensitive = true,
		bool fromStart = true,
		uint32 maxLen = TLimits<uint32>::Max)
	{
		const CharType* foundStr = Find(str, test, caseSensitive, fromStart, maxLen);
		return foundStr ? KOR_PTR_DIFF_TYPED(int32, foundStr, str) : KOR_INDEX_NONE;
	}

private:

	// Compare implementation
	template<typename CharType>
	static int32 CompareImpl(const CharType* lhs, const CharType* rhs, bool caseSensitive, bool reverse, uint32 maxLen)
	{
		while(maxLen > 0)
		{
			CharType lhsChar = *lhs;
			CharType rhsChar = *rhs;

			if (!caseSensitive)
			{
				lhsChar = SPlatformCString::ToLowerChar(*lhs);
				rhsChar = SPlatformCString::ToLowerChar(*rhs);
			}

			if (lhsChar != rhsChar)
				break;

			if (lhsChar == KOR_CHAR_TERM)
				return 0;

			if (reverse)
			{
				--lhs;
				--rhs;
			}
			else
			{
				++lhs;
				++rhs;
			}
			--maxLen;
		}

		return (maxLen > 0) ? (*lhs - *rhs) : 0;
	}
};

KOR_NAMESPACE_END