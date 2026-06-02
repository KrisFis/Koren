// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"

#include <cmath>

// Definitions
///////////////////////////////////////////////////////////

// KOR_MATH_PI = 3.1415926535897932384626433832795
#define KOR_MATH_PI 3.1415926535897932f

#define KOR_MATH_TINY_NUMBER 1.e-8f
#define KOR_MATH_SMALL_NUMBER 1.e-4f

// Types
///////////////////////////////////////////////////////////

KOR_NAMESPACE_BEGIN

struct SMath
{
	template <typename T>
	KOR_FORCEINLINE static constexpr bool IsPowerOfTwo(T Value) noexcept { return ((Value & (Value - 1)) == (T)0); }

	template <typename T>
	KOR_FORCEINLINE static constexpr bool IsWithin(T Value, T Min, T Max) noexcept { return !(Value > Max || Value < Min); }

	template<typename T>
	KOR_FORCEINLINE static constexpr T Min(T ValueA, T ValueB) noexcept { return ValueA > ValueB ? ValueB : ValueA; }

	template<typename T>
	KOR_FORCEINLINE static constexpr T Max(T ValueA, T ValueB) noexcept { return ValueA > ValueB ? ValueA : ValueB; }

	template<typename T>
	KOR_FORCEINLINE static constexpr T Clamp(T Value, T Min, T Max) noexcept { return Value > Max ? Max : (Min > Value ? Min : Value); }

	template<typename T>
	KOR_FORCEINLINE static constexpr T Abs(T Value) noexcept { return Value >= (T)0 ? Value : -Value; }

	template<typename T>
	KOR_FORCEINLINE static constexpr T Lerp(T Value, T Low, T High) noexcept { return (1 - Value) * Low + Value * High; }

	template<typename T>
	KOR_FORCEINLINE static constexpr T Map(T Value, T LowA, T HighA, T LowB, T HighB) noexcept { return Lerp((Value - LowA) / (HighA - LowA), LowB, HighB); }

	template<typename T>
	KOR_FORCEINLINE static T Floor(T Value) noexcept { return floor(Value); }

	template<typename T>
	KOR_FORCEINLINE static T Ceil(T Value) noexcept { return ceil(Value); }

	template<typename T>
	KOR_FORCEINLINE static T Pow(T Value, T Exponent) noexcept { return pow(Value, Exponent); }

	template<typename T>
	KOR_FORCEINLINE static T Sqrt(T Value) noexcept { return sqrt(Value); }

	template<typename T>
	KOR_FORCEINLINE static T LogE(T Value) noexcept { return log(Value); }

	template<typename T>
	KOR_FORCEINLINE static T Log2(T Value) noexcept { return log2(Value); }

	template<typename T>
	KOR_FORCEINLINE static T LogX(T Base, T Value) noexcept { return LogE(Value) / LogE(Base); }

	KOR_FORCEINLINE static constexpr uint32 CountLeadingZeros(uint32 Value) noexcept { return (Value == 0) ? 32 : (31 - FloorLog2(Value)); }
	KOR_FORCEINLINE static constexpr uint64 CountLeadingZeros(uint64 Value) noexcept { return (Value == 0) ? 64 : (63 - FloorLog2(Value)); }

	KOR_FORCEINLINE static constexpr uint32 CeilToPowerOfTwo(uint32 Value) noexcept { return (uint32)1 << CeilLog2(Value); }
	KOR_FORCEINLINE static constexpr uint32 CeilLog2(uint32 Value) noexcept { return (32 - CountLeadingZeros(Value - 1)) & (~(((int32)(CountLeadingZeros(Value) << 26)) >> 31)); }

	KOR_FORCEINLINE static constexpr uint32 CeilToPowerOfTwo(uint64 Value) noexcept { return (uint64)1 << CeilLog2(Value); }
	KOR_FORCEINLINE static constexpr uint64 CeilLog2(uint64 Value) noexcept { return (64 - CountLeadingZeros(Value - 1)) & (~(((int64)(CountLeadingZeros(Value) << 57)) >> 63)); }

	// see http://codinggorilla.domemtech.com/?p=81 or http://en.wikipedia.org/wiki/Binary_logarithm
	KOR_FORCEINLINE static constexpr uint32 FloorToPowerOfTwo(uint32 Value) noexcept { return (uint32)1 << FloorLog2(Value); }
	static constexpr uint32 FloorLog2(uint32 Value) noexcept
	{
		uint32 pos = 0;
		if (Value >= 1 << 16) { Value >>= 16; pos += 16; }
		if (Value >= 1 << 8) { Value >>= 8; pos += 8; }
		if (Value >= 1 << 4) { Value >>= 4; pos += 4; }
		if (Value >= 1 << 2) { Value >>= 2; pos += 2; }
		if (Value >= 1 << 1) { pos += 1; }
		return (Value == 0) ? 0 : pos;
	}

	// see http://codinggorilla.domemtech.com/?p=81 or http://en.wikipedia.org/wiki/Binary_logarithm
	KOR_FORCEINLINE static constexpr uint64 FloorToPowerOfTwo(uint64 Value) noexcept { return (uint64)1 << FloorLog2(Value); }
	static constexpr uint64 FloorLog2(uint64 Value) noexcept
	{
		uint64 pos = 0;
		if (Value >= 1ull << 32) { Value >>= 32; pos += 32; }
		if (Value >= 1ull << 16) { Value >>= 16; pos += 16; }
		if (Value >= 1ull << 8) { Value >>= 8; pos += 8; }
		if (Value >= 1ull << 4) { Value >>= 4; pos += 4; }
		if (Value >= 1ull << 2) { Value >>= 2; pos += 2; }
		if (Value >= 1ull << 1) { pos += 1; }
		return (Value == 0) ? 0 : pos;
	}
};

KOR_NAMESPACE_END