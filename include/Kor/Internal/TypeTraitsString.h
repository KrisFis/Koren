// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

KOR_NAMESPACE_BEGIN

// [Is Character Unicode]
// Character test whether type is unicode character type
// * Unicode types are: char8, char16, char32 and wchar

template<typename T>
struct TIsCharacterUnicode : TFalseValue {};
#if KOR_CHAR8_NATIVE
template<> struct TIsCharacterUnicode<char8> : TTrueValue {};
#endif
template<> struct TIsCharacterUnicode<char16> : TTrueValue {};
template<> struct TIsCharacterUnicode<char32> : TTrueValue {};
template<> struct TIsCharacterUnicode<wchar> : TTrueValue {};

// [ Is Character Fixed Width ]
// Character test whether type is fixed-width type
// * Fixed-Width types enforce characters to always within width

template<typename T> struct TIsCharacterFixedWidth : TFalseValue {};
template<> struct TIsCharacterFixedWidth<achar> : TTrueValue {};
template<> struct TIsCharacterFixedWidth<wchar> : TTrueValue {};
template<> struct TIsCharacterFixedWidth<char16> : TTrueValue {};
template<> struct TIsCharacterFixedWidth<char32> : TTrueValue {};

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

template<typename SrcT, typename DestT> struct TIsCharacterCompatible : TFalseValue { };
template<typename T> struct TIsCharacterCompatible<T, T> : TTrueValue {};

#if KOR_CHAR8_NATIVE
template<> struct TIsCharacterCompatible<achar, char8> : TTrueValue {};
#endif

#if KOR_WCHAR_BYTES == 4
template<> struct TIsCharacterCompatible<wchar, char32> : TTrueValue {};
template<> struct TIsCharacterCompatible<char32, wchar> : TTrueValue {};
#elif KOR_WCHAR_BYTES == 2
template<> struct TIsCharacterCompatible<wchar, char16> : TTrueValue {};
template<> struct TIsCharacterCompatible<char16, wchar> : TTrueValue {};
#endif

KOR_NAMESPACE_END