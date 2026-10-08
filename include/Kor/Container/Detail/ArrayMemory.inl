// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Reserve(SizeType num) noexcept
{
	if (num <= _reservedNum) return;
	SFriend::Grow(*this, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::ReserveExact(SizeType num) noexcept
{
	if (num <= _reservedNum) return;
	SFriend::Reallocate(*this, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SetNum(SizeType num) noexcept
{
	if (num == _num) return;

	if (num > 0)
	{
		// TODO: Apply geometric growth
		const SizeType oldNum = _num;
		SFriend::Resize(*this, num);

		if (num > oldNum)
		{
			SMemoryOps::DefaultConstruct(
				_data + oldNum,
				num - oldNum
			);
		}
	}
	else if (_num > 0)
	{
		SFriend::Destruct(*this);
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SetNumZeroed(SizeType num) noexcept
{
	if (num == _num) return;

	if (num > 0)
	{
		// TODO: Apply geometric growth
		const SizeType oldNum = _num;
		SFriend::Resize(*this, num);

		if (num > oldNum)
		{
			SMemoryOps::ZeroConstruct(
				_data + oldNum,
				num - oldNum
			);
		}
	}
	else if (_num > 0)
	{
		SFriend::Destruct(*this);
	}
}
template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SetNumUninitialized(SizeType num) noexcept
{
	if (num == _num) return;

	if (num > 0)
	{
		// TODO: Apply geometric growth
		SFriend::Resize(*this, num);
	}
	else if (_num > 0)
	{
		SFriend::Destruct(*this);
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::ShrinkToFit() noexcept
{
	if (_num == _reservedNum) return;
	SFriend::Reallocate(*this, _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Reset() noexcept
{
	if (_num == 0) return;
	SFriend::Destruct(*this);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Empty(SizeType num) noexcept
{
	if (_num == 0) return;

	if (num > 0)
	{
		SFriend::EmptyAndReallocate(*this, num);
	}
	else if (_reservedNum > 0)
	{
		SFriend::Deallocate(*this);
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Fill(const ElementType& val) noexcept
{
	if (_num == 0) return;
	SMemoryOps::FillAssign(_data, val, _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Assign(const TArray& other) noexcept
{
	if (this == &other) return;
	SFriend::CopyFromOther(*this, other);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Assign(TArray&& other) noexcept
{
	if (this == &other) return;
	SFriend::MoveFromOther(*this, Move(other));
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Assign(const ElementType& val, SizeType num) noexcept
{
	if (num > 0)
	{
		SFriend::EmptyAndResize(*this, num);
		SMemoryOps::FillConstruct(_data, val, num);
	}
	else if (_reservedNum > 0)
	{
		SFriend::Deallocate(*this);
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Assign(const ElementType* data, SizeType num) noexcept
{
	if (num > 0)
	{
		KOR_ASSERT(data);

		SFriend::EmptyAndResize(*this, num);
		SMemoryOps::CopyConstruct(_data, data, num);
	}
	else if (_reservedNum > 0)
	{
		SFriend::Deallocate(*this);
	}
}
