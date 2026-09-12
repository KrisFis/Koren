// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"

#include "Kor/Allocator.h"

KOR_NAMESPACE_BEGIN

template<uint32 NumLimit>
class TFixedAllocator : public CAllocator {};

template<uint32 NumLimit, typename ElementT>
class TTypedAllocator<TFixedAllocator<NumLimit>, ElementT> : public TTypedAllocator<CAllocator, ElementT>
{
	// TODO_IMPLEMENT
};

KOR_NAMESPACE_END