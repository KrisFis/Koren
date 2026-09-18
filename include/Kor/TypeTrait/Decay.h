// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

#include "Kor/TypeTrait/Detail/DecayHelper.h"

KOR_NAMESPACE_BEGIN

// [Decay]
// * Returns the decayed type
// * ie. applies array-to-pointer and function-to-pointer conversions

template<typename T>
using TDecay = Detail::TDecayImpl<T>;

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
