// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/MemoryOps.h"
#include "Kor/TypeTrait/Macros/HasFieldCheck.h"

KOR_NAMESPACE_BEGIN

// [ THasSwap ]
// Checks whether specific type has "Swap" method
// -------------------------------------------------------------------------

KOR_DEFINE_HAS_METHOD_TRAIT(THasSwap, Swap(DeclVal<T&>()));

// Swaps values between `lhs` and `rhs`.
// * Single-value, `num`-count, and static-array overloads
// * Optionally uses T.Swap if available, falls back to SMemoryOps::Swap otherwise
// -------------------------------------------------------------------------
template<typename T>
KOR_FORCEINLINE void Swap(T& lhs, T& rhs) noexcept
{
	if constexpr (THasSwap<T>::Value)
	{
		lhs.Swap(rhs);
	}
	else
	{
		SMemoryOps::Swap(&lhs, &rhs);
	}
}

template<typename T>
KOR_FORCEINLINE void Swap(T& lhs, T& rhs, TSize num) noexcept
{
	if constexpr (THasSwap<T>::Value)
	{
		for (TSize i = 0; i < num; ++i)
		{
			(&lhs)[i].Swap((&rhs)[i]);
		}
	}
	else
	{
		SMemoryOps::Swap(&lhs, &rhs, num);
	}
}

template<typename T, TSize N>
KOR_FORCEINLINE void Swap(T (&lhs)[N], T (&rhs)[N]) noexcept
{
	Swap(lhs[0], rhs[0], N);
}

KOR_NAMESPACE_END