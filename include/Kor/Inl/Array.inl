// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

#include "Kor/Inl/ArrayConstructors.inl"
#include "Kor/Inl/ArrayOperators.inl"
#include "Kor/Inl/ArrayQuery.inl"
#include "Kor/Inl/ArrayMemory.inl"
#include "Kor/Inl/ArrayMutation.inl"
#include "Kor/Inl/ArraySort.inl"
#include "Kor/Inl/ArrayIterators.inl"

// Container traits
// -------------------------------------------------------------------------

template<typename ElementT, typename AllocatorT>
struct TContainerTraits<TArray<ElementT, AllocatorT> > : TContainerTraits<void>
{
	using ElementType = ElementType;
	using AllocatorType = AllocatorType;

	enum
	{
		IsDynamic = true,
		InlineMemory = TIsSame<AllocatorType, TArrayAllocator<ElementType> >::Value
	};
};