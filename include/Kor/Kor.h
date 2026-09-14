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
#include "Kor/TypeTrait/CallTraits.h"
#include "Kor/TypeTrait/Category.h"
#include "Kor/TypeTrait/Composite.h"
#include "Kor/TypeTrait/Construct.h"
#include "Kor/TypeTrait/Decay.h"
#include "Kor/TypeTrait/Integer.h"
#include "Kor/TypeTrait/MemberPointer.h"
#include "Kor/TypeTrait/Minimal.h"
#include "Kor/TypeTrait/Pack.h"
#include "Kor/TypeTrait/Property.h"
#include "Kor/TypeTrait/Qualifier.h"
#include "Kor/TypeTrait/Relationship.h"
#include "Kor/TypeTrait/String.h"
#include "Kor/TypeTrait/Macros/HasFieldCheck.h"
#include "Kor/TypeTrait/Macros/VariadicIterator.h"

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