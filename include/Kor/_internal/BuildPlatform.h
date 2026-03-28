// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/_internal/BuildPlatformInternals.h"

#if PLATFORM_WINDOWS
	#define PLATFORM_NAME Windows
	#define PLATFORM_FOLDER_NAME Win32
	#include "ASTD/Win32/WindowsBuild.h"
#elif PLATFORM_LINUX
	#define PLATFORM_NAME Linux
	#include "ASTD/Linux/LinuxBuild.h"
#elif PLATFORM_APPLE
	#define PLATFORM_NAME Apple
	#include "ASTD/Apple/AppleBuild.h"
#else
	#error "Unsupported platform"
#endif

#ifndef PLATFORM_FOLDER_NAME
	#define PLATFORM_FOLDER_NAME PLATFORM_NAME
#endif

#define PLATFORM_HEADER_NAME(name) PLATFORM_HEADER_NAME_IMPL(PLATFORM_FOLDER_NAME, PLATFORM_NAME, name).h
#define PLATFORM_HEADER(name) PLATFORM_HEADER_IMPL(PLATFORM_HEADER_NAME(name))

#define PLATFORM_TYPE(name) CONCAT_EXPAND(PLATFORM_NAME, name)
#define PLATFORM_PREFIXED_TYPE(prefix, name) DOUBLE_CONCAT(prefix, PLATFORM_NAME, name)