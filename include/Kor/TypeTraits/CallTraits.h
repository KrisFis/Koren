// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTraits/Minimal.h"
#include "Kor/TypeTraits/Qualifier.h"
#include "Kor/TypeTraits/Composite.h"
#include "Kor/TypeTraits/Property.h"

KOR_NAMESPACE_BEGIN

namespace Internal
{
	template<typename T, bool IsSmall>
	struct TCallTraitsHelper
	{
		typedef const T& Type;
		typedef const T& ConstType;
	};

	template<typename T>
	struct TCallTraitsHelper<T, true>
	{
		typedef const T Type;
		typedef const T ConstType;
	};

	template<typename T>
	struct TCallTraitsHelper<T*, true>
	{
		typedef T* Type;
		typedef const T* ConstType;
	};
}

// [Get type]
// * Gets type variations

template<typename T>
struct TGetType
{
	typedef typename TClean<T>::Type Value;

	typedef Value& Reference;
	typedef const Value& ConstReference;

	typedef Value* Pointer;
	typedef const Value* ConstPointer;
};

// [Call traits]
// * Determines which type will be used for call
// * Similar to boost's call_traits, ie. having info about optimizations of which type is used

template<typename T>
struct TCallTraits : TGetType<T>
{
private:
	enum { IsSmallType = ((sizeof(T) <= sizeof(void*)) && TIsPOD<T>::Value) || TIsArithmetic<T>::Value };

public:
	typedef typename Internal::TCallTraitsHelper<T, IsSmallType>::Type Param;
	typedef typename Internal::TCallTraitsHelper<T, IsSmallType>::ConstType ConstParam;
};

KOR_NAMESPACE_END
