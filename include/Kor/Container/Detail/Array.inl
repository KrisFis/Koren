// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::ElementAllocatorType& TArray<ElementT, AllocatorT>::GetAllocator() noexcept
{
	return _allocator;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE const TArray<ElementT, AllocatorT>::ElementAllocatorType& TArray<ElementT, AllocatorT>::GetAllocator() const noexcept
{
	return _allocator;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorT>::GetData() noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorT>::GetData() const noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::GetNum() const noexcept
{
	return _num;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::GetReservedNum() const noexcept
{
	return _reservedNum;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::IsEmpty() const noexcept
{
	return _num == 0;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::IsValidIndex(SizeType idx) const noexcept
{
	return SMathOps::IsWithin(idx, 0, _num - 1);
}

#include "Kor/Container/Detail/ArrayPrivate.inl"
#include "Kor/Container/Detail/ArrayConstructors.inl"
#include "Kor/Container/Detail/ArrayQuery.inl"
#include "Kor/Container/Detail/ArrayMemory.inl"
#include "Kor/Container/Detail/ArrayMutation.inl"
#include "Kor/Container/Detail/ArrayIterators.inl"
#include "Kor/Container/Detail/ArrayOperators.inl"

// Container traits
// -------------------------------------------------------------------------

template<typename ElementT, typename AllocatorT>
struct TContainerTraits<TArray<ElementT, AllocatorT>>
	: TContainerTraitsBase<TArray<ElementT, AllocatorT>>
{
	using ElementType = ElementT;
	using AllocatorType = AllocatorT;
	using SizeType = TAllocatorTraits<AllocatorT>::SizeType;

	enum { InlineMemory = true };
};