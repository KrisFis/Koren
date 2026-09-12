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
	if (other._num == 0) return;
	SFriend::CopyFromOther<false>(*this, other);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorT>::TArray(TArray&& other) noexcept
	: TArray()
{
	if (other._num == 0) return;
	SFriend::MoveFromOther<false>(*this, Move(other));
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(const ILType& list) noexcept
	: TArray()
{
	// We expect that std::initializer_list's iterators are pointers
	// * In case which this assumption is incorrect, 
	// ** it will fail to compile rather than give undefined behaviour

	const SizeType num = (SizeType)list.size();
	if (num == 0) return;

	SFriend::Resize<false>(*this, num);
	SFriend::CopyConstruct(_data, list.begin(), num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, Init::SNoInit) noexcept
	: TArray()
{
	if (num <= 0) return;
	SFriend::Resize<false>(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, Init::SDefault) noexcept
	: TArray()
{
	if (num == 0) return;
	SFriend::Resize<false>(*this, num);
	SMemoryOps::DefaultConstruct(_data, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(SizeType num, Init::SZero) noexcept
	: TArray()
{
	if (num == 0) return;
	SFriend::Resize<false>(*this, num);
	SMemoryOps::ZeroConstruct(_data, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(const ElementType* data, SizeType num) noexcept
	: TArray()
{
	if (num == 0) return;
	SFriend::Resize<false>(*this, num);
	SFriend::CopyConstruct(_data, data, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::TArray(const ElementType& val, SizeType num) noexcept
	: TArray()
{
	if (num == 0) return;
	SFriend::Resize<false>(*this, num);
	SMemoryOps::FillConstruct(_data, val, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE TArray<ElementT, AllocatorT>::~TArray() noexcept
{
	if (_reservedNum == 0) return;
	SFriend::Deallocate(*this);
}
