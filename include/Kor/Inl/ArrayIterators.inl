// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::ArrayIteratorType TArray<ElementT, AllocatorT>::begin() noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::ConstArrayIteratorType TArray<ElementT, AllocatorT>::begin() const noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::ArrayIteratorType TArray<ElementT, AllocatorT>::end() noexcept
{
	return _data + _num;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::ConstArrayIteratorType TArray<ElementT, AllocatorT>::end() const noexcept
{
	return _data + _num;
}