// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Minimal.h"

#include "Kor/Memory/Minimal.h"

// Required std include
#include <initializer_list>

KOR_NAMESPACE_BEGIN

template<typename ElementT, typename AllocatorT = CAllocator> class TArray;
template<typename ElementT, typename AllocatorT = TQueueAllocator<ElementT>> class TQueue;

KOR_NAMESPACE_END