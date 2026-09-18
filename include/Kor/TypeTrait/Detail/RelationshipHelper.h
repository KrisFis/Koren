// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

#include "Kor/Utility/Forward.h"

KOR_NAMESPACE_BEGIN

namespace Detail
{
	template<typename From, typename To, typename = void>
	struct TIsCastableImpl : TFalseValue {};

	template<typename From, typename To>
	struct TIsCastableImpl<From, To, TVoid<decltype(static_cast<To>(DeclVal<From>()))>> : TTrueValue {};

	template<typename To> void ConvertSink(To);

	template<typename From, typename To, typename = void>
	struct TIsConvertibleImpl : TFalseValue {};

	template<typename From, typename To>
	struct TIsConvertibleImpl<From, To, TVoid<decltype(ConvertSink<To>(DeclVal<From>()))>> : TTrueValue {};
}

KOR_NAMESPACE_END