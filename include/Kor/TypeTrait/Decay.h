// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/TypeTrait/Qualifier.h"

KOR_NAMESPACE_BEGIN

namespace Internal
{
	template<typename T>
	struct TDecayHelper { typedef T Type; };

	template<typename T>
	struct TDecayHelper<T[]> { typedef T* Type; };

	template<typename T, TSize N>
	struct TDecayHelper<T[N]> { typedef T* Type; };

	template<typename T, typename... Params>
	struct TDecayHelper<T(Params...)> { typedef T (*Type)(Params...); };
}

// [Decay]
// * Returns the decayed type
// * ie. applies array-to-pointer and function-to-pointer conversions

template<typename T>
struct TDecay : TType<
	typename Internal::TDecayHelper<
		typename TClean<T>::Type
	>::Type>
{};

// [Remove Extent]
// * Removes extent '[]' from the type

template<typename T> struct TRemoveExtent : TType<T> {};
template<typename T, TSize N> struct TRemoveExtent<T[N]> : TType<T> {};
template<typename T> struct TRemoveExtent<T[]> : TType<T> {};

// [Remove Pointer]
// * Removes pointer from provided type

template<typename T> struct TRemovePointer : TType<T> {};
template<typename T> struct TRemovePointer<T*> : TType<T> {};

KOR_NAMESPACE_END
