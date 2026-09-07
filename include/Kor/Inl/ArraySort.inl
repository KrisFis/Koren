// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Sort() noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::SortByFunc(FunctorT&& func)
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::StableSort() noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::StableSortByFunc(FunctorT&& func)
{
	TODO_IMPLEMENT()
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