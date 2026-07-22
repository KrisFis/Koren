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
	if (this != &other) [[ likely ]]
	{
		CopyFrom(other._data, other._num);
	}
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::TArray(TArray&& other) noexcept
{
	if (this != &other) [[ likely ]]
	{
		MoveFrom(other._data, other._num);
	}
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::TArray(SizeType num, bool reserveOnly) noexcept
{
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::TArray(const ILType& list) noexcept
{
	// We expect that std::initializer_list's iterators are pointers
	// * In case which this assumption is incorrect, 
	// ** it will fail to compile rather than give undefined behaviour
	CopyFrom(list.begin(), (SizeType)list.size());
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::TArray(const ElementType* data, SizeType num) noexcept
{
	KOR_ASSERT_DEBUG(!!data && num > 0);
	CopyFrom(data, num);
}

template<typename ElementT, typename AllocatorT>
TArray<ElementT, AllocatorT>::~TArray() noexcept
{
	Destruct(_data, _num);
	_allocator.Deallocate(_data);
}
