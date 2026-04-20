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

// [ String Ops ]
// String operations for a given character type.
// * CharType must satisfy TIsCharacter
// * All pointer inputs must be non-null unless stated otherwise
// * len is always in CharType units, never bytes
// * Functions without len require null-terminated input
// * Array ref overloads deduce len from array size at compile time
//   * prefer them over pointer overloads when the buffer size is known
template<typename T>
struct TStringOps
{
	static_assert(TIsCharacter<T>::Value, "CharType must be a character");

	using CharType  = T;
	using UCharType = typename TMakeUnsigned<CharType>::Type;

	using CharOps = TCharOps<CharType>;
	using CharConstant  = TCharConstant<CharType>;

	// IsAscii
	// Returns true if all characters in the string fall within the ASCII range (0x00-0x7F).
	// * Always returns true when CharType is achar (assumed ASCII-safe)
	///////////////////////////////////////////////////////////////////////////////////

	static bool IsAscii(const CharType* str) noexcept;

	static bool IsAscii(const CharType* str, int32 len) noexcept;

	template<int32 N>
	static bool IsAscii(const CharType (&str)[N]) noexcept;

	// IsNumeric
	// Returns true if all characters in the string are decimal digits (0-9).
	// * Returns true for empty string
	///////////////////////////////////////////////////////////////////////////////////

	static bool IsNumeric(const CharType* str) noexcept;

	static bool IsNumeric(const CharType* str, int32 len) noexcept;

	template<int32 N>
	static bool IsNumeric(const CharType (&str)[N]) noexcept;

	// IsUpper
	// Returns true if all characters in the string are uppercase.
	///////////////////////////////////////////////////////////////////////////////////

	static bool IsUpper(const CharType* str) noexcept;

	static bool IsUpper(const CharType* str, int32 len) noexcept;

	template<int32 N>
	static bool IsUpper(const CharType (&str)[N]) noexcept;

	// IsLower
	// Returns true if all characters in the string are lowercase.
	///////////////////////////////////////////////////////////////////////////////////

	static bool IsLower(const CharType* str) noexcept;

	static bool IsLower(const CharType* str, int32 len) noexcept;

	template<int32 N>
	static bool IsLower(const CharType (&str)[N]) noexcept;

	// Compare
	// Compares two strings lexicographically.
	// Returns 0 if equal, negative if lhs < rhs, positive if lhs > rhs.
	// * Prefer the template overload when Case is known at compile time
	///////////////////////////////////////////////////////////////////////////////////

	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Compare(const CharType* lhs, const CharType* rhs) noexcept;

	static int32 Compare(const CharType* lhs, const CharType* rhs, ESearchCase searchCase) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Compare(const CharType* lhs, const CharType* rhs, int32 len) noexcept;

	static int32 Compare(const CharType* lhs, const CharType* rhs, int32 len, ESearchCase searchCase) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, int32 N>
	static int32 Compare(CharType (&lhs)[N], const CharType* rhs) noexcept;

	template<int32 N>
	static int32 Compare(CharType (&lhs)[N], const CharType* rhs, ESearchCase searchCase) noexcept;

	// Length
	// Returns the number of characters before the null terminator.
	///////////////////////////////////////////////////////////////////////////////////

	static int32 Length(const CharType* str) noexcept;

	template<int32 N>
	static int32 Length(const CharType (&)[N]) noexcept;

	// ToUpper
	// Converts all characters in the string to uppercase in-place.
	///////////////////////////////////////////////////////////////////////////////////

	static void ToUpper(CharType* str) noexcept;

	static void ToUpper(CharType* str, int32 len) noexcept;

	template<int32 N>
	static void ToUpper(CharType (&str)[N]) noexcept;

	// ToLower
	// Converts all characters in the string to lowercase in-place.
	///////////////////////////////////////////////////////////////////////////////////

	static void ToLower(CharType* str) noexcept;

	static void ToLower(CharType* str, int32 len) noexcept;

	template<int32 N>
	static void ToLower(CharType (&str)[N]) noexcept;

	// Fill
	// Fills len characters of the buffer with the given character.
	// * Does not write a null terminator
	///////////////////////////////////////////////////////////////////////////////////

	static void Fill(CharType* str, CharType c, int32 len) noexcept;

	template<int32 N>
	static void Fill(CharType (&str)[N], CharType c) noexcept;

	// Copy
	// Copies src into dest including the null terminator.
	// * dest must be large enough to hold src
	// * dest and src must not overlap
	///////////////////////////////////////////////////////////////////////////////////

	static void Copy(CharType* dest, const CharType* src) noexcept;

	static void Copy(CharType* dest, const CharType* src, int32 len) noexcept;

	template<int32 N>
	static void Copy(CharType (&dest)[N], const CharType* src) noexcept;

	// Concatenate
	// Appends src to the end of dest, writing a null terminator after.
	// * dest must have enough remaining capacity to hold src
	///////////////////////////////////////////////////////////////////////////////////

