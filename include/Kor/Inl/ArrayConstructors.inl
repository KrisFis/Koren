// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
constexpr TArray<ElementT, AllocatorT>::TArray() noexcept
{
}

template<typename ElementT, typename AllocatorT>
constexpr TArray<ElementT, AllocatorT>::TArray(Init::SNoInit) noexcept
{
}

template<typename ElementT, typename AllocatorT>
constexpr TArray<ElementT, AllocatorT>::TArray(Init::SZero) noexcept
{
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::TArray(const TArray& other) noexcept
{
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::TArray(TArray&& other) noexcept
{
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::TArray(SizeType num, bool reserveOnly) noexcept
{
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::TArray(const ILType& list) noexcept
{
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::TArray(const ElementType* data, SizeType num) noexcept
{
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::~TArray() noexcept
{
}
