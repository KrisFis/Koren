// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

// [Is Base Of]
// * Checks whether specific type is base of other type

template<typename BaseT, typename DerivedT>
struct TIsBaseOf : TBoolValue<__is_base_of(BaseT, DerivedT)> {};


// [Is Derived From]
// * Checks whether specific type is derived from other type

template<typename DerivedT, typename BaseT>
struct TIsDerivedFrom : TIsBaseOf<BaseT, DerivedT> {};