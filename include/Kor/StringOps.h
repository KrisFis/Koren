// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/CharOps.h"

// TODO: Remove
#include "CString.h"
#include <stdio.h>

KOR_NAMESPACE_BEGIN

// Search direction for Find and Replace operations
enum class ESearchDir : uint8
{
	// left to right, returns first match
	Forward = 0,
	// right to left, returns last match
	Backward
};

// Case sensitivity for Find, Replace and Compare operations
enum class ESearchCase : uint8
{
	// exact character match
	Sensitive = 0,
	// case-folded match via CharOps::ToLower
	Insensitive
};

// Output format for FromFloat
enum class EFloatFormat : uint8
{
	// fixed or scientific, whichever is shorter (%g)
	Auto = 0,
	// always decimal notation (%f)
	Fixed,
	// always exponent notation (%e)
	Scientific
};

// String operations for a given character type.
// CharType must satisfy TIsCharacter - i.e. achar, KChar8, wchar, tchar.
// All pointer inputs are expected to be non-null unless stated otherwise.
// Functions accepting len treat it as the number of CharType units to process,
// not byte size. Functions without len require null-terminated input.
template<typename T>
struct TStringOps
{
	static_assert(TIsCharacter<T>::Value, "CharType must be a character");

	using CharType  = T;
	using UCharType = typename TMakeUnsigned<CharType>::Type;

	using CharOps = TCharOps<CharType>;
	using CharConstant  = TCharConstant<CharType>;

	// IsAscii
	////////////////////////////////////////////////

	// Returns true if every character in str falls within the ASCII range (0x00-0x7F).
	// - str: null-terminated input, must not be null
	// Note: always returns true when CharType is achar (assumed ASCII-safe)
	static bool IsAscii(const CharType* str) noexcept;

	// Returns true if the first len characters of str are all within ASCII range.
	// - str: input pointer, must not be null
	// - len: number of characters to check, must be >= 0
	static bool IsAscii(const CharType* str, int32 len) noexcept;

	// Array ref overload - len deduced from array size
	template<int32 N>
	static bool IsAscii(const CharType (&str)[N]) noexcept;

	// IsNumeric
	////////////////////////////////////////////////

	// Returns true if every character in str is a decimal digit (0-9).
	// - str: null-terminated input, must not be null
	// Note: returns true for empty string
	static bool IsNumeric(const CharType* str) noexcept;

	// Returns true if the first len characters are all decimal digits.
	// - str: input pointer, must not be null
	// - len: number of characters to check, must be >= 0
	static bool IsNumeric(const CharType* str, int32 len) noexcept;

	template<int32 N>
	static bool IsNumeric(const CharType (&str)[N]) noexcept;

	// IsUpper
	////////////////////////////////////////////////

	// Returns true if every character in str is uppercase.
	// - str: null-terminated input, must not be null
	static bool IsUpper(const CharType* str) noexcept;

	// Returns true if the first len characters are all uppercase.
	// - str: input pointer, must not be null
	// - len: number of characters to check, must be >= 0
	static bool IsUpper(const CharType* str, int32 len) noexcept;

	template<int32 N>
	static bool IsUpper(const CharType (&str)[N]) noexcept;

	// IsLower
	////////////////////////////////////////////////

	// Returns true if every character in str is lowercase.
	// - str: null-terminated input, must not be null
	static bool IsLower(const CharType* str) noexcept;

	// Returns true if the first len characters are all lowercase.
	// - str: input pointer, must not be null
	// - len: number of characters to check, must be >= 0
	static bool IsLower(const CharType* str, int32 len) noexcept;

	template<int32 N>
	static bool IsLower(const CharType (&str)[N]) noexcept;

	// Compare
	////////////////////////////////////////////////

	// Compares lhs and rhs lexicographically.
	// Returns 0 if equal, negative if lhs < rhs, positive if lhs > rhs.
	// - lhs, rhs: null-terminated inputs, must not be null
	// - Case: compile-time case sensitivity (default Sensitive)
	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Compare(const CharType* lhs, const CharType* rhs) noexcept;

	// Runtime case variant - prefer the template version when Case is known at compile time
	static int32 Compare(const CharType* lhs, const CharType* rhs, ESearchCase searchCase) noexcept;

	// Compares up to len characters.
	// Returns 0 if all len characters are equal, regardless of null terminator.
	// - len: number of characters to compare, must be >= 0
	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Compare(const CharType* lhs, const CharType* rhs, int32 len) noexcept;

