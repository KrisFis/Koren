// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

KOR_NAMESPACE_BEGIN

// [Nth arg]
// * Gets Nth arg type from parameter pack

template<TSize N, typename T, typename... ArgsT> struct TNthArg : TType<typename TNthArg<N - 1, ArgsT...>::Type> {};
template<typename T, typename... ArgsT> struct TNthArg<0, T, ArgsT...> : TType<T> {};

// [First / Last arg]
// * Gets first / last arg type from parameter pack

template<typename T, typename... ArgsT> struct TFirstArg : TType<typename TNthArg<0, T, ArgsT...>::Type> {};
template<typename T, typename... ArgsT> struct TLastArg : TType<typename TNthArg<sizeof...(ArgsT), T, ArgsT...>::Type> {};

KOR_NAMESPACE_END
