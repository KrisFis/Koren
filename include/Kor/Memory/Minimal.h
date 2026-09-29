// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Minimal.h"

// Required std include
#include <new>

KOR_NAMESPACE_BEGIN

// Namespace containing tags for explicit initialization control.
// Types opt in by adding constructor overloads for the relevant tag types.
namespace Init
{
	// Request default-initialization (default construction performed)
	struct SDefault {};
	inline constexpr SDefault Default {};

	// Request zero-initialization (all bytes set to zero)
	struct SZero {};
	inline constexpr SZero Zero {};

	// Request uninitialized construction (no initialization performed)
	struct SNoInit {};
	inline constexpr SNoInit NoInit {};
}

struct CAllocator;
template<uint32 NumLimit> struct TFixedAllocator;

template<int32 Size, uint32 Alignment> struct TBytes;
template<typename T, uint32 Alignment> struct TTypedBytes;

template<typename AllocatorT, typename ElementT> class TTypedAllocator;
template<typename ElementT> class TQueueAllocator;

template<typename T> class TSharedClass;
template<typename T> class TSharedPtr;
template<typename T> class TWeakPtr;

KOR_NAMESPACE_END