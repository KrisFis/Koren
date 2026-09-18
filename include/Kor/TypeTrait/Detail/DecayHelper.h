// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

KOR_NAMESPACE_BEGIN

namespace Detail
{
	template<typename T>
	struct TDecayHelper { using Type = T; };

	template<typename T>
	struct TDecayHelper<T[]> { using Type = T*; };

	template<typename T, TSize N>
	struct TDecayHelper<T[N]> { using Type = T*; };

	template<typename T, typename... Params>
	struct TDecayHelper<T(Params...)> { using Type = T(*)(Params...); };

	template<typename T>
	struct TDecayImpl : TDecayHelper<typename TClean<T>::Type> {};
}

KOR_NAMESPACE_END