// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE AllocatorT& TTypedAllocatorBase<AllocatorT, ElementT>::Get() noexcept
{
	return _allocator;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE const AllocatorT& TTypedAllocatorBase<AllocatorT, ElementT>::Get() const noexcept
{
	return _allocator;
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocatorBase<AllocatorT, ElementT>::Allocate(SizeType num, uint32 alignment) noexcept
{
	using Traits = TAllocatorTraits<AllocatorT>;

	if constexpr (Traits::NeedsAlignment)
	{
		return (ElementType*)AllocatorType::Allocate(num * sizeof(ElementType), alignment);
	}
	else
	{
		return (ElementType*)AllocatorType::Allocate(num * sizeof(ElementType));
	}
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE ElementT* TTypedAllocatorBase<AllocatorT, ElementT>::Reallocate(ElementType* ptr, SizeType num, uint32 alignment) noexcept
{
	using Traits = TAllocatorTraits<AllocatorT>;

	static_assert(
		Traits::HasReallocate,
		"AllocatorType has no native Reallocate. Caller must Allocate + Copy + Deallocate manually, since only the container knows the old element count");

	if constexpr (Traits::NeedsAlignment)
	{
		return (ElementType*)AllocatorType::Reallocate(ptr, num * sizeof(ElementType), alignment);
	}
	else
	{
		return (ElementType*)AllocatorType::Reallocate(ptr, num * sizeof(ElementType));
	}
}

template<typename AllocatorT, typename ElementT>
KOR_FORCEINLINE void TTypedAllocatorBase<AllocatorT, ElementT>::Deallocate(ElementType* ptr, uint32 alignment) noexcept
{
	using Traits = TAllocatorTraits<AllocatorT>;

	if constexpr (Traits::NeedsAlignment)
	{
		AllocatorType::Deallocate(ptr, alignment);
	}
	else
	{
		AllocatorType::Deallocate(ptr);
	}
}