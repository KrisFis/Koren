// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"

#include "Kor/Assert.h"
#include "Kor/Archive.h"
#include "Kor/Array.h"

KOR_NAMESPACE_BEGIN

template<typename CharT>
class TStringView
{
	static_assert(TIsCharacter<CharT>::Value, "CharType must be character type");

	using Constant = TCharConstant<CharT>;
	using COps = TCharOps<CharT>;
	using SOps = TStringOps<CharT>;

public:
	using CharType = CharT;

	// TODO: Replace with TArrayView once implemented
	using DataType = const CharT*;
	using SizeType = TArray<CharT, TArrayAllocator<CharT>>::SizeType;

	using IteratorType      = const CharType*;
	using ConstIteratorType = const CharType*;

	// Constructors - Default, Copy, Move
	// View is trivially copyable, no move needed.
	// -------------------------------------------------------------------------

	constexpr TStringView() noexcept;
	constexpr TStringView(const TStringView& other) noexcept = default;
	TStringView& operator=(const TStringView& other) noexcept = default;

	// Constructors - From raw pointer
	// -------------------------------------------------------------------------

	TStringView(const CharType* text) noexcept;
	TStringView(const CharType* text, SizeType length) noexcept;

	template<TSize N>
	TStringView(const CharType (&text)[N]) noexcept;

	// Comparison operators
	// Performs case-sensitive comparison.
	// Use Equals() for case-insensitive comparison.
	// -------------------------------------------------------------------------

	bool operator==(const TStringView& other) const noexcept;
	bool operator!=(const TStringView& other) const noexcept;

	// Dereference / subscript
	// operator* - Returns raw const char pointer, equivalent to GetChars().
	// operator[] - Returns character at index, no bounds checking.
	// -------------------------------------------------------------------------

	const CharType* operator*() const noexcept;
	CharType        operator[](SizeType idx) const noexcept;

	// Property getters
	// -------------------------------------------------------------------------

	// Returns true if `idx` is within [0, GetLength()).
	bool IsValidIndex(SizeType idx) const noexcept;

	// Returns true if the view has no characters.
	bool IsEmpty() const noexcept;

	// Returns the number of characters, excluding null terminator.
	SizeType GetLength() const noexcept;

	// Returns a pointer to the character buffer.
	// Note: not guaranteed to be null-terminated unless constructed from
	// a null-terminated source and not sliced.
	const CharType* GetChars() const noexcept;

	// Sub-views
	// All return non-owning views. Zero allocation.
	// Valid as long as the source buffer is alive and unmodified.
	// -------------------------------------------------------------------------

	// Returns a view of `length` characters starting at `start`.
	TStringView Sub(SizeType start, SizeType length) const noexcept;

	// Returns a view of the first `count` characters.
	TStringView Left(SizeType count) const noexcept;

	// Returns a view of the last `count` characters.
	TStringView Right(SizeType count) const noexcept;

	// Int conversions
	// -------------------------------------------------------------------------

	// Parses the viewed string as an integer. `base` defaults to 10.
	int64  ToInt(int32 base = 10) const noexcept;
	uint64 ToUInt(int32 base = 10) const noexcept;

	// Float conversions
	// -------------------------------------------------------------------------

	// Parses the viewed string as a double.
	double ToFloat() const noexcept;

	// Query
	// -------------------------------------------------------------------------

	// Returns true if all characters are in the ASCII range (0-127).
	bool IsAscii() const noexcept;

	// Returns true if all characters are numeric digits.
	bool IsNumeric() const noexcept;

	// Returns true if all characters are whitespace.
	bool IsWhitespace() const noexcept;

	// Returns true if all characters are uppercase.
	bool IsUpper() const noexcept;

	// Returns true if all characters are lowercase.
	bool IsLower() const noexcept;

