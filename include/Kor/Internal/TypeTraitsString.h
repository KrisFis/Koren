// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

KOR_NAMESPACE_BEGIN

// [Is Character Unicode]
// Character test whether type is unicode character type
// * Unicode types are: char8, char16, char32 and wchar

template<typename T>
struct TIsCharacterUnicode : TFalseType {};
#if KOR_CHAR8_NATIVE
template<> struct TIsCharacterUnicode<char8> : TTrueType {};
#endif
template<> struct TIsCharacterUnicode<char16> : TTrueType {};
template<> struct TIsCharacterUnicode<char32> : TTrueType {};
template<> struct TIsCharacterUnicode<wchar> : TTrueType {};

// [ Is Character Fixed Width ]
// Character test whether type is fixed-width type
// * Fixed-Width types enforce characters to always within width

template<typename T> struct TIsCharacterFixedWidth : TFalseType {};
template<> struct TIsCharacterFixedWidth<achar> : TTrueType {};
template<> struct TIsCharacterFixedWidth<wchar> : TTrueType {};
template<> struct TIsCharacterFixedWidth<char16> : TTrueType {};
template<> struct TIsCharacterFixedWidth<char32> : TTrueType {};

// [ Is Character Variable Width ]
// Character tests whether type is variable-width type
// * Variable-Width types allow characters to go beyond width (normally UTF8 can go up to 4 bytes)

template<typename T>
struct TIsCharacterVariableWidth
{
	static constexpr bool Value = TIsCharacter<T>::Value && !TIsCharacterFixedWidth<T>::Value;
};

// [ Is Character Compatible ]
// Character test whether source type is binary compatible with destination type
// * Non-commutative

template<typename SrcT, typename DestT> struct TIsCharacterCompatible : TFalseType { };
template<typename T> struct TIsCharacterCompatible<T, T> : TTrueType {};

#if KOR_CHAR8_NATIVE
template<> struct TIsCharacterCompatible<achar, char8> : TTrueType {};
#endif

#if KOR_WCHAR_BYTES == 4
template<> struct TIsCharacterCompatible<wchar, char32> : TTrueValue {};
template<> struct TIsCharacterCompatible<char32, wchar> : TTrueValue {};
#elif KOR_WCHAR_BYTES == 2
template<> struct TIsCharacterCompatible<wchar, char16> : TTrueType {};
template<> struct TIsCharacterCompatible<char16, wchar> : TTrueType {};
#endif

KOR_NAMESPACE_END