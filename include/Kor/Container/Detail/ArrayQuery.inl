// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE_DEBUG ElementT* TArray<ElementT, AllocatorFamilyT>::GetAt(SizeType idx) noexcept
{
	return IsValidIndex(idx) ? _data + idx : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE_DEBUG const ElementT* TArray<ElementT, AllocatorFamilyT>::GetAt(SizeType idx) const noexcept
{
	return IsValidIndex(idx) ? _data + idx : nullptr;
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
template<typename KeyType>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::FindIndex(const KeyType& key) const noexcept
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
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::FindIndexByFunc(FunctorT&& func) const
{
	return SFriend::FindIndexByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::Find(const KeyType& key) noexcept
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
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::Find(const KeyType& key) const noexcept
{
	return SFriend::FindByFunc(*this,
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
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::Contains(const KeyType& key) const noexcept
{
	return !!Find(key);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::ContainsByFunc(FunctorT&& func) const
{
	return !!FindByFunc(Forward<FunctorT>(func));
}