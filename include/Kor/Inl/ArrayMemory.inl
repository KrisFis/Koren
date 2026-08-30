// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Reserve(SizeType num) noexcept
{
	KOR_ASSERT(num > 0);
	if (num <= _reservedNum) return;

	using namespace Internal::Array;
	SFriend::ReallocateExact(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Resize(SizeType num) noexcept
{
	KOR_ASSERT(num > 0);

	using namespace Internal::Array;
	SFriend::Resize<Init::SDefault>(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::ResizeZeroed(SizeType num) noexcept
{
	KOR_ASSERT(num > 0);

	using namespace Internal::Array;
	SFriend::Resize<Init::SZero>(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::ResizeUnitialized(SizeType num) noexcept
{
	KOR_ASSERT(num > 0);

	using namespace Internal::Array;
	SFriend::Resize<Init::SNoInit>(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::ShrinkToFit() noexcept
{
	if (_num == _reservedNum) return;

	SMemoryOps::Destruct(_data + _num, _reservedNum - _num);

	using namespace Internal::Array;
	SFriend::ReallocateExact(*this, _num);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Reset() noexcept
{
	if (_num <= 0) return;

	SMemoryOps::Destruct(_data, _num);
	_num = 0;
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Empty(SizeType num) noexcept
{
	KOR_ASSERT(num > 0);

	if (_num > 0)
	{
		SMemoryOps::Destruct(_data, _num);
		_num = 0;
	}

	if (num != _reservedNum)
	{
		using namespace Internal::Array;
		SFriend::ReallocateExact(*this, num);
	}
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Fill(const ElementType& val) noexcept
{
	if (_num <= 0) return;

	SMemoryOps::FillAssign(_data, val, num);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Assign(const ElementType& val, SizeType num) noexcept
{
	KOR_ASSERT(num > 0);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Assign(const ElementType* data, SizeType num) noexcept
{
	KOR_ASSERT(data && num > 0);
}
