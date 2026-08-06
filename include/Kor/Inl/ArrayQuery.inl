// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename ElementT* TArray<ElementT, AllocatorT>::GetAt(SizeType idx) noexcept
{
	return operator[](idx);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE const typename ElementT* TArray<ElementT, AllocatorT>::GetAt(SizeType idx) const noexcept
{
	return operator[](idx);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename ElementT* TArray<ElementT, AllocatorT>::GetFirst() noexcept
{
	return operator[](0);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE const typename ElementT* TArray<ElementT, AllocatorT>::GetFirst() const noexcept
{
	return operator[](0);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE typename ElementT* TArray<ElementT, AllocatorT>::GetLast() noexcept
{
	return operator[](_num - 1);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE const typename ElementT* TArray<ElementT, AllocatorT>::GetLast() const noexcept
{
	return operator[](_num - 1);
}

template<typename ElementT, typename AllocatorT>
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndex(const ElementType& val) const noexcept
{
	const ElementType* end = _data + _num;
	for (const ElementType* curr = _data; curr != end; ++curr)
	{
		if (SMemoryOps::IsEqual(curr, &val))
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
KOR_INLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndexByKey(KeyType key) const noexcept
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
KOR_INLINE ElementT* TArray<ElementT, AllocatorT>::FindByKey(KeyType key) noexcept
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
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorT>::FindByKey(KeyType key) const noexcept
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
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::ContainsByKey(KeyType key) const noexcept
{
	return !!FindByKey(key);
}