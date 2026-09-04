// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Internal/TypeTraitsForward.h"

KOR_NAMESPACE_BEGIN

// [Has virtual destructor]
// * Checks whether specific type has virtual destructor

template<typename T>
struct THasVirtualDestructor : TBoolValue<__has_virtual_destructor(T)> {};

// [Has equal operator]
// * Checks whether specific type has equal operator
// * NOTE: POD types always have operator==

template<typename T, typename = void> struct THasEqualOperator : TFalseValue {};
template<typename T> struct THasEqualOperator<T, decltype(DeclVal<T>() == DeclVal<T>(), void())> : TTrueValue {};

// [Is Complete]
// * Checks whether specific type is complete type

template<typename T, typename = void> struct TIsComplete : TFalseValue {};
template<typename T> struct TIsComplete<T, TVoid<decltype(sizeof(T))>> : TTrueValue {};

KOR_NAMESPACE_END