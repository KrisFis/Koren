// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_INLINE TArray<ElementT, AllocatorT>& TArray<ElementT, AllocatorT>::operator=(const TArray& other) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE TArray<ElementT, AllocatorT>& TArray<ElementT, AllocatorT>::operator=(TArray&& other) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE TArray<ElementT, AllocatorT>& TArray<ElementT, AllocatorT>::operator=(const ILType& list) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE bool TArray<ElementT, AllocatorT>::operator==(const TArray& other) const noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE bool TArray<ElementT, AllocatorT>::operator!=(const TArray& other) const noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE const typename ElementT* TArray<ElementT, AllocatorT>::operator*() const noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename ElementT* TArray<ElementT, AllocatorT>::operator*() noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT& TArray<ElementT, AllocatorT>::operator[](SizeType idx) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE const ElementT& TArray<ElementT, AllocatorT>::operator[](SizeType idx) const noexcept
{
}