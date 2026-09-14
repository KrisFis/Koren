// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

// Main PCH
// * Should include all features and functionality of Kor
/////////////////////////////////////////

// Look at "Build.h" to see defines that affect features provided

// Minimal
#include "Kor/KorMinimal.h"

// Type Traits
#include "Kor/TypeTraits/CallTraits.h"
#include "Kor/TypeTraits/Category.h"
#include "Kor/TypeTraits/Composite.h"
#include "Kor/TypeTraits/Construct.h"
#include "Kor/TypeTraits/Decay.h"
#include "Kor/TypeTraits/Integer.h"
#include "Kor/TypeTraits/MemberPointer.h"
#include "Kor/TypeTraits/Minimal.h"
#include "Kor/TypeTraits/Pack.h"
#include "Kor/TypeTraits/Property.h"
#include "Kor/TypeTraits/Qualifier.h"
#include "Kor/TypeTraits/Relationship.h"
#include "Kor/TypeTraits/String.h"
#include "Kor/TypeTraits/Macros/HasFieldCheck.h"
#include "Kor/TypeTraits/Macros/VariadicIterator.h"

// Memory
#include "Kor/Memory/AllocatorTraits.h"
#include "Kor/Memory/ContainerTraits.h"

// Utility
#include "Kor/Utility/Forward.h"
#include "Kor/Utility/Invoke.h"
#include "Kor/Utility/IsValid.h"

// Ops
#include "Kor/Math.h"
#include "Kor/MemoryOps.h"
#include "Kor/Misc.h"

// CONTAINERS
#include "Kor/Array.h"
#include "Kor/Optional.h"
#include "Kor/Queue.h"

// ALLOCATORS
#include "Kor/Allocator.h"
#include "Kor/FixedAllocator.h"
#include "Kor/QueueAllocator.h"

// STRINGS
#include "Kor/CharOps.h"
#include "Kor/String.h"
#include "Kor/StringOps.h"

// SHARED
#include "Kor/Shared.h"

// EXTRAS -> ARCHIVE
#include "Kor/Archive.h"
#include "Kor/ArrayArchive.h"
#include "Kor/FileArchive.h"
#include "Kor/StdoutArchive.h"