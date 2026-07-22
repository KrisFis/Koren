// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorT>::TArray() noexcept
	: _allocator()
	, _data(nullptr)
	, _num(0)
	, _reservedNum(0)
{}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorT>::TArray(Init::SNoInit) noexcept
{}

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
