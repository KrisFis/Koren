// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Minimal.h"

// Required std include
#include <initializer_list>

KOR_NAMESPACE_BEGIN

// Forwards
// -------------------------------------------------------------------------

class CAllocator;
struct SArchive;

template<typename T> class TString;
template<typename ElementT, typename AllocatorT = class CAllocator> class TArray;
template<typename ElementT> class TOptional;
template<typename ElementT> class TQueueAllocator;
template<typename ElementT, typename AllocatorT = TQueueAllocator<ElementT>> class TQueue;

KOR_NAMESPACE_END