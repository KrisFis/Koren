// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Minimal.h"
#include "Kor/Memory/Minimal.h"

// Required std include
#include <initializer_list>

KOR_NAMESPACE_BEGIN

struct SArchive;

template<typename ElementT, typename AllocatorT = CAllocator> struct TArrayArchive;
template<int32 FileNo, typename AllocatorT = CAllocator> struct TStdoutArchive;

KOR_NAMESPACE_END