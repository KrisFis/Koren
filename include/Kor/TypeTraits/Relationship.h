// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTraits/Minimal.h"

KOR_NAMESPACE_BEGIN

// [Is Base Of \ Is Derived From]
// * Checks whether specific type is base of other type

template<typename BaseT, typename DerivedT>
struct TIsBaseOf : TBoolValue<__is_base_of(BaseT, DerivedT)> {};

template<typename DerivedT, typename BaseT>
using TIsDerivedFrom = TIsBaseOf<BaseT, DerivedT>;

// [Is Castable]
// * Checks whether specific types could be casted to each other

template<typename T, typename R>
struct TIsCastable { enum { Value = TIsDerivedFrom<T, R>::Value || TIsBaseOf<T, R>::Value }; };

KOR_NAMESPACE_END