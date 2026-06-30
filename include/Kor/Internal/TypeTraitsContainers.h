// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

KOR_NAMESPACE_BEGIN

// [Container Traits]
// * Defines meta about a container type.

template<typename T>
struct TContainerTraits
{
	// Internal element type
	using ElementType = void;

	// Internal allocator type
	using AllocatorType = void;

	// Flags
	enum
	{
		IsDynamic = false,
		InlineMemory = false
	};
};

KOR_NAMESPACE_END