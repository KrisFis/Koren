// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

// Main PCH
// * Should include all features and functionality of Kor
// -------------------------------------------------------------------------

// Look at "Build.h" to see defines that affect features provided

// Minimal
#include "Kor/KorMinimal.h"

// Archive
#include "Kor/Archive/Archive.h"
#include "Kor/Archive/ArrayArchive.h"
#include "Kor/Archive/FileArchive.h"
#include "Kor/Archive/StdoutArchive.h"

// Container
#include "Kor/Container/Array.h"
#include "Kor/Container/ContainerTraits.h"
#include "Kor/Container/Queue.h"

// IO
#include "Kor/IO/IOOps.h"

// Math
#include "Kor/Math/MathOps.h"

// Memory
#include "Kor/Memory/Allocator.h"
#include "Kor/Memory/AllocatorTraits.h"
#include "Kor/Memory/FixedAllocator.h"
#include "Kor/Memory/MemoryOps.h"
#include "Kor/Memory/QueueAllocator.h"
#include "Kor/Memory/Shared.h"
#include "Kor/Memory/TypedAllocator.h"

// String
#include "Kor/String/CharOps.h"
#include "Kor/String/String.h"
#include "Kor/String/StringOps.h"

// Type Traits
#include "Kor/TypeTrait/CallTraits.h"
#include "Kor/TypeTrait/Category.h"
#include "Kor/TypeTrait/Composite.h"
#include "Kor/TypeTrait/Construct.h"
#include "Kor/TypeTrait/Decay.h"
#include "Kor/TypeTrait/Integer.h"
#include "Kor/TypeTrait/MemberPointer.h"
#include "Kor/TypeTrait/Pack.h"
#include "Kor/TypeTrait/Property.h"
#include "Kor/TypeTrait/Qualifier.h"
#include "Kor/TypeTrait/Relationship.h"
#include "Kor/TypeTrait/String.h"
#include "Kor/TypeTrait/Macros/HasFieldCheck.h"
#include "Kor/TypeTrait/Macros/VariadicIterator.h"

// Utility
#include "Kor/Utility/Assert.h"
#include "Kor/Utility/Forward.h"
#include "Kor/Utility/Functional.h"
#include "Kor/Utility/Invoke.h"
#include "Kor/Utility/IsValid.h"
#include "Kor/Utility/NumOf.h"
#include "Kor/Utility/Optional.h"
#include "Kor/Utility/Swap.h"
