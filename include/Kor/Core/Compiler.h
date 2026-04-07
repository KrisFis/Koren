// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

// Compiler Primitives
///////////////////////////////////////////////////////////////
// Helpers for resolving compiler-specific headers and type names.
// KOR_COMPILER_NAME expands to the active platform identifier (e.g. Clang).
//
// Header resolution:
//   KOR_COMPILER_HEADER(name)            - relative to current include root
//   KOR_COMPILER_HEADER_FROM(root, name) - relative to explicit root
///////////////////////////////////////////////////////////////

#if KOR_COMPILER_MSVC
	#define KOR_COMPILER_NAME MSVC
#elif KOR_COMPILER_CLANG
	#define KOR_COMPILER_NAME Clang
#elif KOR_COMPILER_GCC
	#define KOR_COMPILER_NAME GCC
#else
	#error "Unsupported compiler"
#endif

#define KOR_COMPILER_HEADER(name) KOR_MACRO_STRINGIFY_EXPAND(KOR_COMPILER_NAME/KOR_MACRO_CONCAT_EXPAND(KOR_COMPILER_NAME, name.h))
#define KOR_COMPILER_HEADER_FROM(root, name) KOR_MACRO_STRINGIFY_EXPAND(root/KOR_COMPILER_NAME/KOR_MACRO_CONCAT_EXPAND(KOR_COMPILER_NAME, name.h))

// Main compiler include
///////////////////////////////////////////////////////////////

#include KOR_COMPILER_HEADER_FROM(Kor/Core, Compiler)