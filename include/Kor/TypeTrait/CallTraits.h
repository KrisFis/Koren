// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/TypeTrait/Qualifier.h"
#include "Kor/TypeTrait/Composite.h"
#include "Kor/TypeTrait/Property.h"

KOR_NAMESPACE_BEGIN

namespace Detail
{
	template<typename T, bool IsSmall>
	struct TCallTraitsHelper
	{
		using Type = const T&;
		using ConstType = const T&;
	};

	template<typename T>
	struct TCallTraitsHelper<T, true>
	{
		using Type = const T;
		using ConstType = const T;
	};

	template<typename T>
	struct TCallTraitsHelper<T*, true>
	{
		using Type = T*;
		using ConstType = const T*;
	};
}

// [Get type]
// * Gets type variations

template<typename T>
struct TTypeVariants
{
	using Type = typename TClean<T>::Type;

	using Reference = Type&;
	using ConstReference = const Type&;

	using Pointer = Type*;
	using ConstPointer = const Type*;
};

// [Call traits]
// * Determines which type will be used for call
// * Similar to boost's call_traits, ie. having info about optimizations of which type is used

template<typename T>
struct TCallTraits : TTypeVariants<T>
{
private:
	using Type = typename TTypeVariants<T>::Type;
	enum { IsSmallType = ((sizeof(Type) <= sizeof(void*)) && TIsPOD<Type>::Value) || TIsArithmetic<Type>::Value };

public:
	using Param = typename Detail::TCallTraitsHelper<Type, IsSmallType>::Type;
	using ConstParam = typename Detail::TCallTraitsHelper<Type, IsSmallType>::ConstType;
};

KOR_NAMESPACE_END
