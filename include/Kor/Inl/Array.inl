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
	return SMath::IsWithin(idx, 0, _num);
}

#include "Kor/Inl/ArrayPrivate.inl"
#include "Kor/Inl/ArrayConstructors.inl"
#include "Kor/Inl/ArrayQuery.inl"
#include "Kor/Inl/ArrayMemory.inl"
#include "Kor/Inl/ArrayMutation.inl"
#include "Kor/Inl/ArrayIterators.inl"
#include "Kor/Inl/ArrayOperators.inl"

// Container traits
// -------------------------------------------------------------------------

template<typename ElementT, typename AllocatorT>
struct TContainerTraits<TArray<ElementT, AllocatorT>>
	: TContainerTraitsBase<TArray<ElementT, AllocatorT>>
{
	using ElementType = ElementT;
	using AllocatorType = AllocatorT;
	using SizeType = TAllocatorTraits<AllocatorT>::SizeType;
};