// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

// Main PCH
// * Should include all features and functionality of Kor
/////////////////////////////////////////

// Look at "Configurations.h" to see defines that affects features provided

// CORE
#include "Kor/KorMinimal.h"

// UTILITIES
#include "Kor/Math.h"
#include "Kor/Memory.h"
#include "Kor/Misc.h"

// CONTAINERS
#include "Kor/Array.h"
#include "Kor/Optional.h"
#include "Kor/Queue.h"

// ALLOCATORS
#include "Kor/ArrayAllocator.h"
#include "Kor/FixedArrayAllocator.h"
#include "Kor/QueueAllocator.h"

// STRINGS
#include "Kor/CharOps.h"
#include "Kor/StringOps.h"
#include "Kor/CString.h"
#include "Kor/String.h"

// SHARED
#include "Kor/Shared.h"

// Assert
#include "Kor/Assert.h"
#include "Kor/IsValid.h"

// EXTRAS -> ARCHIVE
#include "Kor/Archive.h"
#include "Kor/ArrayArchive.h"
#include "Kor/FileArchive.h"
#include "Kor/StdoutArchive.h"
