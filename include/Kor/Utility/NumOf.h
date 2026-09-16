// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/TypeTrait/Macros/HasFieldCheck.h"

KOR_NAMESPACE_BEGIN

// Returns number of elements within `range`
// * Number type and value is one as calling "GetNum()"
//
// Example:
// TArray<uint8> arr = { 1, 2, 3, 4 };
// TArray<uint8>::SizeType arr = NumOf(arr);
template<typename RangeT>
KOR_NODISCARD KOR_FORCEINLINE constexpr auto NumOf(const RangeT& range)
	-> decltype(DeclVal<RangeT>().GetNum())
{
	return range.GetNum();
}

// Returns number of elements within static array
//
// Example:
// static const int32 Ids[]   = { 1, 2, 3, 4 };
// TSize numOfIds = NumOf(Ids);
template<typename T, TSize N>
KOR_NODISCARD KOR_FORCEINLINE constexpr TSize NumOf(const T(&)[N]) noexcept
{
	return N;
}

KOR_NAMESPACE_END