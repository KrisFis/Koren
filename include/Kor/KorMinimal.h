// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

// Minimal PCH
// * Should include minimal features
/////////////////////////////////////////

// Look at "Build.h" to see defines that affect features provided

// CORE
#include "Kor/Core/Build.h"
#include "Kor/Core/Compiler.h"
#include "Kor/Core/Platform.h"

// INITIALIZATION
#include "Kor/Internal/InitTags.h"

// TYPE MANIPULATION/GENERATION/SFINAE
#include "Kor/TypeTraits.h"
#include "Kor/TypeTraitsMacros.h"

// FORWARDS
#include "Kor/Internal/TypeFowards.h"

// ASSERT
#include "Kor/Assert.h"

// IS VALID
#include "Kor/IsValid.h"