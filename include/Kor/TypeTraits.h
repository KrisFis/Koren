// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"

// INCLUDES ALL INTERNALS
#include "ASTD/_internal/TypeTraitsConstruct.h"
#include "ASTD/_internal/TypeTraitsContainers.h"
#include "ASTD/_internal/TypeTraitsCore.h"
#include "ASTD/_internal/TypeTraitsDecayHelper.h"
#include "ASTD/_internal/TypeTraitsForward.h"
#include "ASTD/_internal/TypeTraitsType.h"

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

// [Is Derived From]
// * Checks whether specific type is derived from other type

template<typename DerivedType, typename BaseType>
struct TIsDerivedFrom
{

private: // Typedefs

	typedef char No[1];
	typedef char Yes[2];

private: // Testing

	static Yes& Test(BaseType*);
	static No& Test(...);

	static DerivedType* ChildPtr() { return nullptr; }

public: // Value

	enum { Value = sizeof(Test(ChildPtr())) == sizeof(Yes) };
};

// [Is Base Of]
// * Checks whether specific type is base of other type

template<typename BaseType, typename DerivedType>
struct TIsBaseOf : TIsDerivedFrom<DerivedType, BaseType> {};

// [Is Same]
// * Checks whether specified types are the same

template<typename T, typename R> struct TIsSame { enum { Value = false }; };
template<typename T> struct TIsSame<T, T> { enum { Value = true }; };

// [Is Castable]
// * Checks whether specific types could be casted to each other

template<typename T, typename R>
struct TIsCastable { enum { Value = TIsDerivedFrom<T, R>::Value || TIsBaseOf<T, R>::Value }; };

// [Get Nth type]
// * Gets Nth type from parameter pack

template<uint32 N, typename T, typename... ArgTypes> struct TGetNthType { typedef typename TGetNthType<N - 1, ArgTypes...>::Type Type; };
template<typename T, typename... ArgTypes> struct TGetNthType<0, T, ArgTypes...> { typedef T Type; };


// [Is arithmetic]
// * Checks whether specific type is arithmetic

template <typename T>
struct TIsArithmetic
{
	enum { Value =
			TIsIntegral<T>::Value ||
			TIsFloating<T>::Value ||
			TIsCharacter<T>::Value ||
			TIsBool<T>::Value
	};
};

// [Decay]
// * Returns the decayed type
// * ie. applies array-to-pointer and function-to-pointer conversions

template<typename T>
struct TDecay
{
	typedef typename _NTypeInternals::TDecayHelper<
		typename TRemoveConstReference<T>::Type
	>::Type Type;
};

// [Pure]
// * Removes all qualifiers

template<typename T>
struct TPure
{
private:

	typedef typename TRemoveConstReference<T>::Type TestType;

public:

	typedef typename TChoose<
		TIsPointer<TestType>::Value, 
		typename TRemovePointer<TestType>::Type,
		typename _NTypeInternals::TDecayHelper<TestType>::Type
	>::Type Type;
};

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
	enum { IsSmallType = ((sizeof(T) <= sizeof(void*)) && TIsPODType<T>::Value ) || TIsArithmetic<T>::Value };

public:
	typedef typename _NTypeInternals::TCallTraitsHelper<T, IsSmallType>::Type Param;
	typedef typename _NTypeInternals::TCallTraitsHelper<T, IsSmallType>::ConstType ConstParam;
};

// [Limits]
// Tells value limit of specific integral type
// Similar to std::numeric_limits<T>

template<typename T>
struct TLimits
{
	static_assert(TIsIntegral<T>::Value, "TIntLimit accepts only integer types");
	static_assert(TIsSame<T, typename TPure<T>::Type>::Value, "TIntLimit accepts only pure types");

	static constexpr bool IsSigned = TIsSigned<T>::Value;

	static constexpr T Max = (T)((uint64)1 << (sizeof(T) * 8 - (IsSigned ? 1 : 0))) - 1;
	static constexpr T Min = IsSigned ? (-(int64)((uint64)1 << (sizeof(T) * 8 - 1))) : 0;
};

// [Type Traits]
// Tells information about the type

template<typename T>
struct TTypeTraits
{
	enum
	{
		IsFundamental = TIsFundamental<T>::Value,
		IsEnum = TIsEnum<T>::Value,

		HasDefaultConstructor = !TIsTriviallyConstructible<T>::Value,

		HasCopyConstructor = !TIsTriviallyCopyConstructible<T>::Value,
		HasMoveConstructor = !TIsTriviallyMoveConstructible<T>::Value,

		HasCopyAssign = !TIsTriviallyCopyAssignable<T>::Value,
		HasMoveAssign = !TIsTriviallyMoveAssignable<T>::Value,

		IsBitwiseCopyable = !HasCopyConstructor && !HasCopyAssign,
		IsBitwiseMovable = !HasMoveConstructor && !HasMoveAssign,
		IsBitwiseComparable = IsFundamental || IsEnum || !THasEqualOperator<T>::Value
	};
};
