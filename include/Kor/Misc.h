// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "ASTD/CString.h"
#include PLATFORM_HEADER(Misc)

typedef PLATFORM_PREFIXED_TYPE(S, PlatformMisc) SPlatformMisc;
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

	FORCEINLINE_DEBUGGABLE static int64 ReadFromStdin(void* buffer, int64 size)
	{
		return SPlatformMisc::ReadFromFile(STDIN_FILE_NO, buffer, size);
	}

	FORCEINLINE_DEBUGGABLE static int64 WriteToStdout(const void* buffer, int64 size)
	{
		return SPlatformMisc::WriteToFile(STDOUT_FILE_NO, buffer, size);
	}

	FORCEINLINE_DEBUGGABLE static int64 WriteToStderr(const void* buffer, int64 size)
	{
		return SPlatformMisc::WriteToFile(STDERR_FILE_NO, buffer, size);
	}

	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE static uint64 WriteToStdout(const CharType* str)
	{
		const uint64 writtenBytes = SPlatformMisc::WriteToFile(STDOUT_FILE_NO, str, SCString::GetLength(str) * sizeof(CharType));
		return writtenBytes >= sizeof(tchar) ? writtenBytes / sizeof(tchar) : 0;
	}

	template<typename CharType, typename TEnableIf<TIsCharacter<CharType>::Value>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE static uint64 WriteToStderr(const CharType* str)
	{
		const uint64 writtenBytes = SPlatformMisc::WriteToFile(STDERR_FILE_NO, str, SCString::GetLength(str) * sizeof(CharType));
		return writtenBytes >= sizeof(tchar) ? writtenBytes / sizeof(tchar) : 0;
	}
};
