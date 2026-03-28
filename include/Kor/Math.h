// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include <cmath>

// Definitions
///////////////////////////////////////////////////////////

#ifdef PI
	#undef PI
#endif

// PI = 3.1415926535897932384626433832795
#define PI 3.1415926535897932f

#define TINY_NUMBER 1.e-8f
#define SMALL_NUMBER 1.e-4f

// Types
///////////////////////////////////////////////////////////

struct SMath
{
	template<typename T>
	FORCEINLINE static T Min(T ValueA, T ValueB) { return ValueA > ValueB ? ValueB : ValueA; }

	template<typename T>
	FORCEINLINE static T Max(T ValueA, T ValueB) { return ValueA > ValueB ? ValueA : ValueB; }

	template<typename T>
	FORCEINLINE static T Clamp(T Value, T Min, T Max) { return Value > Max ? Max : (Min > Value ? Min : Value); }

	template<typename T>
	FORCEINLINE static T Abs(T Value) { return Value >= (T)0 ? Value : -Value; }

	template<typename T>
	FORCEINLINE static T Lerp(T Value, T Low, T High) { return (1 - Value) * Low + Value * High; }

	template<typename T>
	FORCEINLINE static T Map(T Value, T LowA, T HighA, T LowB, T HighB) { return Lerp((Value - LowA) / (HighA - LowA), LowB, HighB); }

	template<typename T>
	FORCEINLINE static T Floor(T Value) { return floor(Value); }

	template<typename T>
	FORCEINLINE static T Ceil(T Value) { return ceil(Value); }

	template<typename T>
	FORCEINLINE static T Pow(T Value, T Exponent) { return pow(Value, Exponent); }

	template<typename T>
	FORCEINLINE static T Sqrt(T Value) { return sqrt(Value); }

	template<typename T>
	FORCEINLINE static T LogE(T Value) { return log(Value); }

	template<typename T>
	FORCEINLINE static T Log2(T Value) { return log2(Value); }

	template<typename T>
	FORCEINLINE static T LogX(T Base, T Value) { return LogE(Value) / LogE(Base); }

	template <typename T>
	FORCEINLINE static bool IsPowerOfTwo(T Value) { return ((Value & (Value - 1)) == (T)0); }

	FORCEINLINE static uint32 CountLeadingZeros(uint32 Value) { return (Value == 0) ? 32 : (31 - FloorLog2(Value)); }
	FORCEINLINE static uint64 CountLeadingZeros(uint64 Value) { return (Value == 0) ? 64 : (63 - FloorLog2(Value)); }

	FORCEINLINE static uint32 CeilToPowerOfTwo(uint32 Value) { return (uint32)1 << CeilLog2(Value); }
	FORCEINLINE static uint32 CeilLog2(uint32 Value) { return (32 - CountLeadingZeros(Value - 1)) & (~(((int32)(CountLeadingZeros(Value) << 26)) >> 31)); }

	FORCEINLINE static uint32 CeilToPowerOfTwo(uint64 Value) { return (uint64)1 << CeilLog2(Value); }
	FORCEINLINE static uint64 CeilLog2(uint64 Value) { return (64 - CountLeadingZeros(Value - 1)) & (~(((int64)(CountLeadingZeros(Value) << 57)) >> 63)); }

	// see http://codinggorilla.domemtech.com/?p=81 or http://en.wikipedia.org/wiki/Binary_logarithm
	FORCEINLINE static uint32 FloorToPowerOfTwo(uint32 Value) { return (uint32)1 << FloorLog2(Value); }
	static uint32 FloorLog2(uint32 Value)
	{
		uint32 pos = 0;
		if (Value >= 1 << 16) { Value >>= 16; pos += 16; }
		if (Value >= 1 << 8) { Value >>=  8; pos +=  8; }
		if (Value >= 1 << 4) { Value >>=  4; pos +=  4; }
		if (Value >= 1 << 2) { Value >>=  2; pos +=  2; }
		if (Value >= 1 << 1) { pos +=  1; }
		return (Value == 0) ? 0 : pos;
	}

	// see http://codinggorilla.domemtech.com/?p=81 or http://en.wikipedia.org/wiki/Binary_logarithm
	FORCEINLINE static uint64 FloorToPowerOfTwo(uint64 Value) { return (uint64)1 << FloorLog2(Value); }
	static uint64 FloorLog2(uint64 Value)
	{
		uint64 pos = 0;
		if (Value >= 1ull << 32) { Value >>= 32; pos += 32; }
		if (Value >= 1ull << 16) { Value >>= 16; pos += 16; }
		if (Value >= 1ull << 8) { Value >>=  8; pos +=  8; }
		if (Value >= 1ull << 4) { Value >>=  4; pos +=  4; }
		if (Value >= 1ull << 2) { Value >>=  2; pos +=  2; }
		if (Value >= 1ull << 1) { pos +=  1; }
		return (Value == 0) ? 0 : pos;
	}
};
