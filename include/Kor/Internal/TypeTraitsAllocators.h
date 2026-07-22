// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"
#include "Kor/Internal/TypeTraitsType.h"

KOR_NAMESPACE_BEGIN

// See Allocator.h for model concept

// [Allocator Traits Base]
// * Shared defaults for allocator trait specializations.
// * Specializations of TAllocatorTraits should inherit from this and override as needed.

template<typename T>
struct TAllocatorTraitsBase
{
	using SizeType = void;

	enum
	{
		// Supports and needs alignment as part of its method signatures
		// * Alignment parameter should come immediately after main signature
		NeedsAlignment = false,

		// Supports reallocation of previously allocated memory
		HasReallocate = false,
	}
};

// [Allocator Traits]
// * Defines meta about an allocator type.
// * Is intentionally left as forward declare
//
// Example Declaration:
//
// template<>
// struct TAllocatorTraits<MyAllocator> : TAllocatorTraitsBase<MyAllocator>
// {
//    using SizeType = MyAllocator::SizeType;
// }
template<typename T>
struct TAllocatorTraits;

// [Is Allocator]
// * Checks whether specific type is an allocator (defines TAllocatorTraits)
template<typename T>
struct TIsAllocator : TBoolValue<TIsComplete<TAllocatorTraits<T>>::Value> {};

KOR_NAMESPACE_END