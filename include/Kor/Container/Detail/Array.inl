// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::AllocatorType& TArray<ElementT, AllocatorFamilyT>::GetAllocator() noexcept
{
	return _allocator;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const TArray<ElementT, AllocatorFamilyT>::AllocatorType& TArray<ElementT, AllocatorFamilyT>::GetAllocator() const noexcept
{
	return _allocator;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::GetData() noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::GetData() const noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::GetNum() const noexcept
{
	return _num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::GetReservedNum() const noexcept
{
	return _reservedNum;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::IsEmpty() const noexcept
{
	return _num == 0;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::IsValidIndex(SizeType idx) const noexcept
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

template<typename ElementT, typename AllocatorFamilyT>
struct TContainerTraits<TArray<ElementT, AllocatorFamilyT>>
	: TContainerTraitsBase<TArray<ElementT, AllocatorFamilyT>>
{
	using Type = TArray<ElementT, AllocatorFamilyT>;

	using ElementType = ElementT;
	using AllocatorFamilyType = AllocatorFamilyT;
	using SizeType = typename Type::SizeType;

	enum { InlineMemory = true };
};