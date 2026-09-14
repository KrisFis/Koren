// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Minimal.h"

KOR_NAMESPACE_BEGIN

// [Size]
// * Size type without need of std (equivalent of std::size_t)

typedef decltype(sizeof(0)) TSize;

// [Nullptr]
// * Nullptr type without need of std (equivalent of std::nullptr_t)

typedef decltype(nullptr) TNullptr;

// [TVoid]
// * Void type
// * Used as SFINAE detection idiom target

template<typename...>
using TVoid = void;

// [Type]
// * Defines "Type" as provided type

template<typename T> struct TType { using Type = T; };

// [Bool Value]
// * Defines "Value" from provided const bool

template<bool T> struct TBoolValue { static constexpr bool Value = T; };
template<typename> struct TValue : TBoolValue<true> {};

using TTrueValue = TBoolValue<true>;
using TFalseValue = TBoolValue<false>;

// [Enable if]
// * Enables compilation of specific template function/struct when condition met

template<bool T, typename R = void> struct TEnableIf;
template<typename R> struct TEnableIf<true, R> : TType<R> {};
template<typename R> struct TEnableIf<false, R> {};

// [Is Same]
// * Checks whether specified types are the same

template<typename T, typename R> struct TIsSame : TFalseValue {};
template<typename T> struct TIsSame<T, T> : TTrueValue {};

// [Conditional\Choose]
// * Chooses between two different types based on a value

template<bool Value, typename TrueT, typename FalseT> struct TConditional;
template<typename TrueT, typename FalseT> struct TConditional<true, TrueT, FalseT> : TType<TrueT> {};
template<typename TrueT, typename FalseT> struct TConditional<false, TrueT, FalseT> : TType<FalseT> {};

template<bool Value, typename TrueT, typename FalseT>
using TChoose = TConditional<Value, TrueT, FalseT>;

// [Conditional\Choose Delayed]
// * Same as choose, but type is delayed by getting type from type

template<bool Value, typename TrueT, typename FalseT> struct TConditionalDelayed;
template<typename TrueT, typename FalseT> struct TConditionalDelayed<true, TrueT, FalseT> : TType<typename TrueT::Type> {};
template<typename TrueT, typename FalseT> struct TConditionalDelayed<false, TrueT, FalseT> : TType<typename FalseT::Type> {};

template<bool Value, typename TrueT, typename FalseT>
using TChooseDelayed = TConditional<Value, TrueT, FalseT>;

KOR_NAMESPACE_END
