// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/Allocator.h"
#include "Kor/Memory/Bytes.h"
#include "Kor/Memory/TypedAllocator.h"

#include "Kor/Math/MathOps.h"

KOR_NAMESPACE_BEGIN

template<uint32 NumLimit>
class TFixedAllocator
{
public:
	KOR_FORCEINLINE void* Allocate(int32 bytes) noexcept { return GetAllocation(bytes); }
	KOR_FORCEINLINE void* Reallocate(void* ptr, int32 bytes) noexcept { return GetAllocation(bytes); }
	KOR_FORCEINLINE void Deallocate(void* ptr) noexcept {}

private:
	void* GetAllocation(int32 bytes) noexcept
	{
		KOR_EXPECT(SMathOps::IsWithin(bytes, 0, (int32)NumLimit));
		return (void*)&_data;
	}

	TBytes<NumLimit> _data;
};

template<uint32 NumLimit, typename ElementT>
class TTypedAllocator<TFixedAllocator<NumLimit>, ElementT>
	: public TTypedAllocatorBase<TFixedAllocator<NumLimit>, ElementT>
{
public:
	TTypedAllocator() = default;

	TTypedAllocator(const TTypedAllocator&) = delete;
	TTypedAllocator& operator=(const TTypedAllocator&) = delete;

	KOR_FORCEINLINE ElementT* Allocate(int32 num) noexcept { return GetAllocation(num); }
	KOR_FORCEINLINE ElementT* Reallocate(ElementT* ptr, int32 newNum) noexcept { return GetAllocation(newNum); }
	KOR_FORCEINLINE void Deallocate(ElementT* ptr) noexcept {}

private:
	ElementT* GetAllocation(int32 num) noexcept
	{
		// Ensure that expected allocation can fit
		KOR_EXPECT(SMathOps::IsWithin(num, 0, (int32)NumLimit));
		return *_data[0];
	}

	TTypedBytes<ElementT> _data[NumLimit];
};

KOR_NAMESPACE_END