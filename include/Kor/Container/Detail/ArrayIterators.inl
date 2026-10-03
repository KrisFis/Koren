// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::IteratorType TArray<ElementT, AllocatorFamilyT>::begin() noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::ConstIteratorType TArray<ElementT, AllocatorFamilyT>::begin() const noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::IteratorType TArray<ElementT, AllocatorFamilyT>::end() noexcept
{
	return _data + _num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::ConstIteratorType TArray<ElementT, AllocatorFamilyT>::end() const noexcept
{
	return _data + _num;
}