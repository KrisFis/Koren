// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

KOR_NAMESPACE_BEGIN

// [Allocator Type Traits]
// * Defines meta about container type

template<typename T>
struct TAllocatorTypeTraits
{
	// Internal element type
	using ElementType = void;

	// Flags
	enum
	{
		IsDynamic = false,
		InlineMemory = false
	};
};

KOR_NAMESPACE_END