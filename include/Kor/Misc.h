// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/StringOps.h"

#include KOR_PLATFORM_HEADER_FROM(Kor/Core, Misc)

KOR_NAMESPACE_BEGIN

typedef KOR_PLATFORM_STRUCT(Misc) SPlatformMisc;
// TODO: Separate to IO (input/output) and STime
struct SMisc : public SPlatformMisc
{
	static constexpr long double NS_PER_SECOND = 1.e9; // nanoseconds
	static constexpr long double MICROS_PER_SECOND = 1.e6; // microseconds
	static constexpr long double MS_PER_SECOND = 1.e3; // miliseconds
	static constexpr long double HOURS_PER_SECOND = 2.7e-4; // hours
	static constexpr long double DAYS_PER_SECOND = 1.1574074e-4; // days

	static constexpr int32 STDIN_FILE_NO = 0;
	static constexpr int32 STDOUT_FILE_NO = 1;
	static constexpr int32 STDERR_FILE_NO = 2;

	KOR_FORCEINLINE_DEBUGGABLE static int64 ReadFromStdin(void* buffer, int64 size)
	{
		return SPlatformMisc::ReadFromFile(STDIN_FILE_NO, buffer, size);
	}

	KOR_FORCEINLINE_DEBUGGABLE static int64 WriteToStdout(const void* buffer, int64 size)
	{
		return SPlatformMisc::WriteToFile(STDOUT_FILE_NO, buffer, size);
	}

	KOR_FORCEINLINE_DEBUGGABLE static int64 WriteToStderr(const void* buffer, int64 size)
	{
		return SPlatformMisc::WriteToFile(STDERR_FILE_NO, buffer, size);
	}
};

KOR_NAMESPACE_END