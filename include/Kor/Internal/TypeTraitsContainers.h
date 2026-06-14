// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

KOR_NAMESPACE_BEGIN

// [Container Type Traits]
// * Defines meta about a container type.
// * Specialize this for each container type, setting ElementType to a non-void type.
// * ElementType = void (default) is the sentinel meaning "not a container" — TIsContainer relies on this invariant.

template<typename T>
struct TContainerTypeTraits
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