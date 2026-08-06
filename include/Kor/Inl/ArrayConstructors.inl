// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorT>::TArray() noexcept
	: _data(nullptr)
	, _num(0)
	, _reservedNum(0)
{}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorT>::TArray(Init::SNoInit) noexcept
{}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(const TArray& other) noexcept
	: TArray()
{
	if (other._num <= 0) return;

	using namespace Internal::Array;
	SFriend::CopyToEmpty(*this, other);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorT>::TArray(TArray&& other) noexcept
	: TArray()
{
	if (other._num <= 0) return;

	using namespace Internal::Array;
	SFriend::MoveToEmpty(*this, Move(other));
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(const ILType& list) noexcept
	: TArray()
{
	// We expect that std::initializer_list's iterators are pointers
	// * In case which this assumption is incorrect, 
	// ** it will fail to compile rather than give undefined behaviour

	const SizeType newNum = (SizeType)list.size();
	if (newNum <= 0) return;

	using namespace Internal::Array;
	SFriend::CopyToEmpty(*this, list.begin(), newNum);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(const ElementType* data, SizeType num) noexcept
	: TArray()
{
	if (num <= 0) return;

	using namespace Internal::Array;
	SFriend::CopyToEmpty(*this, data, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, Init::SNoInit) noexcept
	: TArray()
{
	if (num <= 0) return;

	using namespace Internal::Array;
	SFriend::InitFromEmpty<Init::SNoInit>(*this, 0, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, Init::SDefault) noexcept
	: TArray()
{
	if (num <= 0) return;

	using namespace Internal::Array;
	SFriend::InitFromEmpty<Init::SDefault>(*this, num, 0);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, Init::SZero) noexcept
	: TArray()
{
	if (num <= 0) return;

	using namespace Internal::Array;
	SFriend::InitFromEmpty<Init::SZero>(*this, num, 0);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, const ElementType& value) noexcept
	: TArray()
{
	if (num <= 0) return;

	using namespace Internal::Array;
	SFriend::InitFromEmpty<Init::SNoInit>(*this, 0, num);

	for (SizeType i = 0; i < num; ++i)
	{
		SMemoryOps::Construct(&_data[i], value);
	}
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::~TArray() noexcept
{
	if (_num <= 0) return;

	using namespace Internal::Array;
	SFriend::Empty(*this);
}
