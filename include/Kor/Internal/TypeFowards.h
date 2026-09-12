// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

// Required std include
#include <initializer_list>

KOR_NAMESPACE_BEGIN

// SIMPLE TYPES
/////////////////////////////////

class CAllocator;
struct SArchive;

// TEMPLATED TYPES
/////////////////////////////////

template<typename T>
class TString;

template<typename ElementT, typename AllocatorT = CAllocator>
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