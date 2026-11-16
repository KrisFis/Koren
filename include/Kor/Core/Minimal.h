// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"
#include "Kor/Core/Platform.h"
#include "Kor/Core/Compiler.h"

// Class declaration macros
// -------------------------------------------------------------------------

#define KOR_DISABLE_COPY(ClassName) \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete; \

#define KOR_DISABLE_MOVE(ClassName) \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \

#define KOR_DISABLE_COPY_AND_MOVE(ClassName) \
	KOR_DISABLE_COPY(ClassName) \
	KOR_DISABLE_MOVE(ClassName)