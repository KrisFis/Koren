// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Kor/_internal/BuildPlatformInternals.h"

#if KOR_PLATFORM_WINDOWS
	#define KOR_PLATFORM_NAME Windows
	#define KOR_PLATFORM_FOLDER_NAME Win32
	#include "Kor/Win32/WindowsBuild.h"
#elif KOR_PLATFORM_LINUX
	#define KOR_PLATFORM_NAME Linux
	#include "Kor/Linux/LinuxBuild.h"
#elif KOR_PLATFORM_APPLE
	#define KOR_PLATFORM_NAME Apple
	#include "Kor/Apple/AppleBuild.h"
#else
	#error "Unsupported platform"
#endif

#ifndef KOR_PLATFORM_FOLDER_NAME
	#define KOR_PLATFORM_FOLDER_NAME KOR_PLATFORM_NAME
#endif

#define KOR_PLATFORM_HEADER_NAME(name) KOR_PLATFORM_HEADER_NAME_IMPL(KOR_PLATFORM_FOLDER_NAME, KOR_PLATFORM_NAME, name).h
#define KOR_PLATFORM_HEADER(name) KOR_PLATFORM_HEADER_IMPL(KOR_PLATFORM_HEADER_NAME(name))

#define KOR_PLATFORM_TYPE(name) KOR_MACRO_CONCAT_EXPAND(KOR_PLATFORM_NAME, name)
#define KOR_PLATFORM_PREFIXED_TYPE(prefix, name) KOR_MACRO_DOUBLE_CONCAT(prefix, KOR_PLATFORM_NAME, name)