// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorT>
const typename ElementT* TArray<ElementT, AllocatorT>::GetAt(SizeType idx) const noexcept
{
}

template<typename ElementT, typename AllocatorT>
typename ElementT* TArray<ElementT, AllocatorT>::GetAt(SizeType idx) noexcept
{
}

template<typename ElementT, typename AllocatorT>
const typename ElementT* TArray<ElementT, AllocatorT>::GetFirst() const noexcept
{
}

template<typename ElementT, typename AllocatorT>
typename ElementT* TArray<ElementT, AllocatorT>::GetFirst() noexcept
{
}

template<typename ElementT, typename AllocatorT>
const typename ElementT* TArray<ElementT, AllocatorT>::GetLast() const noexcept
{
}

template<typename ElementT, typename AllocatorT>
typename ElementT* TArray<ElementT, AllocatorT>::GetLast() noexcept
{
}

template<typename ElementT, typename AllocatorT>
typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndex(const ElementType& val) const noexcept
{
}

template<typename ElementT, typename AllocatorT>
template<typename Functor>
typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndexByFunc(Functor&& func) const
{
}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
typename TArray<ElementT, AllocatorT>::SizeType TArray<ElementT, AllocatorT>::FindIndexByKey(KeyType key) const noexcept
{
}

template<typename ElementT, typename AllocatorT>
template<typename Functor>
const ElementT* TArray<ElementT, AllocatorT>::FindByFunc(Functor&& func) const
{

}

template<typename ElementT, typename AllocatorT>
template<typename Functor>
ElementT* TArray<ElementT, AllocatorT>::FindByFunc(Functor&& func)
{

}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
const ElementT* TArray<ElementT, AllocatorT>::FindByKey(KeyType key) const noexcept
{

}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
ElementT* TArray<ElementT, AllocatorT>::FindByKey(KeyType key) noexcept
{

}

template<typename ElementT, typename AllocatorT>
bool TArray<ElementT, AllocatorT>::Contains(const ElementT& val) const noexcept
{

}

template<typename ElementT, typename AllocatorT>
template<typename Functor>
bool TArray<ElementT, AllocatorT>::ContainsByFunc(Functor&& func) const
{

}

template<typename ElementT, typename AllocatorT>
template<typename KeyType>
bool TArray<ElementT, AllocatorT>::ContainsByKey(KeyType key) const noexcept
{

}