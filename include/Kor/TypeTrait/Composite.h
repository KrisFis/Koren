// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/TypeTrait/Category.h"
#include "Kor/TypeTrait/MemberPointer.h"

KOR_NAMESPACE_BEGIN

// [Is Integral]
// * Checks whether specific type is integral

template<typename T> struct TIsIntegral
	: TBoolValue<
		TIsInteger<T>::Value ||
		TIsCharacter<T>::Value
	>
{};

// [Is Arithmetic]
// * Checks whether specific type is arithmetic

template<typename T>
struct TIsArithmetic
	: TBoolValue<
		TIsIntegral<T>::Value ||
		TIsFloating<T>::Value ||
		TIsBool<T>::Value
	>
{};

// [Is Fundamental]
// * Checks whether specific type is a fundamental type
// * Fundamental type is any type that is fundamental to cpp (not user-defined)

template<typename T>
struct TIsFundamental
	: TBoolValue<
		TIsVoid<T>::Value ||
		TIsNullptr<T>::Value ||
		TIsArithmetic<T>::Value
	>
{};

// [Is Compound]
// * Checks whether specific type is a compound type
// * Compound type is any type that is NOT fundamental to cpp

template<typename T> struct TIsCompound
	: TBoolValue<!TIsFundamental<T>::Value>
{};

// [Is Scalar]
// * Checks whether specific type is scalar type
// * Scalar type is any type whose value can used as single indivisible value without containing subobjects

template<typename T> struct TIsScalar
	: TBoolValue<
		TIsArithmetic<T>::Value ||
		TIsEnum<T>::Value ||
		TIsPointer<T>::Value ||
		TIsMemberPointer<T>::Value ||
		TIsNullptr<T>::Value
	>
{};

KOR_NAMESPACE_END
