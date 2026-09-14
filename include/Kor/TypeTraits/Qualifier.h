// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTraits/Minimal.h"

KOR_NAMESPACE_BEGIN

// [Is Reference]
// * Checks whether provided type is reference

template<typename T> struct TIsReference : TFalseValue {};
template<typename T> struct TIsReference<T&> : TTrueValue {};
template<typename T> struct TIsReference<T&&> : TTrueValue {};

// [Is LValue]
// * Checks whether provided type is lvalue reference

template<typename T> struct TIsLValue : TFalseValue {};
template<typename T> struct TIsLValue<T&> : TTrueValue {};

// [Is RValue]
// * Checks whether provided type is rvalue reference

template<typename T> struct TIsRValue : TFalseValue {};
template<typename T> struct TIsRValue<T&&> : TTrueValue {};

// [Remove reference]
// * Removes reference from type

template<typename T> struct TRemoveReference : TType<T> {};
template<typename T> struct TRemoveReference<T& > : TType<T> {};
template<typename T> struct TRemoveReference<T&&> : TType<T> {};

// [Is Const]
// * Checks whether provided type is const, volatile or both

template<typename T> struct TIsConst : TFalseValue {};
template<typename T> struct TIsConst<const T> : TTrueValue {};
template<typename T> struct TIsConst<volatile T> : TTrueValue {};
template<typename T> struct TIsConst<const volatile T> : TTrueValue {};

// [Remove Const]
// * Removes "const" and "volatile" from type

template<typename T> struct TRemoveConst : TType<T> {};
template<typename T> struct TRemoveConst<const T> : TType<T> {};
template<typename T> struct TRemoveConst<volatile T> : TType<T> {};
template<typename T> struct TRemoveConst<const volatile T> : TType<T> {};

// [Clean]
// * Removes const, volatile and reference from provided type
// * Essentially stripping all qualifiers related to template passing
// * NOTE: does not decay arrays or functions - see Decay.h for TDecay

template<typename T> struct TClean
	: TType<typename TRemoveConst<typename TRemoveReference<T>::Type>::Type>
{};

// [Is Clean]
// * Checks if type is clean type (no qualifiers)

template<typename T> struct TIsClean
	: TBoolValue<TIsSame<typename TClean<T>::Type, T>::Value>
{};

KOR_NAMESPACE_END
