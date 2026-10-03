// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>& TArray<ElementT, AllocatorFamilyT>::operator=(const TArray& other) noexcept
{
	if (this != &other)
	{
		SFriend::CopyFromOther(*this, other);
	}
	return *this;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>& TArray<ElementT, AllocatorFamilyT>::operator=(TArray&& other) noexcept
{
	if (this != &other)
	{
		SFriend::MoveFromOther(*this, Move(other));
	}
	return *this;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>& TArray<ElementT, AllocatorFamilyT>::operator=(const ILType& list) noexcept
{
	Assign(list.begin(), list.size());
	return *this;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE bool TArray<ElementT, AllocatorFamilyT>::operator==(const TArray& other) const noexcept
{
	return
		_num == other._num && 
		SMemoryOps::IsEqual(_data, other._data, _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::operator!=(const TArray& other) const noexcept
{
	return !operator==(other);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::operator*() noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::operator*() const noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::operator[](SizeType idx) noexcept
{
	return *GetAt(idx);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT& TArray<ElementT, AllocatorFamilyT>::operator[](SizeType idx) const noexcept
{
	return *GetAt(idx);
}