	// Compares this view with `other`. Returns 0 if equal, <0 if less, >0 if greater.
	template<ESearchCase Case = ESearchCase::Sensitive>
	int32 Compare(const TStringView& other) const noexcept;
	int32 Compare(const TStringView& other, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns true if this view is equal to `other`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	bool Equals(const TStringView& other) const noexcept;
	bool Equals(const TStringView& other, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns true if this view begins with `val`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	bool StartsWith(const TStringView& val) const noexcept;
	bool StartsWith(const TStringView& val, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns true if this view ends with `val`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	bool EndsWith(const TStringView& val) const noexcept;
	bool EndsWith(const TStringView& val, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns true if this view contains `val`.
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	bool Contains(const TStringView& val) const noexcept;
	bool Contains(const TStringView& val, ESearchCase searchCase = ESearchCase::Sensitive, ESearchDir searchDir = ESearchDir::Forward) const noexcept;

	// Returns true if this view contains `val` starting from `index`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	bool ContainsAt(const TStringView& val, SizeType index) const noexcept;
	bool ContainsAt(const TStringView& val, SizeType index, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns the index of the first occurrence of `val`, or KOR_INDEX_NONE if not found.
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	SizeType Find(const TStringView& val) const noexcept;
	SizeType Find(const TStringView& val, ESearchCase searchCase = ESearchCase::Sensitive, ESearchDir searchDir = ESearchDir::Forward) const noexcept;

	// Returns the index of the first occurrence of `c`, or KOR_INDEX_NONE if not found.
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	SizeType Find(CharType c) const noexcept;
	SizeType Find(CharType c, ESearchCase searchCase = ESearchCase::Sensitive, ESearchDir searchDir = ESearchDir::Forward) const noexcept;

	// Split
	// Produces owned TStrings since views cannot guarantee lifetime of split results.
	// -------------------------------------------------------------------------

	// Splits at the first occurrence of `delim`.
	// Returns false if `delim` was not found. outLeft/outRight may be null.
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	bool Split(const TStringView& delim, TStringView* outLeft, TStringView* outRight) const noexcept;
	bool Split(const TStringView& delim, TStringView* outLeft, TStringView* outRight, ESearchCase searchCase = ESearchCase::Sensitive, ESearchDir searchDir = ESearchDir::Forward) const noexcept;

	// Splits into an array of owned substrings divided by `delim`.
	// `discardEmpty` - if true, empty strings between consecutive delimiters are skipped.
	template<ESearchCase Case = ESearchCase::Sensitive>
	TArray<TStringView> SplitToArray(const TStringView& delim, bool discardEmpty = true) const noexcept;
	TArray<TStringView> SplitToArray(const TStringView& delim, bool discardEmpty = true, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Iteration
	// -------------------------------------------------------------------------

	ConstIteratorType begin() const noexcept;
	ConstIteratorType end() const noexcept;

private:
	DataType _data = {};
	SizeType _len = 0;
};

template<typename CharT>
class TString
{
	static_assert(TIsCharacter<CharT>::Value, "CharType must be character type");

	using Constant = TCharConstant<CharT>;
	using COps = TCharOps<CharT>;
	using SOps = TStringOps<CharT>;

public:
	using CharType = CharT;
	using DataType = TArray<CharT, TArrayAllocator<CharT>>;

	using SizeType = typename DataType::SizeType;

	using IteratorType      = CharType*;
	using ConstIteratorType = const CharType*;

	// Constructors - Default, Copy, Move
	// -------------------------------------------------------------------------

	constexpr TString() noexcept;
	TString(const TString& other) noexcept = default;
	TString(TString&& other) noexcept = default;

	// Constructors - From literal
	// -------------------------------------------------------------------------

	TString(const CharType* text) noexcept;
	TString(const CharType* text, SizeType length) noexcept;

	template<TSize N>
	TString(const CharType (&text)[N]) noexcept;

	// Constructor - Fill
	// Constructs a string of `length` characters, all set to `val`.
	// -------------------------------------------------------------------------

	TString(SizeType length, CharType val = Constant::Null) noexcept;

	// Constants
	// -------------------------------------------------------------------------

	// Returns a shared empty string. Prefer over default constructing when
	// you need a const ref to an empty string, avoids allocation.
	static const TString& GetEmpty() noexcept;

	// Comparison operators
	// Performs case-sensitive comparison.
	// Use Equals() for case-insensitive comparison.
	// -------------------------------------------------------------------------

	bool operator==(const TString& other) const noexcept;
	bool operator!=(const TString& other) const noexcept;

	// Assignment operators
	// -------------------------------------------------------------------------

	TString& operator=(const TString& other) noexcept;
	TString& operator=(TString&& other) noexcept;

	// Arithmetic operators
	// operator+  - Returns a new string with `other` appended.
	// operator+= - Appends `other` in place.
	// -------------------------------------------------------------------------

	TString  operator+(const TString& other) const noexcept;
	TString  operator+(TString&& other) const noexcept;
	TString& operator+=(const TString& other) noexcept;
	TString& operator+=(TString&& other) noexcept;

	// Path operators
	// Appends `other` with a path separator in between.
	// Equivalent to appending '/' + other, handles trailing separators.
	// -------------------------------------------------------------------------

	TString  operator/(const TString& other) const noexcept;
	TString& operator/=(const TString& other) noexcept;

	// Dereference / subscript
	// operator* - Returns raw char pointer, equivalent to GetChars().
	// operator[] - Returns character at index, no bounds checking.
	// -------------------------------------------------------------------------

	const CharType* operator*() const noexcept;
	CharType*       operator*() noexcept;
	CharType        operator[](SizeType idx) const noexcept;

	// Property getters
	// -------------------------------------------------------------------------

	// Returns true if `idx` is within [0, GetLength()).
	bool IsValidIndex(SizeType idx) const noexcept;

	// Returns true if the string has no characters.
	bool IsEmpty() const noexcept;

	// Returns the number of characters, excluding null terminator.
	SizeType GetLength() const noexcept;

	// Returns the underlying TArray.
	const DataType& GetData() const noexcept;

	// Returns a pointer to the null-terminated character buffer.
	const CharType* GetChars() const noexcept;
	CharType*       GetChars() noexcept;

	// View
	// Returns a non-owning view into this string's buffer.
	// The view is valid as long as this string is alive and unmodified.
	// -------------------------------------------------------------------------

	TStringView<CharT> View() const noexcept;
	TStringView<CharT> SubView(SizeType start, SizeType length) const noexcept;
	TStringView<CharT> LeftView(SizeType count) const noexcept;
	TStringView<CharT> RightView(SizeType count) const noexcept;

	// Encoding / CharType conversions
	// -------------------------------------------------------------------------

	// Constructs a TString<CharT> by converting from a different char type.
	template<typename OtherCharType>
	static TString ConvertFrom(const OtherCharType* str) noexcept;

	template<typename OtherCharType>
	static TString ConvertFrom(const OtherCharType* str, SizeType length) noexcept;

	template<typename OtherCharType, TSize N>
	static TString ConvertFrom(const OtherCharType (&str)[N]) noexcept;

	template<typename OtherCharType>
	static TString ConvertFrom(const TString<OtherCharType>& str) noexcept;

	// Converts this string to a different char type.
	template<typename OtherCharType>
	TString<OtherCharType> ConvertTo() const noexcept;

	// Int conversions
	// -------------------------------------------------------------------------

	static TString FromInt(int64 val) noexcept;
	static TString FromUInt(uint64 val) noexcept;

	// Parses the string as an integer. `base` defaults to 10.
	int64  ToInt(int32 base = 10) const noexcept;
	uint64 ToUInt(int32 base = 10) const noexcept;

	// Float conversions
	// -------------------------------------------------------------------------

	// Converts `val` to string. `precision` controls decimal places.
	// `Format` controls notation: Auto, Fixed, Scientific.
	template<EFloatFormat Format = EFloatFormat::Auto>
	static TString FromFloat(double val, uint8 precision = 6) noexcept;
	static TString FromFloat(double val, uint8 precision = 6, EFloatFormat format = EFloatFormat::Auto) noexcept;

	// Parses the string as a double.
	double ToFloat() const noexcept;

	// Append
	// -------------------------------------------------------------------------

	void Append(const TString& other) noexcept;
	void Append(TString&& other) noexcept;
	void Append(const CharType* other, SizeType num) noexcept;

	// Formatting
	// -------------------------------------------------------------------------

	// Returns a new formatted string. Uses the same format syntax as printf.
	template<typename FmtT, typename... VarTypes>
	static TString Format(const FmtT& fmt, const VarTypes&... args) noexcept;

	// Appends a formatted string in place.
	template<typename StringT, typename... ArgTypes>
	void AppendFormat(const StringT& fmt, const ArgTypes&... args) noexcept;

	// Query
	// These functions do not modify the string.
	// -------------------------------------------------------------------------

	// Returns true if all characters are in the ASCII range (0-127).
	bool IsAscii() const noexcept;

	// Returns true if all characters are numeric digits.
	bool IsNumeric() const noexcept;

	// Returns true if all characters are whitespace.
	bool IsWhitespace() const noexcept;

	// Returns true if all characters are uppercase.
	bool IsUpper() const noexcept;

	// Returns true if all characters are lowercase.
	bool IsLower() const noexcept;

	// Compares this string with `other`. Returns 0 if equal, <0 if less, >0 if greater.
	template<ESearchCase Case = ESearchCase::Sensitive>
	int32 Compare(const TString& other) const noexcept;
	int32 Compare(const TString& other, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns true if this string is equal to `other`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	bool Equals(const TString& other) const noexcept;
	bool Equals(const TString& other, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns true if this string begins with `val`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	bool StartsWith(const TString& val) const noexcept;
	bool StartsWith(const TString& val, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns true if this string ends with `val`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	bool EndsWith(const TString& val) const noexcept;
	bool EndsWith(const TString& val, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns true if this string contains `val`.
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	bool Contains(const TString& val) const noexcept;
	bool Contains(const TString& val, ESearchCase searchCase = ESearchCase::Sensitive, ESearchDir searchDir = ESearchDir::Forward) const noexcept;

	// Returns true if this string contains `val` starting from `index`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	bool ContainsAt(const TString& val, SizeType index) const noexcept;
	bool ContainsAt(const TString& val, SizeType index, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Returns the index of the first occurrence of `val`, or KOR_INDEX_NONE if not found.
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	SizeType Find(const TString& val) const noexcept;
	SizeType Find(const TString& val, ESearchCase searchCase = ESearchCase::Sensitive, ESearchDir searchDir = ESearchDir::Forward) const noexcept;

	// Returns the index of the first occurrence of `c`, or KOR_INDEX_NONE if not found.
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	SizeType Find(CharType c) const noexcept;
	SizeType Find(CharType c, ESearchCase searchCase = ESearchCase::Sensitive, ESearchDir searchDir = ESearchDir::Forward) const noexcept;

	// Substring
	// Returns a new owned string. For zero-allocation alternatives see View, SubView, LeftView, RightView.
	// -------------------------------------------------------------------------

	// Returns a substring starting at `start` of `length` characters.
	TString Sub(SizeType start, SizeType length) const noexcept;

	// Returns the first `count` characters.
	TString Left(SizeType count) const noexcept;

	// Returns the last `count` characters.
	TString Right(SizeType count) const noexcept;

	// Mutation
	// These functions modify the string in place.
	// -------------------------------------------------------------------------

	// Converts all characters to uppercase.
	void ToUpper() noexcept;

	// Converts all characters to lowercase.
	void ToLower() noexcept;

	// Removes leading whitespace.
	void TrimStart() noexcept;

	// Removes trailing whitespace.
	void TrimEnd() noexcept;

	// Removes leading and trailing whitespace.
	void Trim() noexcept;

	// Removes all characters from `idx` to the end.
	void ChopRight(SizeType idx) noexcept;

	// Removes all characters from the start up to `idx`.
	void ChopLeft(SizeType idx) noexcept;

	// Removes all characters in the range [firstIdx, secondIdx].
	void ChopRange(SizeType firstIdx, SizeType secondIdx) noexcept;

	// Inserts `other` at position `idx`, shifting existing characters right.
	void Insert(SizeType idx, const TString& other) noexcept;

	// Removes `count` characters starting at `idx`.
	void Remove(SizeType idx, SizeType count) noexcept;

	// Splits the string at the first occurrence of `delim`.
	// Returns false if `delim` was not found. outLeft/outRight may be null.
	template<ESearchCase Case = ESearchCase::Sensitive, ESearchDir Dir = ESearchDir::Forward>
	bool Split(const TString& delim, TString* outLeft, TString* outRight) const noexcept;
	bool Split(const TString& delim, TString* outLeft, TString* outRight, ESearchCase searchCase = ESearchCase::Sensitive, ESearchDir searchDir = ESearchDir::Forward) const noexcept;

	// Splits the string into an array of substrings divided by `delim`.
	// `discardEmpty` - if true, empty strings between consecutive delimiters are skipped.
	template<ESearchCase Case = ESearchCase::Sensitive>
	TArray<TString> SplitToArray(const TString& delim, bool discardEmpty = true) const noexcept;
	TArray<TString> SplitToArray(const TString& delim, bool discardEmpty = true, ESearchCase searchCase = ESearchCase::Sensitive) const noexcept;

	// Replaces all occurrences of `from` with `to`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	void Replace(const TString& from, const TString& to) noexcept;
	void Replace(const TString& from, const TString& to, ESearchCase searchCase = ESearchCase::Sensitive) noexcept;

	// Replaces all occurrences of character `from` with character `to`.
	template<ESearchCase Case = ESearchCase::Sensitive>
	void Replace(CharType from, CharType to) noexcept;
	void Replace(CharType from, CharType to, ESearchCase searchCase = ESearchCase::Sensitive) noexcept;

	// Lifecycle & Memory
	// -------------------------------------------------------------------------

	// Returns a deep copy of this string.
	TString Copy() const noexcept;

	// Resets the string to `length` characters all set to `val`.
	void Fill(SizeType length, CharType val = Constant::Null) noexcept;

	// Clears the string and releases memory.
	void Reset() noexcept;

	// Clears the string but retains allocated memory.
	void Empty() noexcept;

	// Pre-allocates memory for at least `num` characters.
	void Reserve(SizeType num) noexcept;

	// Releases any excess allocated memory to fit the current length.
	void ShrinkToFit() noexcept;

	// Iteration
	// -------------------------------------------------------------------------

	IteratorType      begin() noexcept;
	ConstIteratorType begin() const noexcept;
	IteratorType      end() noexcept;
	ConstIteratorType end() const noexcept;

private:
	DataType _data = {};
};

// [ Is TString ]
// Checks if type is TString type

template<typename T> struct TIsTString : TFalseType {};
template<typename CharT> struct TIsTString<TString<CharT>> : TTrueType {};

// [ Is TString View ]
// Checks if type is TStringView type

template<typename T> struct TIsTStringView : TFalseType {};
template<typename CharT> struct TIsTStringView<TStringView<CharT>> : TTrueType {};

#include "Kor/Inl/String.inl"

// Convenience typedef for platform char
using SStringView = TStringView<tchar>;
using SString = TString<tchar>;

KOR_NAMESPACE_END