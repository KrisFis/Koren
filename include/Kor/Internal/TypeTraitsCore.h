// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

KOR_NAMESPACE_BEGIN

// [Size]
// * Gets size type without need of std (same as std::size_t)

typedef decltype(sizeof(0)) TSize;

// [[ TVoid ]]
// * Void type

template<typename...>
using TVoid = void;

// [Type]
// * Defines "Type" as provided type

template<typename T> struct TType { using Type = T; };

// [Bool Value]
// * Defines "Value" from provided const bool

template<bool T> struct TBoolValue { static constexpr bool Value = T; };
template<typename> struct TValue : TBoolValue<true> {};

typedef TBoolValue<true> TTrueValue;
typedef TBoolValue<false> TFalseValue;

// [Is Same]
// * Checks whether specified types are the same

template<typename T, typename R> struct TIsSame : TFalseValue {};
template<typename T> struct TIsSame<T, T> : TTrueValue {};

// [Get Nth type]
// * Gets Nth type from parameter pack

template<TSize N, typename T, typename... ArgTypes> struct TGetNthType : TType<typename TGetNthType<N - 1, ArgTypes...>::Type> {};
template<typename T, typename... ArgTypes> struct TGetNthType<0, T, ArgTypes...> : TType<T> {};

// [Enable if]
// * Enables compilation of specific template function/struct when condition met

template<bool T, typename R = void> struct TEnableIf;
template<typename R> struct TEnableIf<true, R> : TType<R> {};
template<typename R> struct TEnableIf<false, R> {};

// [Is Reference]
// * Checks whether provided type is reference

template<typename T> struct TIsReference : TFalseValue {};
template<typename T> struct TIsReference<T&> : TTrueValue {};
template<typename T> struct TIsReference<T&&> : TTrueValue {};

// [Is LValue]
// * Checks whether provided type is lvalue reference

template<typename T> struct TIsLValue : TFalseValue {};
template<typename T> struct TIsLValue<T&> : TTrueValue {};

// [Is RValue]
// * Checks whether provided type is rvalue reference

template<typename T> struct TIsRValue : TFalseValue {};
template<typename T> struct TIsRValue<T&&> : TTrueValue {};

// [Remove reference]
// * Removes reference from type

template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

// [Is Const]
// * Checks whether provided type is const, volatile or both

template<typename T> struct TIsConst : TFalseValue {};
template<typename T> struct TIsConst<const T> : TTrueValue {};
template<typename T> struct TIsConst<volatile T> : TTrueValue {};
template<typename T> struct TIsConst<const volatile T> : TTrueValue {};

// [Remove Const]
// * Removes "const" and "volatile" from type

template<typename T> struct TRemoveConst { typedef T Type; };
template<typename T> struct TRemoveConst<const T> { typedef T Type; };
template<typename T> struct TRemoveConst<volatile T> { typedef T Type; };
template<typename T> struct TRemoveConst<const volatile T> { typedef T Type; };

// [Remove const reference]
// * Removes const and reference from specific type

template<typename T>
struct TRemoveConstReference : TType<typename TRemoveConst<typename TRemoveReference<T>::Type>::Type> {};

// [Remove Extent]
// * Removes extent '[]' from the type

template<typename T> struct TRemoveExtent : TType<T> {};
template<typename T, TSize N> struct TRemoveExtent<T[N]> : TType<T> {};
template<typename T> struct TRemoveExtent<T[]> { typedef T Type; };

// [Is Pointer]
// * Checks whether provided type is pointer

template <typename T> struct TIsPointer : TFalseValue {};
template <typename T> struct TIsPointer<T*> : TTrueValue {};
template <typename T> struct TIsPointer<const T*> : TTrueValue {};

// [Remove Pointer]
// * Removes pointer from provided type

template<typename T> struct TRemovePointer { typedef T Type; };
template<typename T> struct TRemovePointer<T*> { typedef T Type; };
template<typename T> struct TRemovePointer<const T*> { typedef T Type; };

// [Is Array]
// * Checks whether specific type is array

template<typename T> struct TIsArray : TFalseValue {};
template<typename T> struct TIsArray<T[]> : TTrueValue {};
template<typename T, uint32 N> struct TIsArray<T[N]> : TTrueValue {};

