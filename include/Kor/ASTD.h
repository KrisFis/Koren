// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Main PCH
// * Should include all features and functionality of ASTD
/////////////////////////////////////////

// Look at "Configurations.h" to see defines that affects features provided

// CORE
#include "ASTD/ASTDMinimal.h"

// UTILITIES
#include "ASTD/Math.h"
#include "ASTD/Memory.h"
#include "ASTD/Misc.h"

// CONTAINERS
#include "ASTD/Array.h"
#include "ASTD/Optional.h"
#include "ASTD/Queue.h"

// ALLOCATORS
#include "ASTD/ArrayAllocator.h"
#include "ASTD/FixedArrayAllocator.h"
#include "ASTD/QueueAllocator.h"

// STRINGS
#include "ASTD/CString.h"
#include "ASTD/String.h"

// SHARED
#include "ASTD/Shared.h"

// EXTRAS -> ARCHIVE
#include "ASTD/Archive.h"
#include "ASTD/ArrayArchive.h"
#include "ASTD/FileArchive.h"
#include "ASTD/StdoutArchive.h"
