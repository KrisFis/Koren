// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE_DEBUG ElementT* TArray<ElementT, AllocatorT>::GetAt(SizeType idx) noexcept
{
	KOR_ASSERT_DEBUG(SMath::IsWithin(idx, 0, arr._num));
	return _data + idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE_DEBUG const ElementT* TArray<ElementT, AllocatorT>::GetAt(SizeType idx) const noexcept
{
	KOR_ASSERT_DEBUG(SMath::IsWithin(idx, 0, arr._num));
	return _data + idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorT>::GetFirst() noexcept
{
	return _num > 0 ? _data : nullptr;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorT>::GetFirst() const noexcept
{
	return _num > 0 ? _data : nullptr;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorT>::GetLast() noexcept
{
	return _num > 0 ? _data + (_num - 1) : nullptr;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorT>::GetLast() const noexcept
{
	return _num > 0 ? _data + (_num - 1) : nullptr;
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndex(const ElementType& val) const noexcept
{
	const ElementType* end = _data + _num;
	for (const ElementType* curr = _data; curr != end; ++curr)
	{
		if (SMemoryOps::IsEqualAs(curr, &val))
		{
			return KOR_PTR_TYPED_DIFF(SizeType, curr, _data);
		}
	}

	return KOR_INDEX_NONE;
}

template<typename ElementT, typename AllocatorT>
template<typename Functor>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndexByFunc(Functor&& func) const
{
	const ElementType* end = _data + _num;
	for (const ElementType* curr = _data; curr != end; ++curr)
	{
		// TODO: Implement Invoke (std::invoke)
		if (func(*curr))
		{
			return KOR_PTR_TYPED_DIFF(SizeType, curr, _data);
		}
	}

	return KOR_INDEX_NONE;
}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndexByKey(const KeyType& key) const noexcept
{
	const ElementType* end = _data + _num;
	for (const ElementType* curr = _data; curr != end; ++curr)
	{
		if (*curr == key)
		{
			return KOR_PTR_TYPED_DIFF(SizeType, curr, _data);
		}
	}

	return KOR_INDEX_NONE;
}

template<typename ElementT, typename AllocatorT>
template<typename Functor>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorT>::FindByFunc(Functor&& func)
{
	const ElementType* end = _data + _num;
	for (const ElementType* curr = _data; curr != end; ++curr)
	{
		// TODO: Implement Invoke (std::invoke)
		if (func(*curr))
		{
			return curr;
		}
	}

	return nullptr;
}

template<typename ElementT, typename AllocatorT>
template<typename Functor>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorT>::FindByFunc(Functor&& func) const
{
	return const_cast<TArray*>(this)->FindByFunc(Forward<Functor>(func));
}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
KOR_INLINE ElementT* TArray<ElementT, AllocatorT>::FindByKey(const KeyType& key) noexcept
{
	const ElementType* end = _data + _num;
	for (const ElementType* curr = _data; curr != end; ++curr)
	{
		if (*curr == key)
		{
			return curr;
		}
	}

	return nullptr;
}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorT>::FindByKey(const KeyType& key) const noexcept
{
	return const_cast<TArray*>(this)->FindByKey(key);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::Contains(const ElementT& val) const noexcept
{
	return !!FindByKey(val);
}

template<typename ElementT, typename AllocatorT>
template<typename Functor>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::ContainsByFunc(Functor&& func) const
{
	return !!FindByFunc(Forward<Functor>(func));
}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::ContainsByKey(const KeyType& key) const noexcept
{
	return !!FindByKey(key);
}