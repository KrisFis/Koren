// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/TypeTrait/Composite.h"
#include "Kor/Utility/Forward.h"

KOR_NAMESPACE_BEGIN

// [Is empty type]
// * Checks whether specific type is empty
// ** has no non-static members/fields and if so, then each is bit-fields of zero length
// ** this check is applied to each base class, if everything passes with true, then result is true

template<typename T>
struct TIsEmpty : TBoolValue<__is_empty(T)> {};

// [Is abstract type]
// * Checks whether specific type is abstract

template<typename T>
struct TIsAbstract : TBoolValue<__is_abstract(T)> {};

// [Is POD type]
// * Checks whether specific type is POD (C-lang compatible type)

template<typename T>
struct TIsPOD : TBoolValue<__is_pod(T)> {};

// [Is signed type]
// * Checks whether specific type is signed type

template<typename T> struct TIsSigned
{
	static_assert(TIsArithmetic<T>::Value, "TIsSigned is only valid for arithmetic types");
	enum { Value = T(-1) < T(0) };
};

// [Is unsigned type]
// * Checks whether specific type is unsigned type

template<typename T> struct TIsUnsigned
{
	static_assert(TIsArithmetic<T>::Value, "TIsUnsigned is only valid for arithmetic types");
	enum { Value = !TIsSigned<T>::Value };
};

// [Has virtual destructor]
// * Checks whether specific type has virtual destructor

template<typename T>
struct THasVirtualDestructor : TBoolValue<__has_virtual_destructor(T)> {};

// [Is Complete]
// * Checks whether specific type is complete type

template<typename T, typename = void> struct TIsComplete : TFalseValue {};
template<typename T> struct TIsComplete<T, TVoid<decltype(sizeof(T))>> : TTrueValue {};

// [Has equal operator]
// * Checks whether specific type has equal operator (operator==(T,T))
// * Does not check what operator returns
// * NOTE: POD types always have operator==

template<typename T, typename = void> struct THasEqualOperator : TFalseValue {};
template<typename T> struct THasEqualOperator<T, TVoid<decltype(DeclVal<T>() == DeclVal<T>())>> : TTrueValue {};

// [Has Dereference]
// * Checks whether specific type has dereference operator (operator*())
// * Does not check what operator returns

template<typename T, typename = void> struct THasDereferenceOperator : TFalseValue {};
template<typename T> struct THasDereferenceOperator<T, TVoid<decltype(*DeclVal<T>())>> : TTrueValue {};

KOR_NAMESPACE_END
