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
	if (num <= 0 || num == _num) return;
	SFriend::ResizeDefault(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::ResizeZeroed(SizeType num) noexcept
{
	if (num <= 0 || num == _num) return;
	SFriend::ResizeZero(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::ResizeUnitialized(SizeType num) noexcept
{
	if (num <= 0 || num == _num) return;
	SFriend::Resize(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::ShrinkToFit() noexcept
{
	if (_num == _reservedNum) return;
	SFriend::Reallocate(*this, _num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Reset() noexcept
{
	if (_num <= 0) return;
	SFriend::Reset(*this);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Empty(SizeType num) noexcept
{
	if (num > 0)
	{
		SFriend::ReallocateToEmpty(*this, num);
	}
	else if (_reservedNum > 0)
	{
		SFriend::Empty(*this);
	}
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
	if (num > 0)
	{
		SFriend::ReallocateToEmpty(*this, num);
		SMemoryOps::FillConstruct(_data, val, num);
	}
	else if (_reservedNum > 0)
	{
		SFriend::Empty(*this);
	}
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Assign(const ElementType* data, SizeType num) noexcept
{
	if (data && num > 0)
	{
		SFriend::ReallocateToEmpty(*this, num);
		SMemoryOps::FillConstruct(_data, data, num);
	}
	else if (_reservedNum > 0)
	{
		SFriend::Empty(*this);
	}
}
