// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Reserve(SizeType num) noexcept
{
	if (num <= _reservedNum) return;
	SFriend::Reallocate(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Resize(SizeType num) noexcept
{
	SFriend::Resize<Init::SDefault>(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::ResizeZeroed(SizeType num) noexcept
{
	SFriend::Resize<Init::SZero>(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::ResizeUnitialized(SizeType num) noexcept
{
	SFriend::Resize<Init::SNoInit>(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::ShrinkToFit() noexcept
{
	if (_num == _reservedNum) return;
	SFriend::ReallocateRaw(*this, _num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Reset() noexcept
{
	if (_num <= 0) return;

	SMemoryOps::Destruct(_data, _num);
	_num = 0;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Empty(SizeType num) noexcept
{
	Reset();

	if (num == _reservedNum) return;
	SFriend::ReallocateRaw(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Fill(const ElementType& val) noexcept
{
	if (_num == 0) return;
	SMemoryOps::FillAssign(_data, val, _num);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Assign(const ElementType& val, SizeType num) noexcept
{
	SFriend::Resize(*this, num);
	SMemoryOps::FillConstruct(_data, val, num);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Assign(const ElementType* data, SizeType num) noexcept
{
	SFriend::CheckPointer(data);

	SFriend::Resize(*this, num);
	SMemoryOps::CopyConstruct(_data, data, num);
}
