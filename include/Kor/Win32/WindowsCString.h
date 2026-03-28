// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Win32/WindowsBuild.h"

struct SWindowsPlatformCString
{
	// Checks whether the passed character is alphabetic
	FORCEINLINE static bool IsAlphaChar(char val) { return IsCharAlphaA(val); }
	FORCEINLINE static bool IsAlphaChar(wchar val) { return IsCharAlphaW(val); }

	// Checks whether the passed character is alphanumeric
	FORCEINLINE static bool IsAlphaNumericChar(char val) { return IsCharAlphaNumericA(val); }
	FORCEINLINE static bool IsAlphaNumericChar(wchar val) { return IsCharAlphaNumericW(val); }

	// Checks whether the passed character is numeric
	FORCEINLINE static bool IsNumberChar(char val) { return !IsCharAlphaA(val) && IsCharAlphaNumericA(val); }
	FORCEINLINE static bool IsNumberChar(wchar val) { return !IsCharAlphaW(val) && IsCharAlphaNumericW(val); }

	// Checks whether the passed character is white-space.
	FORCEINLINE static bool IsWhitespaceChar(char val) { return isspace(val); }
	FORCEINLINE static bool IsWhitespaceChar(wchar val) { return iswspace(val); }

	// Checks whether the passed character is printable
	FORCEINLINE static bool IsPrintableChar(char val) { return isprint(val); }
	FORCEINLINE static bool IsPrintableChar(wchar val) { return iswprint(val); }

	// Checks whether the passed character is lowercase letter
	FORCEINLINE static bool IsLowerChar(char val) { return IsCharLowerA(val); }
	FORCEINLINE static bool IsLowerChar(wchar val) { return IsCharLowerW(val); }

	// Checks whether the passed character is an uppercase letter
	FORCEINLINE static bool IsUpperChar(char val) { return IsCharUpperA(val); }
	FORCEINLINE static bool IsUpperChar(wchar val) { return IsCharUpperW(val); }

	// Converts uppercase letters to lowercase
	FORCEINLINE static char ToLowerChar(char val) { return *CharLowerA(&val); }
	FORCEINLINE static wchar ToLowerChar(wchar val) { return *CharLowerW(&val); }

	// Converts lowercase letters to uppercase
	FORCEINLINE static char ToUpperChar(char val) { return *CharUpperA(&val); }
	FORCEINLINE static wchar ToUpperChar(wchar val) { return *CharUpperW(&val); }

	// Converts the string pointed to, by the argument str to an integer
	FORCEINLINE static int32 ToInt32(const char* val) { return atoi(val); }
	FORCEINLINE static int32 ToInt32(const wchar* val) { return _wtoi(val); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt32(int32 val, char* buf, uint32 maxLen) { _itoa_s(val, buf, maxLen, 10); return buf; }
	FORCEINLINE static wchar* FromInt32(int32 val, wchar* buf, uint32 maxLen) { _itow_s(val, buf, maxLen, 10); return buf; }

	// Converts the string pointed to, by the argument str to a long integer
	FORCEINLINE static int64 ToInt64(const char* val) { return _atoi64(val); }
	FORCEINLINE static int64 ToInt64(const wchar* val) { return _wtoi64(val); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt64(int64 val, char* buf, uint32 maxLen) { _i64toa_s(val, buf, maxLen, 10); return buf; }
	FORCEINLINE static wchar* FromInt64(int64 val, wchar* buf, uint32 maxLen) { _i64tow_s(val, buf, maxLen, 10); return buf; }

	// Converts the string pointed to, by the argument str to a floating-point number
	FORCEINLINE static double ToDouble(const char* val) { return atof(val); }
	FORCEINLINE static double ToDouble(const wchar* val) { return _wtof(val); }

	// Converts double value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromDouble(double val, int32 digits, char* buf, uint32 maxLen) { _gcvt_s(buf, maxLen, val, digits); return buf; }
	static wchar* FromDouble(double val, int32 digits, wchar* buf, uint32 maxLen)
	{
		char tmpBuf[_CVTBUFSIZE];
		_gcvt_s(tmpBuf, maxLen, val, digits);

		for(uint16 i = 0;;++i)
		{
			buf[i] = (wchar)tmpBuf[i];
			if(tmpBuf[i] == '\0')
				break;
		}

		return buf;
	}
};
