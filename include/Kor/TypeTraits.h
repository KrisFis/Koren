// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

#include "Kor/Internal/TypeTraitsAllocators.h"
#include "Kor/Internal/TypeTraitsConstruct.h"
#include "Kor/Internal/TypeTraitsContainers.h"
#include "Kor/Internal/TypeTraitsCore.h"
#include "Kor/Internal/TypeTraitsDecay.h"
#include "Kor/Internal/TypeTraitsForward.h"
#include "Kor/Internal/TypeTraitsString.h"
#include "Kor/Internal/TypeTraitsType.h"
#include "Kor/Internal/TypeTraitsInvoke.h"
#include "Kor/Internal/TypeTraitsBaseOf.h"

KOR_NAMESPACE_BEGIN

// [Choose]
// * Chooses between two different types based on a value

template<bool Value, typename TrueType, typename FalseType> struct TChoose;
template<typename TrueType, typename FalseType> struct TChoose<true, TrueType, FalseType> { typedef TrueType Type; };
template<typename TrueType, typename FalseType> struct TChoose<false, TrueType, FalseType> { typedef FalseType Type; };

// [Choose Delayed]
// * Same as choose, but type is delayed by getting type from type

template<bool Value, typename TrueTrait, typename FalseTrait> struct TChooseDelayed;
template<typename TrueTrait, typename FalseTrait> struct TChooseDelayed<true, TrueTrait, FalseTrait> { typedef typename TrueTrait::Type Type; };
template<typename TrueTrait, typename FalseTrait> struct TChooseDelayed<false, TrueTrait, FalseTrait> { typedef typename FalseTrait::Type Type; };

// [Is Castable]
// * Checks whether specific types could be casted to each other

template<typename T, typename R>
struct TIsCastable { enum { Value = TIsDerivedFrom<T, R>::Value || TIsBaseOf<T, R>::Value }; };

// [Limits]
// Tells value limit of specific integral type
// Similar to std::numeric_limits<T>

template<typename T>
struct TLimits
{
	static_assert(TIsArithmetic<T>::Value, "TLimits accepts only arithmetic types");
	static_assert(TIsSame<T, typename TClean<T>::Type>::Value, "TLimits accepts only clean types");

	static constexpr bool IsSigned = TIsSigned<T>::Value;

	static constexpr T Max = (T)((uint64)1 << (sizeof(T) * 8 - (IsSigned ? 1 : 0))) - 1;
	static constexpr T Min = IsSigned ? (-(int64)((uint64)1 << (sizeof(T) * 8 - 1))) : 0;
};

KOR_NAMESPACE_END