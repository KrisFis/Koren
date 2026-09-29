// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

KOR_NAMESPACE_BEGIN

template<typename T, TSize Fallback = 0>
KOR_NODISCARD KOR_FORCEINLINE constexpr TSize SizeOf() noexcept
{
	if constexpr (TIsVoid<typename TClean<T>::Type>::Value) return Fallback;
	else return sizeof(T);
}

template<typename T, TSize Fallback = 0>
KOR_NODISCARD KOR_FORCEINLINE constexpr TSize AlignOf() noexcept
{
	if constexpr (TIsVoid<typename TClean<T>::Type>::Value) return Fallback;
	else return alignof(T);
}

#if KOR_ALLOW_STATIC_TESTS
namespace Detail
{
	static_assert(SizeOf<int32>() == sizeof(int32));
	static_assert(SizeOf<void>() == 0);
	static_assert(SizeOf<void, 1>() == 1);
	static_assert(SizeOf<const void, 1>() == 1);
	static_assert(SizeOf<volatile void, 1>() == 1);
	static_assert(SizeOf<void*>() == sizeof(void*));

	static_assert(AlignOf<void, 16>() == 16);
	static_assert(AlignOf<int32>() == alignof(int32));
}
#endif

KOR_NAMESPACE_END