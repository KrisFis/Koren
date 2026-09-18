// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/Utility/Forward.h"

#include "Kor/TypeTrait/Detail/RelationshipHelper.h"

KOR_NAMESPACE_BEGIN

// [Is Base Of \ Is Derived From]
// * Checks whether specific type is base of other type

template<typename BaseT, typename DerivedT>
struct TIsBaseOf : TBoolValue<__is_base_of(BaseT, DerivedT)> {};

template<typename DerivedT, typename BaseT>
using TIsDerivedFrom = TIsBaseOf<BaseT, DerivedT>;

// [Is Castable]
// * Checks whether specific type `From` could be casted to type `To`
// * Cast means explicit AND implicit cast

template<typename From, typename To>
using TIsCastable = Detail::TIsCastableImpl<From, To>;

// [Is Convertible]
// * Checks whether specific type `From` can be converted to type `To`
// * Convertion means implicit cast

template<typename From, typename To>
struct TIsConvertible : Detail::TIsConvertibleImpl<From, To> {};

template<typename From> struct TIsConvertible<From, void> : TBoolValue<TIsVoid<From>::Value> {};
template<typename To>   struct TIsConvertible<void, To>   : TBoolValue<TIsVoid<To>::Value> {};
template<>              struct TIsConvertible<void, void> : TTrueValue {};

KOR_NAMESPACE_END