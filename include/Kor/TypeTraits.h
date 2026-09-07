// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

#include "Kor/Internal/TypeTraitsConstruct.h"
#include "Kor/Internal/TypeTraitsContainers.h"
#include "Kor/Internal/TypeTraitsAllocators.h"
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

// [Pure]
// * Removes all qualifiers

template<typename T>
struct TPure
{
private:
	using TestType = typename TClean<T>::Type;

public:
	typedef typename TChoose<
		TIsArray<TestType>::Value,
		typename TRemoveExtent<TestType>::Type,
		typename TChoose<
			TIsPointer<TestType>::Value,
			typename TRemovePointer<TestType>::Type,
			TestType
		>::Type
	>::Type Type;
};

// [Is Pure]
// * Checks if type is pure type (no qualifiers)

template<typename T>
struct TIsPure : TBoolValue<TIsSame<typename TPure<T>::Value, T>::Value> {};

// [Get type]
// * Gets type variations

template<typename T>
struct TGetType
{
	typedef typename TPure<T>::Type Value;

	typedef Value& Reference;
	typedef const Value& ConstReference;

	typedef Value* Pointer;
	typedef const Value* ConstPointer;
};

// [Call traits]
// * Determines which type will be used for call
// * Similar to boost's call_traits, ie. having info about optimizations of which type is used

template <typename T>
struct TCallTraits : TGetType<T>
{
private:
	enum { IsSmallType = ((sizeof(T) <= sizeof(void*)) && TIsPOD<T>::Value ) || TIsArithmetic<T>::Value };

public:
	typedef typename Internals::TCallTraitsHelper<T, IsSmallType>::Type Param;
	typedef typename Internals::TCallTraitsHelper<T, IsSmallType>::ConstType ConstParam;
};

// [Limits]
// Tells value limit of specific integral type
// Similar to std::numeric_limits<T>

template<typename T>
struct TLimits
{
	static_assert(TIsArithmetic<T>::Value, "TLimits accepts only arithmetic types");
	static_assert(TIsSame<T, typename TPure<T>::Type>::Value, "TLimits accepts only pure types");

	static constexpr bool IsSigned = TIsSigned<T>::Value;

	static constexpr T Max = (T)((uint64)1 << (sizeof(T) * 8 - (IsSigned ? 1 : 0))) - 1;
	static constexpr T Min = IsSigned ? (-(int64)((uint64)1 << (sizeof(T) * 8 - 1))) : 0;
};

KOR_NAMESPACE_END