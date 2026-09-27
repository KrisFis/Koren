// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/TypeTrait/Pack.h"

KOR_NAMESPACE_BEGIN

// [Member Function Traits]
// Traits that gets information about function
// * Like return type, number of arguments and individual argument types
// *
// * Example:
// ** using FooTraits = TMemberFunction<decltype(SMyType::Foo)>;

template<typename Fn>
struct TMemberFunctionTraits
{
	static constexpr bool Valid = false;
};

template<typename R, typename C, typename... Args>
struct TMemberFunctionTraits<R (C::*)(Args...) const>
{
	using ReturnType = R;

	template<int32 N>
	using ArgType = typename TNthArg<N, Args...>::Type;

	static constexpr int32 Arity = sizeof...(Args);
	static constexpr bool Valid = true;
};

#define KOR_DEFINE_MEMBER_FUNCTION_TRAIT(DefineName, FuncName)			\
	template<typename T>												\
	using DefineName = TMemberFunctionTraits<decltype(&T::FuncName)>;

KOR_NAMESPACE_END