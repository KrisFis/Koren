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
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(const TArray& other) noexcept
{
	if constexpr (!TIsEmpty<ElementAllocatorType>::Value)
	{
		_allocator = other._allocator;
	}

	if (other._num > 0)
	{
		_data = _allocator.Allocate(other._num);
		KOR_ASSERT(_data);

		_num = other._num;
		_reservedNum = other._num;

		SMemoryOps::CopyAs(_data, other._data, other._num);
	}
	else
	{
		_data = nullptr;
		_num = 0;
		_reservedNum = 0;
	}
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorT>::TArray(TArray&& other) noexcept
{
	if constexpr (!TIsEmpty<ElementAllocatorType>::Value)
	{
		_allocator = Move(other._allocator);
	}

	_data = other._data;
	_num = other._num;
	_reservedNum = other._reservedNum;

	other._data = nullptr;
	other._num = 0;
	other._reservedNum = 0;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(const ILType& list) noexcept
	: TArray(list.begin(), (SizeType)list.size())
{
	// We expect that std::initializer_list's iterators are pointers
	// * In case which this assumption is incorrect, 
	// ** it will fail to compile rather than give undefined behaviour
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(const ElementType* data, SizeType num) noexcept
	: TArray()
{
	if (!data || num <= 0) return;

	_data = _allocator.Allocate(num);
	KOR_ASSERT(_data);

	_num = num;
	_reservedNum = num;

	SMemoryOps::CopyAs(_data, data, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, Init::SNoInit) noexcept
	: TArray()
{
	if (num <= 0) return;

	_reservedNum = num;
	_data = _allocator.Allocate(num);
	KOR_ASSERT(_data);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, Init::SDefault) noexcept
	: TArray()
{
	if (num <= 0) return;

	_data = _allocator.Allocate(num);
	KOR_ASSERT(_data);

	_num = num;
	_reservedNum = num;

	SMemoryOps::DefaultConstruct(_data, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, Init::SZero) noexcept
	: TArray()
{
	if (num <= 0) return;

	_data = _allocator.Allocate(num);
	KOR_ASSERT(_data);

	_num = num;
	_reservedNum = num;

	SMemoryOps::ZeroAs(_data, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, const ElementType& value) noexcept
	: TArray()
{
	if (num <= 0) return;

	_data = _allocator.Allocate(num);
	KOR_ASSERT(_data);

	_num = num;
	_reservedNum = num;

	for (SizeType i = 0; i < num; ++i)
	{
		SMemoryOps::CopyConstruct(&_data[i], &value);
	}
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::~TArray() noexcept
{
	if (!_data) return;

	SMemoryOps::Destruct(_data, _num);
	_allocator.Deallocate(_data);
}
