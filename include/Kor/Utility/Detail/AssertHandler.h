// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Minimal.h"

KOR_NAMESPACE_BEGIN

namespace Detail
{
	KOR_DIAG_WARNINGS_PUSH()
	KOR_DIAG_WARNINGS_SUPPRESS(KOR_DIAG_WARNING_NULL_DEREFERENCE)
	KOR_OPTIMIZATIONS_DISABLE();

	KOR_FORCEINLINE static void Crash() noexcept
	{
		*((uint8*)0) = 0;
		KOR_UNREACHABLE();
	}

	KOR_OPTIMIZATIONS_RESET();
	KOR_DIAG_WARNINGS_POP()

	static void LogFailed(const achar* Expression, const achar* File, int32 Line) noexcept
	{
		// TODO: Implement log without including non-minimal features
		// thread_local achar LOG_BUFFER[KOR_BUFFER_SIZE_LARGE];
		// const int32 result = TStringOps<achar>::Format(
		// 	LOG_BUFFER,
		// 	KOR_TEXT_ANSI("ASSERT: '%s' at '%s:%d'\n"),
		// 	Expression,
		// 	File,
		// 	Line
		// );
		//
		// if (result > 1) // > '\0'
		// {
		// 	SIOOps::WriteToStdout(LOG_BUFFER, sizeof(achar) * result);
		// }
	}
}

KOR_NAMESPACE_END