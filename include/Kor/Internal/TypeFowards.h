// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

#include <initializer_list>

KOR_NAMESPACE_BEGIN

// SIMPLE TYPES
/////////////////////////////////

struct SArchive;
struct SString;

// TEMPLATED TYPES
/////////////////////////////////

template<typename ElementT>
class TArrayAllocator;

template<typename ElementT, typename AllocatorT = TArrayAllocator<ElementT>>
class TArray;

template<typename ElementT>
class TOptional;

template<typename ElementT>
class TQueueAllocator;

template<typename ElementT, typename AllocatorT = TQueueAllocator<ElementT>>
class TQueue;

template<typename T>
class TSharedClass;

template<typename T>
class TSharedPtr;

template<typename T>
class TWeakPtr;

KOR_NAMESPACE_END