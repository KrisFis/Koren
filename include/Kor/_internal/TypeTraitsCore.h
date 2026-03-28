// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"

// [Size]
// * Gets size type without need of std (same as std::size_t)

typedef decltype(sizeof(0)) TSize;

// [Bool Value]
// * False and true value

template<bool T> struct TBoolValue { static constexpr bool Value = T; };
template<typename> struct TValue : TBoolValue<true> {};

typedef TBoolValue<true> TTrueValue;
typedef TBoolValue<true> TFalseValue;

// [Enable if]
// * Enables compilation of specific template function/struct when condition met

template<bool T, typename R = void> struct TEnableIf;
template<typename R> struct TEnableIf<true, R> { typedef R Type; };
template<typename R> struct TEnableIf<false, R> {};

// [Is Reference]
// * Checks whether provided type is reference

template<typename T> struct TIsReference { enum { Value = false }; };
template<typename T> struct TIsReference<T&> { enum { Value = true }; };
template<typename T> struct TIsReference<T&&> { enum { Value = true }; };

// [Is LValue]
// * Checks whether provided type is lvalue reference

template<typename T> struct TIsLValue { enum { Value = false }; };
template<typename T> struct TIsLValue<T&> { enum { Value = true }; };

// [Is RValue]
// * Checks whether provided type is rvalue reference

template<typename T> struct TIsRValue { enum { Value = false }; };
template<typename T> struct TIsRValue<T&&> { enum { Value = true }; };

// [Remove reference]
// * Removes reference from type

template <typename T> struct TRemoveReference      { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

// [Is Const]
// * Checks whether provided type is const, volatile or both

template<typename T> struct TIsConst { enum { Value = false }; };
template<typename T> struct TIsConst<const T> { enum { Value = true }; };
template<typename T> struct TIsConst<volatile T> { enum { Value = true }; };
template<typename T> struct TIsConst<const volatile T> { enum { Value = true }; };

// [Remove Const]
// * Removes "const" and "volatile" from type

template<typename T> struct TRemoveConst { typedef T Type; };
template<typename T> struct TRemoveConst<const T> { typedef T Type; };
template<typename T> struct TRemoveConst<volatile T> { typedef T Type; };
template<typename T> struct TRemoveConst<const volatile T> { typedef T Type; };

// [Remove const reference]
// * Removes const and reference from specific type

template<typename T>
struct TRemoveConstReference
{
	typedef typename TRemoveConst<typename TRemoveReference<T>::Type>::Type Type;
};

// [Is Pointer]
// * Checks whether provided type is pointer

template <typename T> struct TIsPointer { enum { Value = false }; };
template <typename T> struct TIsPointer<T*> { enum { Value = true }; };
template <typename T> struct TIsPointer<const T*> { enum { Value = true }; };

// [Remove Pointer]
// * Removes pointer from provided type

template<typename T> struct TRemovePointer { typedef T Type; };
template<typename T> struct TRemovePointer<T*> { typedef T Type; };
template<typename T> struct TRemovePointer<const T*> { typedef T Type; };

// [Is Array]
// * Checks whether specific type is array

template<typename T> struct TIsArray { enum { Value = false }; };
template<typename T> struct TIsArray<T[]> { enum { Value = true }; };
template<typename T, uint32 N> struct TIsArray<T[N]> { enum { Value = true }; };

// [Is Function]
// * Checks whether specific type is function

template<typename T> struct TIsFunction { enum { Value = false }; };
template <typename RetType, typename... Params> struct TIsFunction<RetType(Params...)> { enum { Value = true }; };

// [Is Bool Type]
// * Checks whether specific type is bool type

template<typename T> struct TIsBool { enum { Value = false }; };
template<> struct TIsBool<bool> { enum { Value = true }; };

// [Is Floating Type]
// * Checks whether specific type is floating type
// * Floating types are: float, double

template<typename T> struct TIsFloating { enum { Value = false }; };
template<> struct TIsFloating<float> { enum { Value = true }; };
template<> struct TIsFloating<double> { enum { Value = true }; };
template<> struct TIsFloating<long double> { enum { Value = true }; };

// [Is Character Type]
// * Checks whether specific type is character type
// * Character types are: char, wchar

template<typename T> struct TIsCharacter { enum { Value = false }; };
template<> struct TIsCharacter<char> { enum { Value = true }; };
template<> struct TIsCharacter<wchar> { enum { Value = true }; };

// [Is Integer Type]
// * Checks whether specific type is integer type
// * Integer types are: int8, int16, int32, int64, uint8, uint16, uint32, uint64

template<typename T> struct TIsIntegral { enum { Value = false }; };
template<> struct TIsIntegral<int8> { enum { Value = true }; };
template<> struct TIsIntegral<int16> { enum { Value = true }; };
template<> struct TIsIntegral<int32> { enum { Value = true }; };
template<> struct TIsIntegral<int64> { enum { Value = true }; };
template<> struct TIsIntegral<uint8> { enum { Value = true }; };
template<> struct TIsIntegral<uint16> { enum { Value = true }; };
template<> struct TIsIntegral<uint32> { enum { Value = true }; };
template<> struct TIsIntegral<uint64> { enum { Value = true }; };

// [Is Fundamental]
// * Checks whether specific type is a fundamental type
// * Fundamental types is any type that is fundamental to cpp (not user-defined)

template<typename T>
struct TIsFundamental
{
	enum { Value =
		TIsPointer<T>::Value ||
		TIsBool<T>::Value ||
		TIsFloating<T>::Value ||
		TIsCharacter<T>::Value ||
		TIsIntegral<T>::Value
	};
};

// [Is signed type]
// * Checks whether specific type is signed type
// * Signed types are types that is missing "unsigned" qualifier

template<typename T> struct TIsSigned { enum { Value = true }; };
template<> struct TIsSigned<uint8> { enum { Value = false }; };
template<> struct TIsSigned<uint16> { enum { Value = false }; };
template<> struct TIsSigned<uint32> { enum { Value = false }; };
template<> struct TIsSigned<uint64> { enum { Value = false }; };

// [Make Signed]
// * Gets signed type from unsigned
// * if T is already signed, it remains unchanged

template<typename T> struct TMakeSigned { typedef T Type; };
template<> struct TMakeSigned<uint8> { typedef int8 Type; };
template<> struct TMakeSigned<uint16> { typedef int16 Type; };
template<> struct TMakeSigned<uint32> { typedef int32 Type; };
template<> struct TMakeSigned<uint64> { typedef int64 Type; };

// [Make Unsigned]
// * Gets unsigned type from signed
// * if T is already unsigned, it remains unchanged

template<typename T> struct TMakeUnsigned { typedef T Type; };
template<> struct TMakeUnsigned<int8> { typedef uint8 Type; };
template<> struct TMakeUnsigned<int16> { typedef uint16 Type; };
template<> struct TMakeUnsigned<int32> { typedef uint32 Type; };
template<> struct TMakeUnsigned<int64> { typedef uint64 Type; };
