// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Core/Build.h"
#include "Kor/Misc.h"
#include "Kor/String.h"

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

		KOR_FORCEINLINE static void Crash()
		{
			*((uint8*)0) = 0;
			KOR_UNREACHABLE_CODE();
		}

		KOR_OPTIMIZATIONS_RESET();
		KOR_DIAG_WARNINGS_POP()

		KOR_FORCEINLINE static void LogFailed(const char* Expression, const char* File, int32 Line)
		{
			SMisc::WriteToStderr(*SString::Printf(KTEXT("Assert failed '%s' at '%s:%d'"), Expression, File, Line));
		}
	}

#else
	#define KOR_ASSERT(expression) (!!(expression))
	#define KOR_EXPECT(expression) (!!(expression))
#endif
