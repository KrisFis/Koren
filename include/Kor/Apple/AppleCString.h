// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <cstdio>
#include <cctype>
#include <cwchar>
#include <cwctype>

#include "ASTD/Apple/AppleBuild.h"

struct SApplePlatformCString
{
	// Checks whether the passed character is alphabetic
	FORCEINLINE static bool IsAlphaChar(char val) { return isalpha(val); }
	FORCEINLINE static bool IsAlphaChar(wchar val) { return iswalpha(val); }

	// Checks whether the passed character is alphanumeric
	FORCEINLINE static bool IsAlphaNumericChar(char val) { return isalnum(val); }
	FORCEINLINE static bool IsAlphaNumericChar(wchar val) { return iswalnum(val); }

	// Checks whether the passed character is numeric
	FORCEINLINE static bool IsNumberChar(char val) { return !isalpha(val) && isalnum(val); }
	FORCEINLINE static bool IsNumberChar(wchar val) { return !iswalpha(val) && iswalnum(val); }

	// Checks whether the passed character is white-space.
	FORCEINLINE static bool IsWhitespaceChar(char val) { return isspace(val); }
	FORCEINLINE static bool IsWhitespaceChar(wchar val) { return iswspace(val); }

	// Checks whether the passed character is printable
	FORCEINLINE static bool IsPrintableChar(char val) { return isprint(val); }
	FORCEINLINE static bool IsPrintableChar(wchar val) { return iswprint(val); }

	// Checks whether the passed character is lowercase letter
	FORCEINLINE static bool IsLowerChar(char val) { return islower(val); }
	FORCEINLINE static bool IsLowerChar(wchar val) { return iswlower(val); }

	// Checks whether the passed character is an uppercase letter
	FORCEINLINE static bool IsUpperChar(char val) { return isupper(val); }
	FORCEINLINE static bool IsUpperChar(wchar val) { return iswupper(val); }

	// Converts uppercase letters to lowercase
	FORCEINLINE static char ToLowerChar(char val) { return tolower(val); }
	FORCEINLINE static wchar ToLowerChar(wchar val) { return towlower(val); }

	// Converts lowercase letters to uppercase
	FORCEINLINE static char ToUpperChar(char val) { return toupper(val); }
	FORCEINLINE static wchar ToUpperChar(wchar val) { return towupper(val); }

	// Converts the string pointed to, by the argument str to an integer
	FORCEINLINE static int32 ToInt32(const char* val) { return atoi(val); }
	FORCEINLINE static int32 ToInt32(const wchar* val) { return wcstol(val, nullptr, 10); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt32(int32 val, char* buf, uint32 maxLen) { snprintf(buf, maxLen, "%d", val); return buf; }
	FORCEINLINE static wchar* FromInt32(int32 val, wchar* buf, uint32 maxLen) { swprintf(buf, maxLen, L"%d", val); return buf; }

	// Converts the string pointed to, by the argument str to a long integer
	FORCEINLINE static int64 ToInt64(const char* val) { return atol(val); }
	FORCEINLINE static int64 ToInt64(const wchar* val) { return wcstol(val, nullptr, 10); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt64(int64 val, char* buf, uint32 maxLen) { snprintf(buf, maxLen, "%lld", val); return buf; }
	FORCEINLINE static wchar* FromInt64(int64 val, wchar* buf, uint32 maxLen) { swprintf(buf, maxLen, L"%lld", val); return buf; }

	// Converts the string pointed to, by the argument str to a floating-point number
	FORCEINLINE static double ToDouble(const char* val) { return atof(val); }
	FORCEINLINE static double ToDouble(const wchar* val) { return wcstod(val, nullptr); }

	// Converts double value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromDouble(double val, int32 Digits, char* buf, uint32 maxLen) { snprintf(buf, maxLen, "%.*f", Digits, val); return buf; }
	FORCEINLINE static wchar* FromDouble(double val, int32 Digits, wchar* buf, uint32 maxLen) { swprintf(buf, maxLen, L"%.*f", Digits, val); return buf; }
};
