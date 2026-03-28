// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Kor/Build.h"

#include <iostream>

KOR_NAMESPACE_BEGIN

#if KOR_DO_CHECKS

	namespace _NAssert
	{
		KOR_DIAG_WARNINGS_PUSH()
		KOR_DIAG_WARNINGS_SUPPRESS(KOR_DIAG_WARNING_NULL_DEREFERENCE)
		KOR_OPTIMIZATIONS_DISABLE();

		KOR_FORCEINLINE static void Crash() { *((uint8*)0) = 0; }

		KOR_OPTIMIZATIONS_RESET();
		KOR_DIAG_WARNINGS_POP()

		KOR_FORCEINLINE static void LogFailed(const char* Expression, const char* File, int32 Line)
		{
			// TODO(jan.kristian.fisera): Make use of native logging, instead of std::cerr
			std::cerr << "Check failed: \"" << Expression << '"' << " at " << File << ':' << Line << std::endl;
		}
	}

	#define KOR_CHECK_IMPL(expression)												\
		(KOR_LIKELY(!!(expression)) || []()											\
		{ 																		\
			_NAssert::LogFailed(#expression, __FILE__, __LINE__);	\
			static bool didBreak = false; 										\
			if(!didBreak) 														\
			{ 																	\
				didBreak = true; KOR_DEBUG_BREAK(); 								\
			}																	\
			return false; 														\
		}())

	#define KOR_CHECKF_IMPL(expression)												\
		(KOR_LIKELY(!!(expression)) || []()											\
		{ 																		\
			_NAssert::LogFailed(#expression, __FILE__, __LINE__);	\
			KOR_DEBUG_BREAK();														\
			_NAssert::Crash();										\
			return false; 														\
		}())

#else

	#define KOR_CHECK_IMPL(expression) (!!(expression))
	#define KOR_CHECKF_IMPL(expression) (!!(expression))

#endif

KOR_NAMESPACE_END