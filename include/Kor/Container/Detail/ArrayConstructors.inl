// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorFamilyT>::TArray() noexcept
	: _data(nullptr)
	, _num(0)
	, _reservedNum(0)
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator) noexcept
	: _allocator(Move(allocator))
	, _data(nullptr)
	, _num(0)
	, _reservedNum(0)
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE consteval TArray<ElementT, AllocatorFamilyT>::TArray(Init::SConstEval) noexcept
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const TArray& other) noexcept
	: TArray()
{
	if (other._num == 0) return;
	SFriend::template CopyFromOther<false>(*this, other);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorFamilyT>::TArray(TArray&& other) noexcept
	: TArray()
{
	if (other._num == 0) return;
	SFriend::template MoveFromOther<false>(*this, Move(other));
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const ILType& list) noexcept
	: TArray()
{
	const SizeType num = (SizeType)list.size();
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::CopyConstruct(_data, list.begin(), num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ILType& list) noexcept
	: TArray(Move(allocator))
{
	const SizeType num = (SizeType)list.size();
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::CopyConstruct(_data, list.begin(), num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(SizeType num, Init::SNoInit) noexcept
	: TArray()
{
	if (num <= 0) return;

	SFriend::template Reallocate<false>(*this, num);
	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, SizeType num, Init::SNoInit) noexcept
	: TArray(Move(allocator))
{
	if (num <= 0) return;

	SFriend::template Reallocate<false>(*this, num);
	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(SizeType num, Init::SDefault) noexcept
	: TArray()
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::DefaultConstruct(_data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, SizeType num, Init::SDefault) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::DefaultConstruct(_data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(SizeType num, Init::SZero) noexcept
	: TArray()
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::ZeroConstruct(_data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, SizeType num, Init::SZero) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::ZeroConstruct(_data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const ElementType* data, SizeType num) noexcept
	: TArray()
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::CopyConstruct(_data, data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ElementType* data, SizeType num) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::CopyConstruct(_data, data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const ElementType& val, SizeType num) noexcept
	: TArray()
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::FillConstruct(_data, val, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ElementType& val, SizeType num) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::FillConstruct(_data, val, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::~TArray() noexcept
{
	if (_reservedNum == 0) return;
	SFriend::Deallocate(*this);
}
