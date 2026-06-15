// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

// Constructors, Destructor
//#include "Kor/Inl/ArrayConstructors.inl

// Assign, Compare, Dereference, Index operators
//#include "Kor/Inl/ArrayOperators.inl

// Property Getters, Validation, Get, Find Index, Find Element, Contains
//#include "Kor/Inl/ArrayQuery.inl

// Reserve / Resize / Reset, Fill / Assign
//#include "Kor/Inl/ArrayMemory.inl

// Add, Emplace, Insert, Append, Remove, Replace, Swap
//#include "Kor/Inl/ArrayMutation.inl

// Sort, Reverse
//#include "Kor/Inl/ArraySort.inl

// Iterators
//#include "Kor/Inl/ArrayIterators.inl

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