	static void Concatenate(CharType* dest, const CharType* src) noexcept;

	static void Concatenate(CharType* dest, const CharType* src, int32 len) noexcept;

	template<int32 N>
	static void Concatenate(CharType* dest, const CharType (&src)[N]) noexcept;

	// Find
	// Searches for a substring in str.
	// Returns the index of the first (Forward) or last (Backward) match, or KOR_INDEX_NONE if not found.
	// * Prefer the template overload when Case and Dir are known at compile time
	///////////////////////////////////////////////////////////////////////////////////

	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	static int32 Find(const CharType* str, const CharType* substr) noexcept;

	static int32 Find(const CharType* str, const CharType* substr, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	static int32 Find(const CharType* str, const CharType* substr, int32 len, int32 subLen) noexcept;

	static int32 Find(const CharType* str, const CharType* substr, int32 len, int32 subLen, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward, int32 N, int32 R>
	static int32 Find(const CharType (&str)[N], const CharType (&subStr)[R]) noexcept;

	template<int32 N, int32 R>
	static int32 Find(const CharType (&str)[N], const CharType (&subStr)[R], ESearchCase searchCase, ESearchDir searchDir) noexcept;

	// Find (Char)
	// Searches for a character in str.
	// Returns the index of the first (Forward) or last (Backward) occurrence, or KOR_INDEX_NONE if not found.
	// * Prefer the template overload when Case and Dir are known at compile time
	///////////////////////////////////////////////////////////////////////////////////

	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	static int32 Find(const CharType* str, CharType c) noexcept;

	static int32 Find(const CharType* str, CharType c, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	static int32 Find(const CharType* str, CharType c, int32 len) noexcept;

	static int32 Find(const CharType* str, CharType c, int32 len, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward, int32 N>
	static int32 Find(const CharType (&str)[N], CharType c) noexcept;

	template<int32 N>
	static int32 Find(const CharType (&str)[N], CharType c, ESearchCase searchCase, ESearchDir searchDir) noexcept;

	// Replace
	// Replaces all occurrences of a substring with another in-place.
	// Returns the number of replacements made.
	// * Uses a thread-local scratch buffer of SCString::LARGE_BUFFER_SIZE
	// * Result must fit within LARGE_BUFFER_SIZE or behavior is undefined
	// * Prefer the template overload when Case is known at compile time
	///////////////////////////////////////////////////////////////////////////////////

	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Replace(CharType* str, const CharType* from, const CharType* to) noexcept;

	static int32 Replace(CharType* str, const CharType* from, const CharType* to, ESearchCase searchCase) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Replace(CharType* str, const CharType* from, const CharType* to, int32 len) noexcept;

	static int32 Replace(CharType* str, const CharType* from, const CharType* to, int32 len, ESearchCase searchCase) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, int32 N>
	static int32 Replace(CharType (&str)[N], const CharType* from, const CharType* to) noexcept;

	template<int32 N>
	static int32 Replace(CharType (&str)[N], const CharType* from, const CharType* to, ESearchCase searchCase) noexcept;

	// Replace (Char)
	// Replaces all occurrences of a character with another in-place.
	// Returns the number of replacements made.
	// * When Case is Insensitive, both upper and lower forms of from are replaced with to as-is
	// * Prefer the template overload when Case is known at compile time
	///////////////////////////////////////////////////////////////////////////////////

	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Replace(CharType* str, CharType from, CharType to) noexcept;

	static int32 Replace(CharType* str, CharType from, CharType to, ESearchCase searchCase) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive>
	static int32 Replace(CharType* str, CharType from, CharType to, int32 len) noexcept;

	static int32 Replace(CharType* str, CharType from, CharType to, int32 len, ESearchCase searchCase) noexcept;

	template<ESearchCase Case = ESearchCase::Sensitive, int32 N>
	static int32 Replace(CharType (&str)[N], CharType from, CharType to) noexcept;

	template<int32 N>
	static int32 Replace(CharType (&str)[N], CharType from, CharType to, ESearchCase searchCase) noexcept;

	// Format
	// Writes a formatted string into the buffer using printf-style format string.
	// Returns the number of characters written excluding null terminator, or negative on error.
	// * Does not guarantee null termination - caller is responsible
	// * Prefer the array ref overload when buffer size is known at compile time
	///////////////////////////////////////////////////////////////////////////////////

	template<typename... ArgT>
	static int32 Format(CharType* str, const CharType* fmt, int32 len, const ArgT&... args) noexcept;

	template<int32 N, typename... ArgT>
	static int32 Format(CharType (&str)[N], const CharType* fmt, const ArgT&... args) noexcept;

	// ConvertedLength
	// Returns the number of ToCharType units required to hold the converted string, excluding null terminator.
	// Returns KOR_INDEX_NONE if len <= 0.
	// * Passing a UTF8 mid-sequence len produces undefined count
	///////////////////////////////////////////////////////////////////////////////////

	template<typename ToCharType>
	static int32 ConvertedLength(const CharType* str, int32 len) noexcept;

	template<typename ToCharType, TSize N>
	static int32 ConvertedLength(const CharType (&str)[N]) noexcept;

	// Convert
	// Converts src from CharType encoding into ToCharType encoding, writing result into toStr.
	// Returns the number of ToCharType units written, excluding null terminator.
	// Returns KOR_INDEX_NONE on error.
	// * toStr must be large enough to hold the result - use ConvertedLength to determine required size
	// * Passing a UTF8 mid-sequence len produces undefined count
	///////////////////////////////////////////////////////////////////////////////////

	template<typename ToCharType>
	static int32 Convert(const CharType* str, ToCharType* toStr, int32 len) noexcept;

	template<typename ToCharType, TSize N>
	static int32 Convert(const CharType (&str)[N], ToCharType* toStr) noexcept;

	// ToInt32 | ToInt64 | ToUInt32 | ToUInt64
	// Parses the string as an integer of the corresponding type.
	// Returns the parsed value, or 0 on failure.
	// * Does not skip leading whitespace - input must start with optional sign then digits
	// * Stops at the first character that is not a valid digit in the given base
	// * Use the outEnd overload to detect how many characters were consumed
	///////////////////////////////////////////////////////////////////////////////////

	static int32 ToInt32(const CharType* str, int32 base = 10) noexcept;
	static int32 ToInt32(const CharType* str, const CharType*& outEnd, int32 base = 10) noexcept;

	static int64 ToInt64(const CharType* str, int32 base = 10) noexcept;
	static int64 ToInt64(const CharType* str, const CharType*& outEnd, int32 base = 10) noexcept;

	static uint32 ToUInt32(const CharType* str, int32 base = 10) noexcept;
	static uint32 ToUInt32(const CharType* str, const CharType*& outEnd, int32 base = 10) noexcept;

	static uint64 ToUInt64(const CharType* str, int32 base = 10) noexcept;
	static uint64 ToUInt64(const CharType* str, const CharType*& outEnd, int32 base = 10) noexcept;

	// FromInt32 | FromInt64 | FromUInt32 | FromUInt64
	// Writes the integer value as a string into the buffer.
	// Returns the number of characters written, excluding null terminator.
	// * Use the bounded overload to guard against buffer overflows - returns KOR_INDEX_NONE if too small
	// * Prefer the array ref overload when buffer size is known at compile time
	///////////////////////////////////////////////////////////////////////////////////

	static int32 FromInt32(CharType* str, int32 value, int32 base = 10) noexcept;
	static int32 FromInt32(CharType* str, int32 value, int32 len, int32 base = 10) noexcept;

	static int32 FromInt64(CharType* str, int64 value, int32 base = 10) noexcept;
	static int32 FromInt64(CharType* str, int64 value, int32 len, int32 base = 10) noexcept;

	static int32 FromUInt32(CharType* str, uint32 value, int32 base = 10) noexcept;
	static int32 FromUInt32(CharType* str, uint32 value, int32 len, int32 base = 10) noexcept;

	static int32 FromUInt64(CharType* str, uint64 value, int32 base = 10) noexcept;
	static int32 FromUInt64(CharType* str, uint64 value, int32 len, int32 base = 10) noexcept;

	template<int32 N> static int32 FromInt32(CharType (&str)[N], int32 value, int32 base = 10) noexcept;
	template<int32 N> static int32 FromInt64(CharType (&str)[N], int64 value, int32 base = 10) noexcept;
	template<int32 N> static int32 FromUInt32(CharType (&str)[N], uint32 value, int32 base = 10) noexcept;
	template<int32 N> static int32 FromUInt64(CharType (&str)[N], uint64 value, int32 base = 10) noexcept;

	// ToFloat
	// Parses the string as a double.
	// * Delegates to strtod/wcstod - leading whitespace is skipped per CRT behavior
	// * Use the outEnd overload to detect how many characters were consumed
	///////////////////////////////////////////////////////////////////////////////////

	static double ToFloat(const CharType* str) noexcept;
	static double ToFloat(const CharType* str, const CharType*& outEnd) noexcept;

	// FromFloat
	// Writes a double value as a formatted string into the buffer.
	// Returns the number of characters written, excluding null terminator.
	// * Buffer must be at least SCString::MAX_BUFFER_SIZE_DOUBLE characters
	// * Prefer the template overload when Format is known at compile time
	// * Bounded overload returns would-be length on truncation (snprintf semantics)
	///////////////////////////////////////////////////////////////////////////////////

	template<EFloatFormat Format = EFloatFormat::Fixed>
	static int32 FromFloat(CharType* str, double value, int32 precision = 6) noexcept;

	static int32 FromFloat(CharType* str, double value, int32 precision, EFloatFormat format) noexcept;

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
