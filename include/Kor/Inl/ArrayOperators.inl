// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>& TArray<ElementT, AllocatorT>::operator=(const TArray& other) noexcept
{
	if (this == &other) return;

	SFriend::Empty(*this);
	if (other._num > 0)
	{
		SFriend::CopyToEmpty(*this, other);
	}
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>& TArray<ElementT, AllocatorT>::operator=(TArray&& other) noexcept
{
	if (this == &other) return;

	SFriend::Empty(*this);
	if (other._num > 0)
	{
		SFriend::MoveToEmpty(*this, other);
	}
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE TArray<ElementT, AllocatorT>& TArray<ElementT, AllocatorT>::operator=(const ILType& list) noexcept
{
	SFriend::Empty(*this);
	if (const SizeType newNum = (SizeType)list.size(); newNum > 0)
	{
		SFriend::CopyToEmpty(*this, list.begin(), newNum);
	}
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE bool TArray<ElementT, AllocatorT>::operator==(const TArray& other) const noexcept
{
	return 
		_num == other._num && 
		SMemoryOps::IsEqualAs(_data, other._data, _num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::operator!=(const TArray& other) const noexcept
{
	return !operator==(other);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename ElementT* TArray<ElementT, AllocatorT>::operator*() noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE const typename ElementT* TArray<ElementT, AllocatorT>::operator*() const noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::operator[](SizeType idx) noexcept
{
	SFriend::CheckRange(*this, idx);
	return _data[idx];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE const ElementT& TArray<ElementT, AllocatorT>::operator[](SizeType idx) const noexcept
{
	SFriend::CheckRange(*this, idx);
	return _data[idx];
}