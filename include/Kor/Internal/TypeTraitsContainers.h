// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"
#include "Kor/Internal/TypeTraitsAllocators.h"

KOR_NAMESPACE_BEGIN

// [Container Traits Base]
// * Shared defaults for container trait specializations.
// * Specializations of TContainerTraits should inherit from this and override as needed.

template<typename T>
struct TContainerTraitsBase
{
	using ElementType = void;
	using AllocatorType = void;
	using SizeType = void;
};

// [Container Traits]
// * Defines meta about a container type.
// * Is intentionally left as forward declare
//
// Example Declaration:
//
// template<>
// struct TContainerTraits<MyContainer> : TContainerTraitsBase<MyContainer>
// {
//    using AllocatorType = typename MyContainer::AllocatorType;
//    enum { IsDynamic = true }
// }
template<typename T>
struct TContainerTraits;

// [Is Container]
// * Checks whether specific type is a container (defines TContainerTraits)
template<typename T>
struct TIsContainer : TBoolValue<TIsComplete<TContainerTraits<T>>::Value> {};

KOR_NAMESPACE_END