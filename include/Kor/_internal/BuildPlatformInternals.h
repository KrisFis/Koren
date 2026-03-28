// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#define KOR_PLATFORM_HEADER_NAME_IMPL(x,y,z) x/KOR_MACRO_CONCAT(y, z)
#define KOR_PLATFORM_HEADER_IMPL(x) KOR_MACRO_STRINGIFY(x)