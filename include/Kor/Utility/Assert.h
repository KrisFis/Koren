// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Minimal.h"

#include "Kor/Utility/Detail/AssertHandler.h"

// KOR_ASSERT(statement)
// - Fatal
// - Hard requirement

// KOR_EXPECT(expression)
// - Non-Fatal
// - Recoverable expectation
// - returns evaluated expression

#if KOR_USE_ASSERT
	#define KOR_ASSERT(statement)														\
		if (!(statement)) [[ unlikely ]]												\
		{																				\
			KOR_DETAIL_NAMESPACE::LogFailed(#statement, __FILE__, __LINE__);			\
			KOR_DETAIL_NAMESPACE::Crash();											\
		}

	#define KOR_EXPECT(expression)														\
		(KOR_LIKELY(!!(expression)) || []()												\
		{ 																				\
			KOR_DETAIL_NAMESPACE::LogFailed(#expression, __FILE__, __LINE__);			\
			static bool didBreak = false; 												\
			if (!didBreak) 																\
			{ 																			\
				didBreak = true; KOR_DEBUG_BREAK(); 									\
			}																			\
			return false; 																\
		}())

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

