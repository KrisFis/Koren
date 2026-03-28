// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <initializer_list>

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