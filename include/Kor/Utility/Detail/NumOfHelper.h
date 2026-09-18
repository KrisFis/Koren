// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

KOR_NAMESPACE_BEGIN

namespace Detail
{
	// Priority system
	// 1 - GetNum
	// 2 - size

	template<typename T, typename = void>
	struct TNumOfViaSize {};

	template<typename T>
	struct TNumOfViaSize<T, TVoid<decltype(DeclVal<const T&>().size())>>
	{
		KOR_FORCEINLINE static constexpr auto Get(const T& range)
			-> decltype(range.size())
		{
			return range.size();
		}
	};

	template<typename T, typename = void>
	struct TNumOfViaGetNum : TNumOfViaSize<T> {};

	template<typename T>
	struct TNumOfViaGetNum<T, TVoid<decltype(DeclVal<const T&>().GetNum())>>
	{
		KOR_FORCEINLINE static constexpr auto Get(const T& range)
			-> decltype(range.GetNum())
		{
			return range.GetNum();
		}
	};

	template<typename T>
	struct TNumOfTrait : TNumOfViaGetNum<T> {};
}

KOR_NAMESPACE_END