	static int32 Compare(const CharType* lhs, const CharType* rhs, int32 len, ESearchCase searchCase) noexcept;

	// Array ref overloads - len deduced from lhs array size
	template<ESearchCase Case = ESearchCase::Sensitive, int32 N>
	static int32 Compare(CharType (&lhs)[N], const CharType* rhs) noexcept;

	template<int32 N>
	static int32 Compare(CharType (&lhs)[N], const CharType* rhs, ESearchCase searchCase) noexcept;

	// Length
	////////////////////////////////////////////////

	// Returns the number of characters before the null terminator.
	// - str: null-terminated input, must not be null
	static int32 Length(const CharType* str) noexcept;

	// Compile-time array size overload - returns N
	template<int32 N>
	static int32 Length(const CharType (&)[N]) noexcept;

	// ToUpper
	////////////////////////////////////////////////

	// Converts all characters in str to uppercase in-place.
	// - str: null-terminated input/output, must not be null
	static void ToUpper(CharType* str) noexcept;

	// Converts the first len characters to uppercase in-place.
	// - len: number of characters to convert, must be >= 0
	static void ToUpper(CharType* str, int32 len) noexcept;

	template<int32 N>
	static void ToUpper(CharType (&str)[N]) noexcept;

	// ToLower
	////////////////////////////////////////////////

	// Converts all characters in str to lowercase in-place.
	// - str: null-terminated input/output, must not be null
	static void ToLower(CharType* str) noexcept;

	// Converts the first len characters to lowercase in-place.
	// - len: number of characters to convert, must be >= 0
	static void ToLower(CharType* str, int32 len) noexcept;

	template<int32 N>
	static void ToLower(CharType (&str)[N]) noexcept;

	// Fill
	////////////////////////////////////////////////

	// Fills len characters of str with c. Does not write a null terminator.
	// - str: output buffer, must not be null
	// - c: character to fill with
	// - len: number of characters to fill, must be >= 0
	static void Fill(CharType* str, CharType c, int32 len) noexcept;

	template<int32 N>
	static void Fill(CharType (&str)[N], CharType c) noexcept;

	// Copy
	////////////////////////////////////////////////

	// Copies src into dest including the null terminator.
	// - dest: output buffer, must be large enough to hold src
	// - src: null-terminated input, must not be null
	// Note: dest and src must not overlap
	static void Copy(CharType* dest, const CharType* src) noexcept;

	// Copies exactly len characters from src into dest. Does not write null terminator.
	// - len: number of characters to copy, must be >= 0
	// Note: dest and src must not overlap
	static void Copy(CharType* dest, const CharType* src, int32 len) noexcept;

	template<int32 N>
	static void Copy(CharType (&dest)[N], const CharType* src) noexcept;

	// Concatenate
	////////////////////////////////////////////////

	// Appends src to the end of dest, writing a null terminator after.
	// - dest: null-terminated buffer with enough remaining capacity for src
	// - src: null-terminated input, must not be null
	static void Concatenate(CharType* dest, const CharType* src) noexcept;

	// Appends len characters of src to dest, writing a null terminator after.
	// - len: number of characters to append from src, must be >= 0
	static void Concatenate(CharType* dest, const CharType* src, int32 len) noexcept;

	template<int32 N>
	static void Concatenate(CharType* dest, const CharType (&src)[N]) noexcept;

	// Find
	////////////////////////////////////////////////

	// Searches for substr in str.
	// Returns the index of the first (Forward) or last (Backward) match, or KOR_INDEX_NONE.
	// - str: null-terminated input, must not be null
	// - substr: null-terminated search pattern, must not be null
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	static int32 Find(const CharType* str, const CharType* substr) noexcept;

