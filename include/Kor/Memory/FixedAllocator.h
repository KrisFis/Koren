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
	template<typename T>
	struct Typed
	{
		Typed() = default;

		Typed(const Typed&) = delete;
		Typed& operator=(const Typed&) = delete;

		KOR_FORCEINLINE T* Allocate(int32 num) noexcept { return GetAllocation(num); }
		KOR_FORCEINLINE T* Reallocate(T*, int32 num) noexcept { return GetAllocation(num); }
		KOR_FORCEINLINE void Deallocate(T*) noexcept {}

	private:
		T* GetAllocation(int32 num) noexcept
		{
			// Ensure that expected allocation can fit
			KOR_EXPECT(SMathOps::IsWithin(num, 0, (int32)NumLimit));
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