// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

namespace KOR_NAMESPACE Internals
{
	template<typename T>
	struct TDecayHelper	{ typedef T Type; };

	template <typename T>
	struct TDecayHelper<T[]> { typedef T* Type; };

	template <typename T, uint32 N>
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