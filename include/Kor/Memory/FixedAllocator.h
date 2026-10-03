// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/Bytes.h"

#include "Kor/Math/MathOps.h"

KOR_NAMESPACE_BEGIN

template<uint32 NumLimit>
struct TFixedAllocator
{
	using SizeType = int32;

	template<typename T>
	struct Typed
	{
		Typed() = default;

		Typed(const Typed&) = delete;
		Typed& operator=(const Typed&) = delete;

		KOR_FORCEINLINE T* Allocate(SizeType num) noexcept { return GetAllocation(num); }
		KOR_FORCEINLINE T* Reallocate(T*, SizeType newNum) noexcept { return GetAllocation(newNum); }
		KOR_FORCEINLINE void Deallocate(T*) noexcept {}

		KOR_FORCEINLINE SizeType CalculateGrow(SizeType newNum, SizeType oldNum) const noexcept { return NumLimit; }
		KOR_FORCEINLINE SizeType CalculateShrink(SizeType newNum, SizeType oldNum) const noexcept { return NumLimit; }

	private:
		T* GetAllocation(SizeType num) noexcept
		{
			// Ensure that expected allocation can fit
			KOR_EXPECT(SMathOps::IsWithin<int64>(num, 0, NumLimit));
			return *_data[0];
		}

		using BytesType = TChoose<TIsVoid<T>::Value,
			TTypedBytes<uint8, KOR_DEFAULT_HEAP_ALIGNMENT>,
			TTypedBytes<T>
		>::Type;

		BytesType _data[NumLimit];
	};

	using Untyped = Typed<void>;
};

KOR_NAMESPACE_END