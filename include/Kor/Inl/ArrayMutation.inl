// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Add(const ElementType& val) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this);
	SMemoryOps::CopyConstruct(_data + idx, &val);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Add(ElementType&& val) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this);
	SMemoryOps::MoveConstruct(_data + (_num - 1), &val);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::Add_GetRef(const ElementType& val) noexcept
{
	return _data[Add(val)];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::Add_GetRef(ElementType&& val) noexcept
{
	return _data[Add(Move(val))];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddUnique(const ElementType& val) noexcept
{
	if (const int32 foundIdx = FindIndex(val); foundIdx != KOR_INDEX_NONE)
	{
		return foundIdx;
	}

	return Add(val);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddUnique(ElementType&& val) noexcept
{
	if (const int32 foundIdx = FindIndex(val); foundIdx != KOR_INDEX_NONE)
	{
		return foundIdx;
	}

	return Add(Move(val));
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::AddUnique_GetRef(const ElementType& val) noexcept
{
	return _data[AddUnique(val)];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::AddUnique_GetRef(ElementType&& val) noexcept
{
	return _data[AddUnique(Move(val))];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddDefaulted(SizeType num) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this, num);
	SMemoryOps::DefaultConstruct(_data + idx, num);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::AddDefaulted_GetRef() noexcept
{
	return _data[AddDefaulted(num)];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddZeroed(SizeType num) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this, num);
	SMemoryOps::ZeroConstruct(_data + idx, num);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::AddZeroed_GetRef() noexcept
{
	return _data[AddZeroed()];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddUninitialized(SizeType num) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this, num);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::AddUninitialized_GetRef() noexcept
{
	return _data[AddUninitialized()];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Push(const ElementType& val) noexcept
{
	Add(val);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Push(ElementType&& val) noexcept
{
	Add(Move(val));
}

template<typename ElementT, typename AllocatorT>
template<typename ... ArgTypes>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Emplace(ArgTypes&&... args) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this);
	SMemoryOps::Construct(_data + idx, Forward<ArgTypes>(args)...);
	return idx;
}

template<typename ElementT, typename AllocatorT>
template<typename ... ArgTypes>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::Emplace_GetRef(ArgTypes&&... args) noexcept
{
	return _data[Emplace(Forward<ArgTypes>(args)...)];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Insert(SizeType idx, const ElementType& val) noexcept
{
	SFriend::Insert(*this, idx);
	SMemoryOps::CopyConstruct(_data + idx, &val);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Insert(SizeType idx, ElementType&& val) noexcept
{
	SFriend::Insert(*this, idx);
	SMemoryOps::MoveConstruct(data + idx, &val);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Insert(SizeType idx, const ElementType* data, SizeType num) noexcept
{
	SFriend::Insert(*this, idx, num);
	SMemoryOps::CopyConstruct(_data + idx, data, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::Insert(SizeType idx, const ILType& list) noexcept
{
	Insert(idx, list.begin(), list.size());
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Append(const TArray& other) noexcept
{
	if (other._num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _num;
	SFriend::AppendFromOther(*this, other);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Append(TArray&& other) noexcept
{
	if (other._num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _num;
	SFriend::AppendFromOther(*this, other);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Append(const ILType& list) noexcept
{
	const SizeType num = (SizeType)list.size();
	if (num == 0) return KOR_INDEX_NONE;

	const SizeType idx = num;
	SFriend::Add(*this, num);
	SMemoryOps::CopyConstruct(_data + idx, list.begin(), num);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Append(const ElementType& val, SizeType num) noexcept
{
	if (num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _num;
	SFriend::Add(*this, num);
	SMemoryOps::FillConstruct(_data + idx, val, num);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Append(const ElementType* data, SizeType num) noexcept
{
	if (num == 0) return KOR_INDEX_NONE;

	KOR_ASSERT(data);

	const SizeType idx = _num;
	SFriend::Add(*this, num);
	SMemoryOps::CopyConstruct(_data + idx, data, num);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Remove(const ElementType& val, bool allowShrink) noexcept
{
	const SizeType totalRemoved = SFriend::RemoveByFunc(*this,
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqualAs(&el, &val);
		}
	);

	if (allowShrink && totalRemoved > 0)
	{
		SFriend::Shrink(*this, _num - totalRemoved);
	}

	return totalRemoved;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSingle(const ElementType& val) noexcept
{
	const SizeType idx = SFriend::FindIndexByFunc(*this,
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqualAs(&el, &val);
		}
	);

	if (idx == KOR_INDEX_NONE) return 0;

	SFriend::RemoveAt(*this, idx);
	return 1;
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveByFunc(FunctorT&& functor, bool allowShrink)
{
	const SizeType totalRemoved = SFriend::RemoveByFunc(*this, Forward<FunctorT>(func));
	if (allowShrink && totalRemoved > 0)
	{
		SFriend::Shrink(*this, _num - totalRemoved);
	}
	return totalRemoved;
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSingleByFunc(FunctorT&& functor)
{
	const SizeType idx = SFriend::FindIndexByFunc(*this, Forward<FunctorT>(func));
	if (idx == KOR_INDEX_NONE) return 0;

	SFriend::RemoveAt(*this, idx);
	return 1;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwap(const ElementType& val, bool allowShrink) noexcept
{
	const SizeType totalRemoved = SFriend::RemoveSwapByFunc(*this,
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqualAs(&el, &val);
		}
	);

	if (allowShrink && totalRemoved > 0)
	{
		SFriend::Shrink(*this, _num - totalRemoved);
	}

	return totalRemoved;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwapSingle(const ElementType& val) noexcept
{
	const SizeType idx = SFriend::FindIndexByFunc(*this,
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqualAs(&el, &val);
		}
	);

	if (idx == KOR_INDEX_NONE) return 0;

	SFriend::RemoveAtSwap(*this, idx);
	return 1;
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwapByFunc(FunctorT&& functor, bool allowShrink)
{
	const SizeType totalRemoved = SFriend::RemoveSwapByFunc(*this, Forward<FunctorT>(func));

	if (allowShrink && totalRemoved > 0)
	{
		SFriend::Shrink(*this, _num - totalRemoved);
	}

	return totalRemoved;
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwapSingleByFunc(FunctorT&& functor)
{
	const SizeType idx = SFriend::FindIndexByFunc(*this, Forward<FunctorT>(func));
	if (idx == KOR_INDEX_NONE) return 0;

	SFriend::RemoveAtSwap(*this, idx);
	return 1;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::RemoveAt(SizeType idx, SizeType num) noexcept
{
	SFriend::RemoveAt(*this, idx, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorT>::RemoveAtSwap(SizeType idx) noexcept
{
	SFriend::RemoveAtSwap(*this, idx, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT TArray<ElementT, AllocatorT>::RemoveAt_GetCopy(SizeType idx) noexcept
{
	ElementT copy = *GetAt(idx);
	SFriend::RemoveAt(*this, idx);
	return copy;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT TArray<ElementT, AllocatorT>::RemoveAtSwap_GetCopy(SizeType idx) noexcept
{
	ElementT copy = *GetAt(idx);
	SFriend::RemoveAtSwap(*this, idx);
	return copy;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT TArray<ElementT, AllocatorT>::Pop() noexcept
{
	ElementT copy = *GetLast();
	SFriend::RemoveFromBack(*this);
	return copy;
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Swap(SizeType firstIdx, SizeType secondIdx) noexcept
{
	KOR_ASSERT(firstIdx < secondIdx);
	KOR_ASSERT(SMath::IsWithin(firstIdx, 0, _num));
	KOR_ASSERT(SMath::IsWithin(secondIdx, 0, _num));

	SMemoryOps::SwapAs(_data + firstIdx, _data + secondIdx);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::SwapRange(SizeType firstIdx, SizeType secondIdx, SizeType num) noexcept
{
	KOR_ASSERT(firstIdx < secondIdx);
	KOR_ASSERT(SMath::IsWithin(firstIdx, 0, _num) && SMath::IsWithin(firstIdx + num, 0, _num));
	KOR_ASSERT(SMath::IsWithin(secondIdx, 0, _num) && SMath::IsWithin(secondIdx + num, 0, _num));

	SMemoryOps::SwapAs(_data + firstIdx, _data + secondIdx, num);
}