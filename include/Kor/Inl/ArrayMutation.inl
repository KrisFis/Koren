// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Add(const ElementType& val) noexcept
{
	const SizeType idx = SFriend::Add(*this);
	SMemoryOps::CopyConstruct(_data + idx, &val);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Add(ElementType&& val) noexcept
{
	const SizeType idx = SFriend::Add(*this);
	SMemoryOps::MoveConstruct(_data + idx, &val);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::Add_GetRef(const ElementType& val) noexcept
{
	const SizeType idx = Add(val);
	return _data[idx];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::Add_GetRef(ElementType&& val) noexcept
{
	const SizeType idx = Add(Move(val));
	return _data[idx];
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
	const int32 idx = AddUnique(val);
	return _data[idx];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::AddUnique_GetRef(ElementType&& val) noexcept
{
	const int32 idx = AddUnique(Move(val));
	return _data[idx];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddDefaulted(SizeType num) noexcept
{
	const SizeType idx = SFriend::Add(*this, num);
	SMemoryOps::DefaultConstruct(_data + idx, num);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::AddDefaulted_GetRef() noexcept
{
	const int32 idx = AddDefaulted(num);
	return _data[idx];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddZeroed(SizeType num) noexcept
{
	const SizeType idx = SFriend::Add(*this, num);
	SMemoryOps::ZeroConstruct(_data + idx, num);
	return idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::AddZeroed_GetRef() noexcept
{
	const int32 idx = AddZeroed(num);
	return _data[idx];
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddUninitialized(SizeType num) noexcept
{
	return SFriend::Add(*this, num);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::AddUninitialized_GetRef() noexcept
{
	const SizeType idx = SFriend::Add(*this);
	return _data[idx];
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
	const SizeType idx = SFriend::Add(*this);
	SMemoryOps::Construct(_data + idx, Forward<ArgTypes>(args)...);
	return idx;
}

template<typename ElementT, typename AllocatorT>
template<typename ... ArgTypes>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorT>::Emplace_GetRef(ArgTypes&&... args) noexcept
{
	const SizeType idx = Emplace(Forward<ArgTypes>(args)...);
	return _data[idx];
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
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(const TArray& other) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(TArray&& other) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(const ElementType& val, SizeType num) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(const ILType& list) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(const ElementType* data, SizeType num) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::AppendUninitialized(SizeType numToAdd) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Remove(const ElementType& val, bool allowShrink) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwap(const ElementType& val, bool allowShrink) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
template<typename Predicate>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveByPredicate(Predicate&& predicate, bool allowShrink)
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
template<typename Predicate>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwapByPredicate(Predicate&& predicate, bool allowShrink)
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveFirst(const ElementType& val) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwapFirst(const ElementType& val) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::RemoveAt(SizeType idx) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::RemoveAt(SizeType idx, SizeType count) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::RemoveAtSwap(SizeType idx) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT TArray<ElementT, AllocatorT>::RemoveAt_GetCopy(SizeType idx) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT TArray<ElementT, AllocatorT>::RemoveAtSwap_GetCopy(SizeType idx) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT TArray<ElementT, AllocatorT>::Pop() noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Replace(const TArray& other) noexcept
{
	TODO_IMPLEMENT()
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Replace(TArray&& other) noexcept
{
	TODO_IMPLEMENT()
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