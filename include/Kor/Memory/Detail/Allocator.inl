// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename T>
KOR_FORCEINLINE T* CAllocator::Typed<T>::Allocate(SizeType num, uint32 alignment) noexcept
{
	static constexpr TSize elementSize = SizeOf<T, 1>();
	return (T*)SMemoryOps::Malloc(num * elementSize, alignment);
}

template<typename T>
KOR_FORCEINLINE T* CAllocator::Typed<T>::Reallocate(T* ptr, SizeType newNum, uint32 alignment) noexcept
{
	static constexpr TSize elementSize = SizeOf<T, 1>();
	return (T*)SMemoryOps::Realloc((void*)ptr, newNum * elementSize, alignment);
}

template<typename T>
KOR_FORCEINLINE void CAllocator::Typed<T>::Deallocate(T* ptr, uint32 alignment) noexcept
{
	SMemoryOps::Free((void*)ptr, alignment);
}

template<typename SizeType>
KOR_INLINE SizeType SDefaultAllocationPolicy::CalculateGrow(SizeType newNum, SizeType oldNum, uint32 bytesPerElement, uint32 alignment) noexcept
{
	static constexpr SizeType INIT = 4;
	static constexpr SizeType PADDING = KOR_DEFAULT_HEAP_ALIGNMENT; // in elements
	static constexpr SizeType FACTOR_NUM = 3;
	static constexpr SizeType FACTOR_DEN = 8;

	// First small allocation: start at INIT
	if (oldNum == 0 && newNum <= INIT)
	{
		return INIT;
	}

	// 64-bit intermediate so num * FACTOR_NUM can't overflow a 32-bit SizeT
	const uint64 n = (uint64)newNum;
	const uint64 result = n + (n * FACTOR_NUM) / FACTOR_DEN + PADDING;

	return (SizeType)SMathOps::Clamp<uint64>(result, 0, TLimits<SizeType>::Max);
}

template<typename SizeType>
KOR_INLINE SizeType SDefaultAllocationPolicy::CalculateShrink(SizeType newNum, SizeType oldNum, uint32 bytesPerElement, uint32 alignment) noexcept
{
	// num <= oldNum, so slack is oldNum - num
	const uint64 slack = (uint64)oldNum - (uint64)newNum;
	const uint64 slackBytes = slack * bytesPerElement;

	// Keep the block unless enough memory would be reclaimed
	return (slackBytes >= KOR_BUFFER_SIZE_LARGE) ? newNum : oldNum;
}