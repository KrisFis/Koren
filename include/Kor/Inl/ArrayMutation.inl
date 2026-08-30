// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
template<typename ValueType>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Add(ValueType&& val) noexcept
{

}

template<typename ElementT, typename AllocatorT>
template<typename ValueType>
KOR_INLINE ElementT& TArray<ElementT, AllocatorT>::Add_GetRef(ValueType&& val) noexcept
{

}

template<typename ElementT, typename AllocatorT>
template<typename ValueType>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddUnique(ValueType&& val) noexcept
{
	
}

template<typename ElementT, typename AllocatorT>
template<typename ValueType>
KOR_INLINE ElementT& TArray<ElementT, AllocatorT>::AddUnique_GetRef(ValueType&& val) noexcept
{

}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddDefaulted(SizeType num) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT& TArray<ElementT, AllocatorT>::AddDefaulted_GetRef() noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddZeroed(SizeType num) noexcept
{

}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT& TArray<ElementT, AllocatorT>::AddZeroed_GetRef() noexcept
{

}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddUninitialized(SizeType num) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT& TArray<ElementT, AllocatorT>::AddUninitialized_GetRef() noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::AddUnique(const ElementType& val) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Push(const ElementType& val) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Push(ElementType&& val) noexcept
{
}

template<typename ElementT, typename AllocatorT>
template<typename ... ArgTypes>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Emplace(ArgTypes&&... args) noexcept
{
}

template<typename ElementT, typename AllocatorT>
template<typename ... ArgTypes>
KOR_INLINE ElementT& TArray<ElementT, AllocatorT>::Emplace_GetRef(ArgTypes&&... args) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Insert(SizeType idx, const ElementType& val) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Insert(SizeType idx, ElementType&& val) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Insert(SizeType idx, const ElementType* data, SizeType num) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Insert(SizeType idx, const ILType& list) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(const TArray& other) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(TArray&& other) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(const ElementType& val, SizeType num) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(const ILType& list) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Append(const ElementType* data, SizeType num) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::AppendUninitialized(SizeType numToAdd) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::Remove(const ElementType& val, bool allowShrink) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwap(const ElementType& val, bool allowShrink) noexcept
{
}

template<typename ElementT, typename AllocatorT>
template<typename Predicate>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveByPredicate(Predicate&& predicate, bool allowShrink)
{
}

template<typename ElementT, typename AllocatorT>
template<typename Predicate>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwapByPredicate(Predicate&& predicate, bool allowShrink)
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveFirst(const ElementType& val) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::RemoveSwapFirst(const ElementType& val) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::RemoveAt(SizeType idx) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::RemoveAt(SizeType idx, SizeType count) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::RemoveAtSwap(SizeType idx) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT TArray<ElementT, AllocatorT>::RemoveAt_GetCopy(SizeType idx) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT TArray<ElementT, AllocatorT>::RemoveAtSwap_GetCopy(SizeType idx) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE ElementT TArray<ElementT, AllocatorT>::Pop() noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Replace(const TArray& other) noexcept
{
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE void TArray<ElementT, AllocatorT>::Replace(TArray&& other) noexcept
{
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