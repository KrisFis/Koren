// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Core/Build.h"

KOR_NAMESPACE_BEGIN

// [Size]
// * Size type without need of std (equivalent of std::size_t)

typedef decltype(sizeof(0)) TSize;

// [Nullptr]
// * Nullptr type without need of std (equivalent of std::nullptr_t)

typedef decltype(nullptr) TNullptr;

// [ TVoid ]
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

// [Nth arg]
// * Gets Nth arg type from parameter pack

template<TSize N, typename T, typename... ArgsT> struct TNthArg : TType<typename TNthArg<N - 1, ArgsT...>::Type> {};
template<typename T, typename... ArgsT> struct TNthArg<0, T, ArgsT...> : TType<T> {};

template<typename T, typename... ArgsT> struct TFirstArg : TType<typename TNthArg<0, T, ArgsT...>::Type> {};
template<typename T, typename... ArgsT> struct TLastArg : TType<typename TNthArg<sizeof...(ArgsT), T, ArgsT...>::Type> {};

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

template<typename T> struct TRemoveReference : TType<T> {};
template<typename T> struct TRemoveReference<T& > : TType<T> {};
template<typename T> struct TRemoveReference<T&&> : TType<T> {};

// [Is Const]
// * Checks whether provided type is const, volatile or both

template<typename T> struct TIsConst : TFalseValue {};
template<typename T> struct TIsConst<const T> : TTrueValue {};
template<typename T> struct TIsConst<volatile T> : TTrueValue {};
template<typename T> struct TIsConst<const volatile T> : TTrueValue {};

// [Remove Const]
// * Removes "const" and "volatile" from type

template<typename T> struct TRemoveConst : TType<T> {};
template<typename T> struct TRemoveConst<const T> : TType<T> {};
template<typename T> struct TRemoveConst<volatile T> : TType<T> {};
template<typename T> struct TRemoveConst<const volatile T> : TType<T> {};

// [Remove Extent]
// * Removes extent '[]' from the type

template<typename T> struct TRemoveExtent : TType<T> {};
template<typename T, TSize N> struct TRemoveExtent<T[N]> : TType<T> {};
template<typename T> struct TRemoveExtent<T[]> : TType<T> {};

// [Is Pointer]
// * Checks whether provided type is pointer

template<typename T> struct TIsPointer : TFalseValue {};
template<typename T> struct TIsPointer<T*> : TTrueValue {};

// [Member Pointer Outer]
// * Gets member pointer outer from provided member pointer

template<typename T> struct TMemberPointer : TType<T> {};
template<typename T, typename C> struct TMemberPointer<T C::*> : TType<T> {};

// [Member Pointer Outer]
// * Gets member pointer base/outer from provided member pointer

template<typename T> struct TMemberPointerBase : TType<void> {};
template<typename T, typename C> struct TMemberPointerBase<T C::*> : TType<C> {};

// [Is Member Pointer]
// * Checks whether provided type is a member pointer
// * Member pointers are pointers to members in format 'member_type member_parent_class::*'

template<typename T> struct TIsMemberPointer : TFalseValue {};
template<typename T, typename C> struct TIsMemberPointer<T C::*> : TTrueValue {};

// [Remove Pointer]
// * Removes pointer from provided type

template<typename T> struct TRemovePointer : TType<T> {};
template<typename T> struct TRemovePointer<T*> : TType<T> {};

// [Is Array]
// * Checks whether specific type is array

template<typename T> struct TIsArray : TFalseValue {};
template<typename T> struct TIsArray<T[]> : TTrueValue {};
template<typename T, uint32 N> struct TIsArray<T[N]> : TTrueValue {};

// [Is Function]
// * Checks whether specific type is function

template<typename T> struct TIsFunction : TFalseValue {};
template<typename RetType, typename... Params> struct TIsFunction<RetType(Params...)> : TTrueValue {};

// [Is Nullptr]
// * Checks whether specific type is nullptr

template<typename T> struct TIsNullptr : TFalseValue {};
template<> struct TIsNullptr<TNullptr> : TTrueValue {};

// [Is Void]
// * Checks whether specific type is void

template<typename T> struct TIsVoid : TFalseValue {};
template<> struct TIsVoid<void> : TTrueValue {};

// [Is Bool]
// * Checks whether specific type is boolean type

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

