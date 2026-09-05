// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Sort() noexcept
{
	KOR_ASSERT(false);
}

template<typename ElementT, typename AllocatorT>
template<typename Predicate>
KOR_INLINE void TArray<ElementT, AllocatorT>::Sort(Predicate&& predicate)
{
	KOR_ASSERT(false);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::StableSort() noexcept
{
	KOR_ASSERT(false);
}

template<typename ElementT, typename AllocatorT>
template<typename Predicate>
KOR_INLINE void TArray<ElementT, AllocatorT>::StableSort(Predicate&& predicate)
{
	KOR_ASSERT(false);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Reverse() noexcept
{
	// TODO: We can make swap range-aware

	ElementType* left = _data;
	ElementType* right = _data + _num - 1;
	while (left < right)
	{
		SMemoryOps::SwapAs(left, right, 1);
		++left;
		--right;
	}
}