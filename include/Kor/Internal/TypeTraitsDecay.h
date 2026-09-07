// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

KOR_NAMESPACE_BEGIN

namespace Internal
{
	template<typename T>
	struct TDecayHelper	{ typedef T Type; };

	template <typename T>
	struct TDecayHelper<T[]> { typedef T* Type; };

	template <typename T, TSize N>
	struct TDecayHelper<T[N]> { typedef T* Type; };

	template <typename T, typename... Params>
	struct TDecayHelper<T(Params...)> { typedef T (*Type)(Params...); };

	template <typename T, bool IsSmall>
	struct TCallTraitsHelper
	{
		typedef const T& Type;
		typedef const T& ConstType;
	};

	template <typename T>
	struct TCallTraitsHelper<T, true>
	{
		typedef const T Type;
		typedef const T ConstType;
	};

	template <typename T>
	struct TCallTraitsHelper<T*, true>
	{
		typedef T* Type;
		typedef const T* ConstType;
	};
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

KOR_NAMESPACE_END