// [Is Function]
// * Checks whether specific type is function

template<typename T> struct TIsFunction : TFalseValue {};
template <typename RetType, typename... Params> struct TIsFunction<RetType(Params...)> : TTrueValue {};

// [Is Bool]
// * Checks whether specific type is bool type

template<typename T> struct TIsBool : TFalseValue {};
template<> struct TIsBool<bool> : TTrueValue {};

// [Is Floating]
// * Checks whether specific type is floating type
// * Floating types are: float, double

template<typename T> struct TIsFloating : TFalseValue {};
template<> struct TIsFloating<float> : TTrueValue {};
template<> struct TIsFloating<double> : TTrueValue {};
template<> struct TIsFloating<long double> : TTrueValue {};

// [Is Character]
// * Checks whether specific type is character type
// * Character types are: char, wchar

template<typename T> struct TIsCharacter : TFalseValue {};
template<> struct TIsCharacter<achar> : TTrueValue {};
#if KOR_CHAR8_NATIVE
template<> struct TIsCharacter<char8> : TTrueValue {};
#endif
template<> struct TIsCharacter<wchar> : TTrueValue {};
template<> struct TIsCharacter<char16> : TTrueValue {};
template<> struct TIsCharacter<char32> : TTrueValue {};

// [Is Integer]
// * Checks whether specific type is integer type
// * Integer types are: int8, int16, int32, int64, uint8, uint16, uint32, uint64

template<typename T> struct TIsIntegral : TFalseValue {};
template<> struct TIsIntegral<int8> : TTrueValue {};
template<> struct TIsIntegral<int16> : TTrueValue {};
template<> struct TIsIntegral<int32> : TTrueValue {};
template<> struct TIsIntegral<int64> : TTrueValue {};
template<> struct TIsIntegral<uint8> : TTrueValue {};
template<> struct TIsIntegral<uint16> : TTrueValue {};
template<> struct TIsIntegral<uint32> : TTrueValue {};
template<> struct TIsIntegral<uint64> : TTrueValue {};

// [Is Arithmetic]
// * Checks whether specific type is arithmetic

template <typename T>
struct TIsArithmetic
{
	enum { Value =
			TIsIntegral<T>::Value ||
			TIsFloating<T>::Value ||
			TIsCharacter<T>::Value ||
			TIsBool<T>::Value
	};
};

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

template<typename T> struct TIsSigned
{
	static_assert(TIsArithmetic<T>::Value, "TIsSigned is only valid for arithmetic types");
	enum { Value = T(-1) < T(0) };
};

// [Is unsigned type]
// * Checks whether specific type is unsigned type

template<typename T> struct TIsUnsigned
{
	static_assert(TIsArithmetic<T>::Value, "TIsUnsigned is only valid for arithmetic types");
	enum { Value = !TIsSigned<T>::Value };
};

// [TInt]
// * Maps a byte size to its corresponding signed and unsigned integer types
// * Primary source of truth for integer type resolution by size
// * Unsupported sizes will result in a compile error (incomplete type)
template<TSize Size> struct TInt { static_assert(Size == 0, "TInt: unsupported size, must be 1, 2, 4 or 8"); };
template <> struct TInt<1> { typedef int8 Signed; typedef uint8 Unsigned; };
template <> struct TInt<2> { typedef int16 Signed; typedef uint16 Unsigned; };
template <> struct TInt<4> { typedef int32 Signed; typedef uint32 Unsigned; };
template <> struct TInt<8> { typedef int64 Signed; typedef uint64 Unsigned; };

// [TMakeUnsigned]
// * Convenience wrapper around TInt - resolves to unsigned integer of given size
// * Example: TMakeUnsigned<int32>::Type → uint32
template <typename T>
struct TMakeUnsigned { typedef typename TInt<sizeof(T)>::Unsigned Type; };

// [TMakeSigned]
// * Convenience wrapper around TInt - resolves to signed integer of given size
// * Example: TMakeSigned<uint32>::Type → int32
template <typename T>
struct TMakeSigned { typedef typename TInt<sizeof(T)>::Signed Type; };

KOR_NAMESPACE_END