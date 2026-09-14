// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Composite.h"
#include "Kor/TypeTrait/Property.h"
#include "Kor/TypeTrait/Qualifier.h"

KOR_NAMESPACE_BEGIN
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

// [TInt]
// * Maps a byte size to its corresponding signed and unsigned integer types
// * Primary source of truth for integer type resolution by size
// * Unsupported sizes will result in a compile error (incomplete type)

template<TSize Size> struct TInt { static_assert(Size == 0, "TInt: unsupported size, must be 1, 2, 4 or 8"); };
template<> struct TInt<1> { typedef int8 Signed; typedef uint8 Unsigned; };
template<> struct TInt<2> { typedef int16 Signed; typedef uint16 Unsigned; };
template<> struct TInt<4> { typedef int32 Signed; typedef uint32 Unsigned; };
template<> struct TInt<8> { typedef int64 Signed; typedef uint64 Unsigned; };

// [TMakeUnsigned]
// * Convenience wrapper around TInt - resolves to unsigned integer of given size
// * Example: TMakeUnsigned<int32>::Type → uint32

template<typename T>
struct TMakeUnsigned : TType<typename TInt<sizeof(T)>::Unsigned> {};

// [TMakeSigned]
// * Convenience wrapper around TInt - resolves to signed integer of given size
// * Example: TMakeSigned<uint32>::Type → int32

template<typename T>
struct TMakeSigned : TType<typename TInt<sizeof(T)>::Signed> {};

KOR_NAMESPACE_END
