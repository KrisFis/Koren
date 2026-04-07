// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/GNUC/GNUCCompiler.h"

// BUILD
////////////////////////////////////////////////////////////////////////

#if defined(__i386__) || defined(__x86_64__)
	#define KOR_DEBUG_BREAK() __asm__ volatile("int3")
#else
	#define KOR_DEBUG_BREAK() __builtin_trap()
#endif