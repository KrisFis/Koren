// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE_DEBUG ElementT* TArray<ElementT, AllocatorT>::GetAt(SizeType idx) noexcept
{
	KOR_ASSERT_DEBUG(IsValidIndex(idx));
	return _data + idx;
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE_DEBUG const ElementT* TArray<ElementT, AllocatorT>::GetAt(SizeType idx) const noexcept
{
	KOR_ASSERT_DEBUG(IsValidIndex(idx));
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
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndex(const ElementType& val) const noexcept
{
	return SFriend::FindIndexByFunc(*this, 
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqualAs(&el, &val);
		}
	);
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndexByFunc(FunctorT&& func) const
{
	return SFriend::FindIndexByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndexByKey(const KeyType& key) const noexcept
{
	return SFriend::FindIndexByFunc(*this, 
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqualAs(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorT>::FindByFunc(FunctorT&& func)
{
	return SFriend::FindByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorT>::FindByFunc(FunctorT&& func) const
{
	return SFriend::FindByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorT>::FindByKey(const KeyType& key) noexcept
{
	return SFriend::FindByFunc(*this, 
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqualAs(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorT>::FindByKey(const KeyType& key) const noexcept
{
	return SFriend::FindByFunc(*this, 
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqualAs(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::Contains(const ElementT& val) const noexcept
{
	return !!FindByKey(val);
}

template<typename ElementT, typename AllocatorT>
template<typename FunctorT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::ContainsByFunc(FunctorT&& func) const
{
	return !!FindByFunc(Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorT>::ContainsByKey(const KeyType& key) const noexcept
{
	return !!FindByKey(key);
}