	// Runtime variant - prefer template version when Case/Dir are known at compile time
	static int32 Find(const CharType* str, const CharType* substr, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	// Bounded variant - searches only within the first len characters of str.
	// - len: length of str to search within, must be >= 0
	// - subLen: length of substr, must be >= 0
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	static int32 Find(const CharType* str, const CharType* substr, int32 len, int32 subLen) noexcept;

	static int32 Find(const CharType* str, const CharType* substr, int32 len, int32 subLen, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	// Array ref overloads - len and subLen deduced from array sizes
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward, int32 N, int32 R>
	static int32 Find(const CharType (&str)[N], const CharType (&subStr)[R]) noexcept;

	template<int32 N, int32 R>
	static int32 Find(const CharType (&str)[N], const CharType (&subStr)[R], ESearchCase searchCase, ESearchDir searchDir) noexcept;

	// Find (Char)
	////////////////////////////////////////////////

	// Searches for character c in str.
	// Returns the index of the first (Forward) or last (Backward) occurrence, or KOR_INDEX_NONE.
	// - str: null-terminated input, must not be null
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	static int32 Find(const CharType* str, CharType c) noexcept;

	static int32 Find(const CharType* str, CharType c, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	// Bounded variant - searches only within the first len characters.
	// - len: number of characters to search, must be >= 0
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	static int32 Find(const CharType* str, CharType c, int32 len) noexcept;

	static int32 Find(const CharType* str, CharType c, int32 len, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward, int32 N>
	static int32 Find(const CharType (&str)[N], CharType c) noexcept;

	template<int32 N>
	static int32 Find(const CharType (&str)[N], CharType c, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	// Replace
	////////////////////////////////////////////////

	// Replaces all occurrences of from with to in str in-place.
	// Returns the number of replacements made.
	// - str: null-terminated input/output, must not be null
	// - from: null-terminated pattern to search for, must not be null
	// - to: null-terminated replacement string, must not be null
	// Note: uses a thread-local scratch buffer of SCString::LARGE_BUFFER_SIZE
	// Warning: result must fit within LARGE_BUFFER_SIZE or behavior is undefined
	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Replace(CharType* str, const CharType* from, const CharType* to) noexcept;

	static int32 Replace(CharType* str, const CharType* from, const CharType* to, ESearchCase searchCase) noexcept;

	// Bounded variant - operates only on the first len characters of str.
	// - len: number of characters to process, must be >= 0
	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Replace(CharType* str, const CharType* from, const CharType* to, int32 len) noexcept;

	static int32 Replace(CharType* str, const CharType* from, const CharType* to, int32 len, ESearchCase searchCase) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, int32 N>
	static int32 Replace(CharType (&str)[N], const CharType* from, const CharType* to) noexcept;

	template<int32 N>
	static int32 Replace(CharType (&str)[N], const CharType* from, const CharType* to, ESearchCase searchCase) noexcept;

	// Replace (Char)
	////////////////////////////////////////////////

	// Replaces all occurrences of character from with to in str in-place.
	// Returns the number of replacements made.
	// - str: null-terminated input/output, must not be null
	// Note: when Case is Insensitive, both upper and lower forms of from are replaced with to as-is
	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Replace(CharType* str, CharType from, CharType to) noexcept;

	static int32 Replace(CharType* str, CharType from, CharType to, ESearchCase searchCase) noexcept;

	// Bounded variant
	// - len: number of characters to process, must be >= 0
	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Replace(CharType* str, CharType from, CharType to, int32 len) noexcept;

	static int32 Replace(CharType* str, CharType from, CharType to, int32 len, ESearchCase searchCase) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, int32 N>
	static int32 Replace(CharType (&str)[N], CharType from, CharType to) noexcept;

	template<int32 N>
	static int32 Replace(CharType (&str)[N], CharType from, CharType to, ESearchCase searchCase) noexcept;

	// Format | FormatLength
	////////////////////////////////////////////////

	// Writes a formatted string into str using printf-style fmt.
	// Returns the number of CharType units written, excluding null terminator. Returns negative on error. (may exceed len on truncation).
	// - str: output buffer, must not be null
	// - fmt: null-terminated format string, must not be null
	// - len: size of str buffer in characters, must be > 0
	// Warning: does not guarantee null termination - caller is responsible.
	template<typename... ArgT>
	static int32 Format(CharType* str, const CharType* fmt, int32 len, const ArgT&... args) noexcept;

	template<int32 N, typename... ArgT>
	static int32 Format(CharType (&str)[N], const CharType* fmt, const ArgT&... args) noexcept;

	// ToInt32 | ToInt64 | ToUInt32 | ToUInt64
	////////////////////////////////////////////////

	// Parses a null-terminated string as a signed 32-bit integer.
	// - str: null-terminated input, must not be null
	// - base: numeric base (2-36), default 10
	// Note: does not skip whitespace - input must start with optional sign then digits
	// Note: stops at first character that is not a valid digit in base
	static int32 ToInt32(const CharType* str, int32 base = 10) noexcept;

	// outEnd variant - sets outEnd to the first character not consumed.
	// - outEnd: receives pointer to first unconsumed character, must not be null
	static int32 ToInt32(const CharType* str, const CharType*& outEnd, int32 base = 10) noexcept;

	static int64 ToInt64(const CharType* str, int32 base = 10) noexcept;
	static int64 ToInt64(const CharType* str, const CharType*& outEnd, int32 base = 10) noexcept;

	static uint32 ToUInt32(const CharType* str, int32 base = 10) noexcept;
	static uint32 ToUInt32(const CharType* str, const CharType*& outEnd, int32 base = 10) noexcept;

	static uint64 ToUInt64(const CharType* str, int32 base = 10) noexcept;
	static uint64 ToUInt64(const CharType* str, const CharType*& outEnd, int32 base = 10) noexcept;

	// FromInt32 | FromInt64 | FromUInt32 | FromUInt64
	////////////////////////////////////////////////

	// Writes the integer value as a string into str.
	// Returns the number of characters written, not including null terminator.
	// - str: output buffer, must be large enough (see SCString::MAX_INT32_STR_LEN etc.)
	// - value: integer value to convert
	// - base: numeric base (2-36), default 10
	static int32 FromInt32(CharType* str, int32 value, int32 base = 10) noexcept;

	// Bounded variant - returns -1 if buffer is too small.
	// - len: size of str buffer in characters, must be > 0
	static int32 FromInt32(CharType* str, int32 value, int32 len, int32 base = 10) noexcept;

	static int32 FromInt64(CharType* str, int64 value, int32 base = 10) noexcept;
	static int32 FromInt64(CharType* str, int64 value, int32 len, int32 base = 10) noexcept;

	static int32 FromUInt32(CharType* str, uint32 value, int32 base = 10) noexcept;
	static int32 FromUInt32(CharType* str, uint32 value, int32 len, int32 base = 10) noexcept;

	static int32 FromUInt64(CharType* str, uint64 value, int32 base = 10) noexcept;
	static int32 FromUInt64(CharType* str, uint64 value, int32 len, int32 base = 10) noexcept;

	// Array ref overloads - buffer size deduced from array
	template<int32 N> static int32 FromInt32(CharType (&str)[N], int32 value, int32 base = 10) noexcept;
	template<int32 N> static int32 FromInt64(CharType (&str)[N], int64 value, int32 base = 10) noexcept;
	template<int32 N> static int32 FromUInt32(CharType (&str)[N], uint32 value, int32 base = 10) noexcept;
	template<int32 N> static int32 FromUInt64(CharType (&str)[N], uint64 value, int32 base = 10) noexcept;

	// ToFloat
	////////////////////////////////////////////////

	// Parses a null-terminated string as a double.
	// - str: null-terminated input, must not be null
	// Note: delegates to strtod/wcstod - whitespace is skipped per CRT behavior
	static double ToFloat(const CharType* str) noexcept;

	// outEnd variant - sets outEnd to the first character not consumed.
	static double ToFloat(const CharType* str, const CharType*& outEnd) noexcept;

	// FromFloat
	////////////////////////////////////////////////

	// Writes a double value as a formatted string into str.
	// Returns the number of characters written, not including null terminator.
	// - str: output buffer, must be at least SCString::MAX_BUFFER_SIZE_DOUBLE characters
	// - value: double value to convert
	// - precision: number of significant digits or decimal places (default 6)
	// - Format: compile-time output format (Auto, Fixed, Scientific)
	template<EFloatFormat Format = EFloatFormat::Fixed>
	static int32 FromFloat(CharType* str, double value, int32 precision = 6) noexcept;

	// Runtime format variant - prefer template version when Format is known at compile time
	static int32 FromFloat(CharType* str, double value, int32 precision, EFloatFormat format) noexcept;

	// Bounded variant - writes at most len characters.
	// Returns would-be length on truncation (same as snprintf semantics).
	// - len: size of str buffer in characters
	template<EFloatFormat Format = EFloatFormat::Fixed>
	static int32 FromFloat(CharType* str, double value, int32 len, int32 precision = 6) noexcept;

	static int32 FromFloat(CharType* str, double value, int32 len, int32 precision, EFloatFormat format) noexcept;

	template<EFloatFormat Format = EFloatFormat::Fixed, int32 N>
	static int32 FromFloat(CharType (&str)[N], double value, int32 precision = 6) noexcept;

	template<int32 N>
	static int32 FromFloat(CharType (&str)[N], double value, int32 precision, EFloatFormat format) noexcept;
};

#include "Kor/Internal/StringOps.inl"

// Convenience typedefs for tchar
using SStringOps = TStringOps<tchar>;

KOR_NAMESPACE_END
