// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/Utility/Detail/NumOfHelper.h"

KOR_NAMESPACE_BEGIN

// Returns number of elements within `range`
// Supports containers with `GetNum()` or `size()` and static arrays
//
// Examples:
// TArray<uint8> arr = { 1, 2, 3, 4 };
// TArray<uint8>::SizeType arrNum = NumOf(arr);
//
// std::vector<uint8> vec = { 1, 2, 3, 4 };
// std::vector<uint8>::size_type vecNum = NumOf(vec);
//
// static const int32 Ids[] = { 1, 2, 3, 4 };
// TSize idsNum = NumOf(Ids);

template<typename RangeT>
KOR_NODISCARD KOR_FORCEINLINE constexpr auto NumOf(RangeT&& range)
	-> decltype(Detail::TNumOfTrait<typename TClean<RangeT>::Type>::Get(range))
{
	return Detail::TNumOfTrait<typename TClean<RangeT>::Type>::Get(range);
}

template<typename T, TSize N>
KOR_NODISCARD KOR_FORCEINLINE constexpr TSize NumOf(const T(&)[N]) noexcept
{
	return N;
}

KOR_NAMESPACE_END