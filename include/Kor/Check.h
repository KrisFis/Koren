// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Build.h"

#include "Kor/_internal/AssertInternals.h"
#include "Kor/IsValid.h"

#define KOR_CHECK(expression) KOR_CHECK_IMPL(expression)
#define KOR_CHECKF(expression) KOR_CHECKF_IMPL(expression)

#define KOR_CHECK_VALID(object) KOR_CHECK_IMPL(::IsValid(object))
#define KOR_CHECKF_VALID(object) KOR_CHECKF_IMPL(::IsValid(object))

#define KOR_CHECK_RET(expression, ...) if(!KOR_CHECK_IMPL(expression)) { return __VA_ARGS__; }
#define KOR_CHECK_VALID_RET(object, ...) if(!KOR_CHECK_IMPL(::IsValid()) { return __VA_ARGS__; }
