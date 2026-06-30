// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
const ElementT* TArray<ElementT, AllocatorT>::GetData() const noexcept
{
	return _allocator.GetData();
}

template<typename ElementT, typename AllocatorT>
ElementT* TArray<ElementT, AllocatorT>::GetData() noexcept
{
	return _allocator.GetData();
}

template<typename ElementT, typename AllocatorT>
typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::GetNum() const noexcept
{
	return _num;
}

template<typename ElementT, typename AllocatorT>
typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::GetReservedNum() const noexcept
{
	return _allocator.GetNum();
}

// Property Getters, Validation, Get, Find Index, Find Element, Contains