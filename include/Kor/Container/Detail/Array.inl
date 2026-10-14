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
	return SMathOps::IsWithin(idx, (SizeType)0, _num - 1);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::View() noexcept
{
	return TArrayView<ElementType, SizeType>(_data, _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::View() const noexcept
{
	return TConstArrayView<ElementType, SizeType>(_data, _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::SubView(SizeType idx, SizeType num) noexcept
{
	KOR_ASSERT(idx + num <= _num);
	return TArrayView<ElementType, SizeType>(_data + idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::SubView(SizeType idx, SizeType num) const noexcept
{
	KOR_ASSERT(idx + num <= _num);
	return TConstArrayView<ElementType, SizeType>(_data + idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::LeftView(SizeType num) noexcept
{
	KOR_ASSERT(num <= _num);
	return TArrayView<ElementType, SizeType>(_data, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::LeftView(SizeType num) const noexcept
{
	KOR_ASSERT(num <= _num);
	return TConstArrayView<ElementType, SizeType>(_data, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::RightView(SizeType num) noexcept
{
	KOR_ASSERT(num <= _num);
	return TArrayView<ElementType, SizeType>(_data + (_num - num), num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::RightView(SizeType num) const noexcept
{
	KOR_ASSERT(num <= _num);
	return TConstArrayView<ElementType, SizeType>(_data + (_num - num), num);
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