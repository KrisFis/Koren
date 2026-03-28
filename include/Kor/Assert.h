// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Build.h"
#include "Kor/Misc.h"
#include "Kor/String.h"

#include <iostream>

#if KOR_USE_ASSERT

KOR_NAMESPACE_BEGIN
namespace Private::Assert
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
KOR_NAMESPACE_END

	#define KOR_ASSERT_IMPL(statement)													\
		if (KOR_UNLIKELY(!(statement)))													\
		{																				\
			KOR_NAMESPACE Private::Assert::LogFailed(#statement, __FILE__, __LINE__);	\
			KOR_NAMESPACE Private::Assert::Crash();										\
		}

	#define KOR_EXPECT_IMPL(expression)													\
		(KOR_LIKELY(!!(expression)) || []()												\
		{ 																				\
			KOR_NAMESPACE Private::Assert::LogFailed(#expression, __FILE__, __LINE__);	\
			static bool didBreak = false; 												\
			if (!didBreak) 																\
			{ 																			\
				didBreak = true; KOR_DEBUG_BREAK(); 									\
			}																			\
			return false; 																\
		}())

#else

	#define KOR_ASSERT_IMPL(expression) (!!(expression))
	#define KOR_EXPECT_IMPL(expression) (!!(expression))

#endif

// Fatal - Hard requirement
#define KOR_ASSERT(statement) KOR_ASSERT_IMPL(statement)

// Non-fatal — Recoverable expectation
// * returns evaluated expression
#define KOR_EXPECT(expression) KOR_EXPECT_IMPL(expression)
