// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

KOR_NAMESPACE_BEGIN

// [Is Array]
// * Checks whether specific type is array

template<typename T> struct TIsArray : TFalseValue {};
template<typename T> struct TIsArray<T[]> : TTrueValue {};
template<typename T, TSize N> struct TIsArray<T[N]> : TTrueValue {};

// [Is Function]
// * Checks whether specific type is function

template<typename T> struct TIsFunction : TFalseValue {};
template<typename RetType, typename... Params> struct TIsFunction<RetType(Params...)> : TTrueValue {};

// [Is Nullptr]
// * Checks whether specific type is nullptr

template<typename T> struct TIsNullptr : TFalseValue {};
template<> struct TIsNullptr<TNullptr> : TTrueValue {};

// [Is Void]
// * Checks whether specific type is void

template<typename T> struct TIsVoid : TFalseValue {};
template<> struct TIsVoid<void> : TTrueValue {};

// [Is Bool]
// * Checks whether specific type is boolean type

template<typename T> struct TIsBool : TFalseValue {};
template<> struct TIsBool<bool> : TTrueValue {};

// [Is Floating]
// * Checks whether specific type is floating type
// * Floating types are: float, double

template<typename T> struct TIsFloating : TFalseValue {};
template<> struct TIsFloating<float> : TTrueValue {};
template<> struct TIsFloating<double> : TTrueValue {};
template<> struct TIsFloating<long double> : TTrueValue {};

// [Is Character]
// * Checks whether specific type is character type
// * Character types are: char, wchar

template<typename T> struct TIsCharacter : TFalseValue {};
template<> struct TIsCharacter<achar> : TTrueValue {};
#if KOR_CHAR8_NATIVE
template<> struct TIsCharacter<char8> : TTrueValue {};
#endif
template<> struct TIsCharacter<wchar> : TTrueValue {};
template<> struct TIsCharacter<char16> : TTrueValue {};
template<> struct TIsCharacter<char32> : TTrueValue {};

// [Is Integer]
// * Checks whether specific type is integer type
// * Integer types are: int8, int16, int32, int64, uint8, uint16, uint32, uint64

template<typename T> struct TIsInteger : TFalseValue {};
template<> struct TIsInteger<int8> : TTrueValue {};
template<> struct TIsInteger<int16> : TTrueValue {};
template<> struct TIsInteger<int32> : TTrueValue {};
template<> struct TIsInteger<int64> : TTrueValue {};
template<> struct TIsInteger<uint8> : TTrueValue {};
template<> struct TIsInteger<uint16> : TTrueValue {};
template<> struct TIsInteger<uint32> : TTrueValue {};
template<> struct TIsInteger<uint64> : TTrueValue {};

// [Is class type]
// * Checks whether specific type is class

template<typename T>
struct TIsClass : TBoolValue<__is_class(T)> {};

// [Is enum type]
// * Checks whether specific type is enum

template<typename T>
struct TIsEnum : TBoolValue<__is_enum(T)> {};

// [Is union type]
// * Checks whether specific type is union

template<typename T>
struct TIsUnion : TBoolValue<__is_union(T)> {};

// [Is Pointer]
// * Checks whether provided type is pointer
// * NOTE: does not match member pointers ('T C::*') - see MemberPointer.h for TIsMemberPointer

template<typename T> struct TIsPointer : TFalseValue {};
template<typename T> struct TIsPointer<T*> : TTrueValue {};

KOR_NAMESPACE_END
