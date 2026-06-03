// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Platform.h"

KOR_NAMESPACE_BEGIN

// Namespace containing tags for explicit initialization control.
// Types opt in by adding constructor overloads for the relevant tag types.
namespace Init
{
	// Request zero-initialization (all bytes set to zero)
	struct SZero {};
	inline constexpr SZero Zero {};

	// Request uninitialized construction (no initialization performed)
	struct SNoInit {};
	inline constexpr SNoInit NoInit {};
}

KOR_NAMESPACE_END