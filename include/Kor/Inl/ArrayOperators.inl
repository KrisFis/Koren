// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>& TArray<ElementT, AllocatorT>::operator=(const TArray& other) noexcept
{
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>& TArray<ElementT, AllocatorT>::operator=(TArray&& other) noexcept
{
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>& TArray<ElementT, AllocatorT>::operator=(const ILType& list) noexcept
{
}

template<typename ElementT, typename AllocatorT>
bool TArray<ElementT, AllocatorT>::operator==(const TArray& other) const noexcept
{
}

template<typename ElementT, typename AllocatorT>
bool TArray<ElementT, AllocatorT>::operator!=(const TArray& other) const noexcept
{
}

template<typename ElementT, typename AllocatorT>
const typename ElementT* TArray<ElementT, AllocatorT>::operator*() const noexcept
{
}

template<typename ElementT, typename AllocatorT>
typename ElementT* TArray<ElementT, AllocatorT>::operator*() noexcept
{
}

template<typename ElementT, typename AllocatorT>
ElementT& TArray<ElementT, AllocatorT>::operator[](SizeType idx) noexcept
{
}

template<typename ElementT, typename AllocatorT>
const ElementT& TArray<ElementT, AllocatorT>::operator[](SizeType idx) const noexcept
{
}