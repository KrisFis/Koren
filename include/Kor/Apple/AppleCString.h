// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <cstdio>
#include <cctype>
#include <cwchar>
#include <cwctype>

#include "Kor/Apple/AppleBuild.h"

struct SApplePlatformCString
{
	// Checks whether the passed character is alphabetic
	KOR_FORCEINLINE static bool IsAlphaChar(char val) { return isalpha(val); }
	KOR_FORCEINLINE static bool IsAlphaChar(wchar val) { return iswalpha(val); }

	// Checks whether the passed character is alphanumeric
	KOR_FORCEINLINE static bool IsAlphaNumericChar(char val) { return isalnum(val); }
	KOR_FORCEINLINE static bool IsAlphaNumericChar(wchar val) { return iswalnum(val); }

	// Checks whether the passed character is numeric
	KOR_FORCEINLINE static bool IsNumberChar(char val) { return !isalpha(val) && isalnum(val); }
	KOR_FORCEINLINE static bool IsNumberChar(wchar val) { return !iswalpha(val) && iswalnum(val); }

	// Checks whether the passed character is white-space.
	KOR_FORCEINLINE static bool IsWhitespaceChar(char val) { return isspace(val); }
	KOR_FORCEINLINE static bool IsWhitespaceChar(wchar val) { return iswspace(val); }

	// Checks whether the passed character is printable
	KOR_FORCEINLINE static bool IsPrintableChar(char val) { return isprint(val); }
	KOR_FORCEINLINE static bool IsPrintableChar(wchar val) { return iswprint(val); }

	// Checks whether the passed character is lowercase letter
	KOR_FORCEINLINE static bool IsLowerChar(char val) { return islower(val); }
	KOR_FORCEINLINE static bool IsLowerChar(wchar val) { return iswlower(val); }

	// Checks whether the passed character is an uppercase letter
	KOR_FORCEINLINE static bool IsUpperChar(char val) { return isupper(val); }
	KOR_FORCEINLINE static bool IsUpperChar(wchar val) { return iswupper(val); }

	// Converts uppercase letters to lowercase
	KOR_FORCEINLINE static char ToLowerChar(char val) { return tolower(val); }
	KOR_FORCEINLINE static wchar ToLowerChar(wchar val) { return towlower(val); }

	// Converts lowercase letters to uppercase
	KOR_FORCEINLINE static char ToUpperChar(char val) { return toupper(val); }
	KOR_FORCEINLINE static wchar ToUpperChar(wchar val) { return towupper(val); }

	// Converts the string pointed to, by the argument str to an integer
	KOR_FORCEINLINE static int32 ToInt32(const char* val) { return atoi(val); }
	KOR_FORCEINLINE static int32 ToInt32(const wchar* val) { return wcstol(val, nullptr, 10); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	KOR_FORCEINLINE static char* FromInt32(int32 val, char* buf, uint32 maxLen) { snprintf(buf, maxLen, "%d", val); return buf; }
	KOR_FORCEINLINE static wchar* FromInt32(int32 val, wchar* buf, uint32 maxLen) { swprintf(buf, maxLen, L"%d", val); return buf; }

	// Converts the string pointed to, by the argument str to a long integer
	KOR_FORCEINLINE static int64 ToInt64(const char* val) { return atol(val); }
	KOR_FORCEINLINE static int64 ToInt64(const wchar* val) { return wcstol(val, nullptr, 10); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	KOR_FORCEINLINE static char* FromInt64(int64 val, char* buf, uint32 maxLen) { snprintf(buf, maxLen, "%lld", val); return buf; }
	KOR_FORCEINLINE static wchar* FromInt64(int64 val, wchar* buf, uint32 maxLen) { swprintf(buf, maxLen, L"%lld", val); return buf; }

	// Converts the string pointed to, by the argument str to a floating-point number
	KOR_FORCEINLINE static double ToDouble(const char* val) { return atof(val); }
	KOR_FORCEINLINE static double ToDouble(const wchar* val) { return wcstod(val, nullptr); }

	// Converts double value to string in specified base, string is saved to provided buffer
	KOR_FORCEINLINE static char* FromDouble(double val, int32 Digits, char* buf, uint32 maxLen) { snprintf(buf, maxLen, "%.*f", Digits, val); return buf; }
	KOR_FORCEINLINE static wchar* FromDouble(double val, int32 Digits, wchar* buf, uint32 maxLen) { swprintf(buf, maxLen, L"%.*f", Digits, val); return buf; }
};
