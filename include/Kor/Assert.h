// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Core/Build.h"

#include "Kor/StringOps.h"
#include "Kor/Memory.h"
#include "Kor/Misc.h"

// TODO: Decouple assert from logging and let user provide function pointer that will be called on assert

// KOR_ASSERT(statement)
// - Fatal
// - Hard requirement

// KOR_EXPECT(expression)
// - Non-Fatal
// - Recoverable expectation
// - returns evaluated expression

#if KOR_USE_ASSERT

	#define KOR_ASSERT(statement)													\
		if (KOR_UNLIKELY(!(statement)))													\
		{																				\
			KOR_NAMESPACE Internal::LogFailed(#statement, __FILE__, __LINE__);	\
			KOR_NAMESPACE Internal::Crash();										\
		}

	#define KOR_EXPECT(expression)													\
		(KOR_LIKELY(!!(expression)) || []()												\
		{ 																				\
			KOR_NAMESPACE Internal::LogFailed(#expression, __FILE__, __LINE__);	\
			static bool didBreak = false; 												\
			if (!didBreak) 																\
			{ 																			\
				didBreak = true; KOR_DEBUG_BREAK(); 									\
			}																			\
			return false; 																\
		}())

	namespace KOR_NAMESPACE Internal
	{
		KOR_DIAG_WARNINGS_PUSH()
		KOR_DIAG_WARNINGS_SUPPRESS(KOR_DIAG_WARNING_NULL_DEREFERENCE)
		KOR_OPTIMIZATIONS_DISABLE();

		KOR_FORCEINLINE static void Crash() noexcept
		{
			*((uint8*)0) = 0;
			KOR_UNREACHABLE_CODE();
		}

		KOR_OPTIMIZATIONS_RESET();
		KOR_DIAG_WARNINGS_POP()

		static void LogFailed(const achar* Expression, const achar* File, int32 Line) noexcept
		{
			thread_local achar LOG_BUFFER[SMemory::BUFFER_SIZE_LARGE];
			const int32 result = TStringOps<achar>::Format(
				LOG_BUFFER,
				KOR_TEXT_ANSI("ASSERT: '%s' at '%s:%d'\n"),
				Expression,
				File,
				Line
			);

			if (result > 1) // > '\0'
			{
				SMisc::WriteToStdout(LOG_BUFFER, sizeof(achar) * result);
			}
		}
	}

#if KOR_BUILD_DEBUG
	#define KOR_ASSERT_DEBUG(statement) KOR_ASSERT(statement)
	#define KOR_EXPECT_DEBUG(statement) KOR_EXPECT(statement)
#else
	#define KOR_ASSERT_DEBUG(statement)
	#define KOR_EXPECT_DEBUG(expression) (!!(expression))
#endif

#else
	#define KOR_ASSERT(statement)
	#define KOR_EXPECT(expression) (!!(expression))

	#define KOR_ASSERT_DEBUG(statement)
	#define KOR_EXPECT_DEBUG(expression) (!!(expression))
#endif

