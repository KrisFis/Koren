// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Build.h"

#include <ctime>
#include <unistd.h>

KOR_NAMESPACE_BEGIN

struct SAppleMisc
{
	static double GetSecondsSinceEpoch()
	{
		timespec ts = {};
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (double)ts.tv_sec + (double)ts.tv_nsec / 1.e9;
	}

	// Reads from standard file by fileno. example: STDIN_FILENO
	KOR_FORCEINLINE static int64 ReadFromFile(int32 fileNo, void* buffer, uint64 size) { return read(fileNo, buffer, size); }

	// Writes to standard file by fileno. example: STDOUT_FILENO
	KOR_FORCEINLINE static int64 WriteToFile(int32 fileNo, const void* buffer, uint64 size) { return write(fileNo, buffer, size); }
};

KOR_NAMESPACE_END