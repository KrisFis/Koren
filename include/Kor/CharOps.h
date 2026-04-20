// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"

KOR_NAMESPACE_BEGIN

// [ Char Constant ]
// Per-type character constants for all standard ASCII control, whitespace,
// linebreak, and operator characters.
// * All values are cast to CharType at compile time - safe for any character width
// * Bogus degrades to '?' for narrow types that cannot represent U+FFFD
template <typename CharType>
struct TCharConstant
{
	static_assert(TIsCharacter<CharType>::Value, "CharType must be a character");

	// Sentinel
	///////////////////////////////////////////////////////////////////////////////////

	// U+FFFD for wide types, '?' for narrow (ANSI cannot represent U+FFFD)
	static constexpr CharType Bogus = sizeof(CharType) > 1
		? (CharType)0xFFFD
		: (CharType)0x3F;

	// Linebreak
	///////////////////////////////////////////////////////////////////////////////////

	// '\n' - Unix/Linux/macOS line ending
	static constexpr CharType LineFeed			= (CharType)0xa;
	// '\v' - legacy vertical tab, treated as whitespace by C standard
	static constexpr CharType VerticalTab		= (CharType)0xb;
	// '\f' - legacy page break, treated as whitespace by C standard
	static constexpr CharType FormFeed			= (CharType)0xc;
	// '\r' - part of Windows CRLF, alone on old Mac Classic
	static constexpr CharType CarriageReturn	= (CharType)0xd;

	// Whitespace
	///////////////////////////////////////////////////////////////////////////////////

	// ' ' - standard space
	static constexpr CharType Space				= (CharType)0x20;
	// '\t' - horizontal tab, commonly 4 or 8 spaces wide
	static constexpr CharType Tab				= (CharType)0x9;

	// Control
	///////////////////////////////////////////////////////////////////////////////////

	// '\0' - string terminator in C-style strings
	static constexpr CharType Null				= (CharType)0x0;
	// '\b' - moves cursor one position back
	static constexpr CharType Backspace			= (CharType)0x8;
	// '\e' - begins escape sequences (e.g. ANSI terminal codes)
	static constexpr CharType Escape			= (CharType)0x1b;
	// DEL - opposite of Null, often used as "invalid" sentinel
	static constexpr CharType Delete			= (CharType)0x7f;

	// Arithmetic operators
	///////////////////////////////////////////////////////////////////////////////////

	// '+' - positive sign / addition operator
	static constexpr CharType Plus				= (CharType)0x2B;

	// '-' - negative sign / subtraction operator
	static constexpr CharType Minus				= (CharType)0x2D;

	// '*' - multiplication operator
	static constexpr CharType Asterisk			= (CharType)0x2A;

	// '/' - division operator / path separator
	static constexpr CharType Slash				= (CharType)0x2F;

	// '\' - escape character / path separator
	static constexpr CharType Backslash			= (CharType)0x5C;

	// '%' - modulo operator
	static constexpr CharType Percent			= (CharType)0x25;

	// '=' - assignment / equality operator
	static constexpr CharType Equal				= (CharType)0x3D;

	// '<' - less than operator
	static constexpr CharType Less				= (CharType)0x3C;

	// '>' - greater than operator
	static constexpr CharType Greater			= (CharType)0x3E;

	// '!' - logical not operator
	static constexpr CharType Exclamation		= (CharType)0x21;

	// '&' - bitwise and / reference operator
	static constexpr CharType Ampersand			= (CharType)0x26;

	// '|' - bitwise or operator
	static constexpr CharType Pipe				= (CharType)0x7C;

	// '^' - bitwise xor operator
	static constexpr CharType Caret				= (CharType)0x5E;

	// '~' - bitwise not operator
	static constexpr CharType Tilde				= (CharType)0x7E;
};

// [ Char Ops ]
// Character classification and conversion operations.
// * ASCII only (0x00-0x7F) — codepoints outside this range always return
//   false for classification and are returned unchanged by conversion
// * No locale, no CRT dependency, no Unicode category tables
// * Bitwise operators (& |) used instead of logical (&& ||) throughout
//   to avoid short-circuit branching on cheap scalar comparisons
// * UCharType cast guards against sign extension on signed char types
//   e.g. '\x80' as signed char would sign-extend to 0xFFFFFF80 before
//   arithmetic, producing incorrect range check results
template <typename T>
struct TCharOps
{
	static_assert(TIsCharacter<T>::Value, "T must be a character type");

	using CharType      = T;
	using UCharType     = typename TMakeUnsigned<CharType>::Type;
	using CharConstant  = TCharConstant<CharType>;

	// Classification
	///////////////////////////////////////////////////////////////////////////////////

	// Returns true if c is within the ASCII range (0x00-0x7F)
	// Useful as a guard before applying any ASCII-specific classification
	// to wide character types
	static constexpr bool IsAscii(CharType c) noexcept;

