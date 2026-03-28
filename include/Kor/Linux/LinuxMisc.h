// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <time.h>
#include <unistd.h>

#include "ASTD/Linux/LinuxBuild.h"

struct SLinuxPlatformMisc
{
	static double GetSecondsSinceEpoch()
	{
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (double)ts.tv_sec + (double)ts.tv_nsec / 1.e9;
	}

	// Reads from standard file by fileno. example: STDIN_FILENO
	FORCEINLINE static int64 ReadStd(int32 fileNo, void* buffer, uint64 size) { return read(fileNo, buffer, size); }

	// Writes to standard file by fileno. example: STDOUT_FILENO
	FORCEINLINE static int64 WriteStd(int32 fileNo, const void* buffer, uint64 size) { return write(fileNo, buffer, size); }
};
