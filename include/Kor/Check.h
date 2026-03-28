// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"

#include "ASTD/_internal/ChecksInternals.h"
#include "ASTD/IsValid.h"

#define CHECK(expression) CHECK_IMPL(expression)
#define CHECKF(expression) CHECKF_IMPL(expression)

#define CHECK_VALID(object) CHECK_IMPL(::IsValid(object))
#define CHECKF_VALID(object) CHECKF_IMPL(::IsValid(object))

#define CHECK_RET(expression, ...) if(!CHECK_IMPL(expression)) { return __VA_ARGS__; }
#define CHECK_VALID_RET(object, ...) if(!CHECK_IMPL(::IsValid()) { return __VA_ARGS__; }