	// Returns true if c is a decimal digit (0-9)
	static constexpr bool IsDigit(CharType c) noexcept;

	// Returns true if c is an uppercase ASCII letter (A-Z)
	static constexpr bool IsUpperAlpha(CharType c) noexcept;

	// Returns true if c is a lowercase ASCII letter (a-z)
	static constexpr bool IsLowerAlpha(CharType c) noexcept;

	// Returns true if c is an ASCII letter (a-z, A-Z)
	static constexpr bool IsAlpha(CharType c) noexcept;

	// Returns true if c is an uppercase ASCII letter (A-Z)
	// Alias for IsUpperAlpha — ASCII only, no locale
	static constexpr bool IsUpper(CharType c) noexcept;

	// Returns true if c is a lowercase ASCII letter (a-z)
	// Alias for IsLowerAlpha — ASCII only, no locale
	static constexpr bool IsLower(CharType c) noexcept;

	// Returns true if c is an ASCII letter or decimal digit (a-z, A-Z, 0-9)
	static constexpr bool IsAlnum(CharType c) noexcept;

	// Returns true if c is a valid hexadecimal digit (0-9, a-f, A-F)
	static constexpr bool IsHexDigit(CharType c) noexcept;

	// Returns true if c is a sign character (+ or -)
	// Useful for number parsing
	static constexpr bool IsSign(CharType c) noexcept;

	// Returns true if c is an ASCII whitespace character
	// Covers: space (0x20), tab (0x09), line feed (0x0A),
	//         carriage return (0x0D), vertical tab (0x0B), form feed (0x0C)
	static constexpr bool IsWhitespace(CharType c) noexcept;

	// Returns true if c is an ASCII control character
	// Covers: C0 controls (0x00-0x1F) and DEL (0x7F)
	static constexpr bool IsControl(CharType c) noexcept;

	// Returns true if c is a printable ASCII character (0x20-0x7E)
	// Includes space — use IsGraph to exclude it
	static constexpr bool IsPrint(CharType c) noexcept;

	// Returns true if c is a visible (graphical) ASCII character (0x21-0x7E)
	// Excludes space — use IsPrint to include it
	static constexpr bool IsGraph(CharType c) noexcept;

	// Returns true if c is an ASCII punctuation or symbol character
	// Defined as: graphical (0x21-0x7E) and non-alphanumeric
	// e.g. !, @, #, [, ], ., ,
	static constexpr bool IsPunct(CharType c) noexcept;

	// Returns true if c is an ASCII line-ending character
	// Covers: LF (0x0A), VT (0x0B), FF (0x0C), CR (0x0D)
	// Uses unsigned subtraction range trick to collapse 4 values into one check
	// Unicode line endings (NEL, LS, PS) are intentionally excluded — ASCII only
	static constexpr bool IsLinebreak(CharType c) noexcept;

	// Conversion
	///////////////////////////////////////////////////////////////////////////////////

	// Converts a digit character to its integer value (0-9)
	// Returns KOR_INDEX_NONE if c is not a valid digit
	static constexpr int32 DigitToInt(CharType c) noexcept;

	// Converts a hexadecimal digit character to its integer value (0-15)
	// '0'-'9' -> 0-9, 'a'-'f' -> 10-15, 'A'-'F' -> 10-15
	// Returns KOR_INDEX_NONE if c is not a valid hex digit
	static constexpr int32 HexToInt(CharType c) noexcept;

	// Converts an integer value (0-9) to its digit character
	// Returns Null if n is outside 0-9 range
	static constexpr int32 IntToDigit(int32 n) noexcept;

	// Converts an integer value (0-15) to its hexadecimal digit character
	// 0-9 -> '0'-'9', 10-15 -> 'a'-'f' (lowercase) or 'A'-'F' (uppercase)
	// Returns Null if n is outside 0-15 range
	static constexpr CharType IntToHex(int32 n, bool bUpper = false) noexcept;

	// Casing
	///////////////////////////////////////////////////////////////////////////////////

	// Converts c to uppercase if it is a lowercase ASCII letter (a-z)
	// Characters outside a-z are returned unchanged — no locale, no Unicode folding
	// Uses bit manipulation: subtracts 0x20 (the case bit) when c is in a-z range
	static constexpr CharType ToUpper(CharType c) noexcept;

	// Converts c to lowercase if it is an uppercase ASCII letter (A-Z)
	// Characters outside A-Z are returned unchanged — no locale, no Unicode folding
	// Uses bit manipulation: adds 0x20 (the case bit) when c is in A-Z range
	static constexpr CharType ToLower(CharType c) noexcept;
};

#include "Kor/Internal/CharOps.inl"

// Convenience typedefs for tchar
using SCharOps = TCharOps<tchar>;
using SCharConstant = TCharConstant<tchar>;

KOR_NAMESPACE_END
