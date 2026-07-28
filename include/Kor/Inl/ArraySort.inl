// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Sort() noexcept
{
}

template<typename ElementT, typename AllocatorT>
template<typename Predicate>
KOR_INLINE void TArray<ElementT, AllocatorT>::Sort(Predicate&& predicate)
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::StableSort() noexcept
{
}

template<typename ElementT, typename AllocatorT>
template<typename Predicate>
KOR_INLINE void TArray<ElementT, AllocatorT>::StableSort(Predicate&& predicate)
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Reverse() noexcept
{
}