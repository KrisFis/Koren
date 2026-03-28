// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Build.h"

#include <io.h>

KOR_NAMESPACE_BEGIN

struct SWindowsPlatformMisc
{
	static double GetSecondsSinceEpoch()
	{
		int64 wintime;
		GetSystemTimeAsFileTime((FILETIME*)&wintime);
		wintime -=116444736000000000i64;  //1jan1601 to 1jan1970
		return (double)(wintime / 10000000i64) + (double)(wintime % 10000000i64 * 100) / 1.e9;
	}

	// Reads from standard file by fileno. example: STDIN_FILENO
	static int64 ReadFromFile(int32 fileNo, void* buffer, uint64 size) { return _read(fileNo, buffer, size); }

	// Writes to standard file by fileno. example: STDOUT_FILENO
	static int64 WriteToFile(int32 fileNo, const void* buffer, uint64 size) { return _write(fileNo, buffer, size); }
};

KOR_NAMESPACE_END