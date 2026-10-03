// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE_DEBUG ElementT* TArray<ElementT, AllocatorFamilyT>::GetAt(SizeType idx) noexcept
{
	KOR_ASSERT_DEBUG(IsValidIndex(idx));
	return _data + idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE_DEBUG const ElementT* TArray<ElementT, AllocatorFamilyT>::GetAt(SizeType idx) const noexcept
{
	KOR_ASSERT_DEBUG(IsValidIndex(idx));
	return _data + idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::GetFirst() noexcept
{
	return _num > 0 ? _data : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::GetFirst() const noexcept
{
	return _num > 0 ? _data : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::GetLast() noexcept
{
	return _num > 0 ? _data + (_num - 1) : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::GetLast() const noexcept
{
	return _num > 0 ? _data + (_num - 1) : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::FindIndex(const ElementType& val) const noexcept
{
	return SFriend::FindIndexByFunc(*this, 
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &val);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::FindIndexByFunc(FunctorT&& func) const
{
	return SFriend::FindIndexByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::FindIndexByKey(const KeyType& key) const noexcept
{
	return SFriend::FindIndexByFunc(*this, 
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::FindByFunc(FunctorT&& func)
{
	return SFriend::FindByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::FindByFunc(FunctorT&& func) const
{
	return SFriend::FindByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::FindByKey(const KeyType& key) noexcept
{
	return SFriend::FindByFunc(*this, 
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::FindByKey(const KeyType& key) const noexcept
{
	return SFriend::FindByFunc(*this, 
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::Contains(const ElementT& val) const noexcept
{
	return !!FindByKey(val);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::ContainsByFunc(FunctorT&& func) const
{
	return !!FindByFunc(Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::ContainsByKey(const KeyType& key) const noexcept
{
	return !!FindByKey(key);
}