template<typename T> struct TIsInteger : TFalseValue {};
template<> struct TIsInteger<int8> : TTrueValue {};
template<> struct TIsInteger<int16> : TTrueValue {};
template<> struct TIsInteger<int32> : TTrueValue {};
template<> struct TIsInteger<int64> : TTrueValue {};
template<> struct TIsInteger<uint8> : TTrueValue {};
template<> struct TIsInteger<uint16> : TTrueValue {};
template<> struct TIsInteger<uint32> : TTrueValue {};
template<> struct TIsInteger<uint64> : TTrueValue {};

// [Clean]
// * Removes const, volatile and reference from provided type
// * Essentially stripping all qualifiers related to template passing

template<typename T> struct TClean 
	: TType<typename TRemoveConst<typename TRemoveReference<T>::Type>::Type> 
{};

// [Is Clean]
// * Checks if type is clean type (no qualifiers)

template<typename T> struct TIsClean 
	: TBoolValue<TIsSame<typename TClean<T>::Type, T>::Value> 
{};

// [Is empty type]
// * Checks whether specific type is empty
// ** has no non-static members/fields and if so, then each is bit-fields of zero length
// ** this check is applied to each base class, if everything passes with true, then result is true

template<typename T>
struct TIsEmpty : TBoolValue<__is_empty(T)> {};

// [Is abstract type]
// * Checks whether specific type is abstract

template<typename T>
struct TIsAbstract : TBoolValue<__is_abstract(T)> {};

// [Is class type]
// * Checks whether specific type is class

template<typename T>
struct TIsClass : TBoolValue<__is_class(T)> {};

// [Is enum type]
// * Checks whether specific type is enum

template<typename T>
struct TIsEnum : TBoolValue<__is_enum(T)> {};

// [Is POD type]
// * Checks whether specific type is POD (C-lang compatible type)

template<typename T>
struct TIsPOD : TBoolValue<__is_pod(T)> {};

// [Is union type]
// * Checks whether specific type is union

template<typename T>
struct TIsUnion : TBoolValue<__is_union(T)> {};

// [Is Integral]
// * Checks whether specific type is integral

template<typename T> struct TIsIntegral 
	: TBoolValue<
		TIsInteger<T>::Value || 
		TIsCharacter<T>::Value
	>
{};

// [Is Arithmetic]
// * Checks whether specific type is arithmetic

template<typename T>
struct TIsArithmetic 
	: TBoolValue<
		TIsIntegral<T>::Value ||
		TIsFloating<T>::Value ||
		TIsBool<T>::Value
	>
{};

// [Is Fundamental]
// * Checks whether specific type is a fundamental type
// * Fundamental type is any type that is fundamental to cpp (not user-defined)

template<typename T>
struct TIsFundamental 
	: TBoolValue<
		TIsVoid<T>::Value ||
		TIsNullptr<T>::Value ||
		TIsArithmetic<T>::Value
	>
{};

// [Is Compound]
// * Checks whether specific type is a compound type
// * Compound type is any type that is NOT fundamental to cpp

template<typename T> struct TIsCompound 
	: TBoolValue<!TIsFundamental<T>> 
{};

// [Is Scalar]
// * Checks whether specific type is scalar type
// * Scalar type is any type whose value can used as single indivisible value without containing subobjects

template<typename T> struct TIsScalar
	: TBoolValue<
		TIsArithmetic<T>::Value ||
		TIsEnum<T>::Value ||
		TIsPointer<T>::Value ||
		TIsMemberPointer<T>::Value ||
		TIsNullptr<T>::Value
	>
{};

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
template<> struct TInt<1> { typedef int8 Signed; typedef uint8 Unsigned; };
template<> struct TInt<2> { typedef int16 Signed; typedef uint16 Unsigned; };
template<> struct TInt<4> { typedef int32 Signed; typedef uint32 Unsigned; };
template<> struct TInt<8> { typedef int64 Signed; typedef uint64 Unsigned; };

// [TMakeUnsigned]
// * Convenience wrapper around TInt - resolves to unsigned integer of given size
// * Example: TMakeUnsigned<int32>::Type → uint32

template<typename T>
struct TMakeUnsigned : TType<typename TInt<sizeof(T)>::Unsigned> {};

// [TMakeSigned]
// * Convenience wrapper around TInt - resolves to signed integer of given size
// * Example: TMakeSigned<uint32>::Type → int32

template<typename T>
struct TMakeSigned : TType<typename TInt<sizeof(T)>::Signed> {};

KOR_NAMESPACE_END