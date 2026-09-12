// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Internal/TypeTraitsCore.h"

KOR_NAMESPACE_BEGIN

// [Is constructible]
// * Checks whether specific type is constructible from specific types

template<typename T, typename... ArgTypes>
struct TIsConstructible : TBoolValue<__is_constructible(T, ArgTypes...)> {};

// [Is default constructible]
// * Checks whether specific type is default constructible

template<typename T>
struct TIsDefaultConstructible : TBoolValue<__is_constructible(T)> {};

// [Is copy constructible]
// * Checks whether specific type is copy constructible

template<typename T>
struct TIsCopyConstructible : TIsConstructible<T, const typename TClean<T>::Type&> {};

// [Is move constructible]
// * Checks whether specific type is move constructible

template<typename T>
struct TIsMoveConstructible : TIsConstructible<T, typename TClean<T>::Type&&> {};

// Trivial type
////////////////////////////////////////////////////////////////

// [Is trivially constructible]
// * Checks whether specific type has trivial constructor from specific types

template<typename T, typename... ArgTypes>
struct TIsTriviallyConstructible : TBoolValue<__is_trivially_constructible(T, ArgTypes...)> {};

// [Is trivially default constructible]
// * Checks whether specific type has trivial default constructor

template<typename T>
struct TIsTriviallyDefaultConstructible : TBoolValue<__is_trivially_constructible(T)> {};

// [Is trivially copy constructible]
// * Checks whether specific type has trivial copy constructor

template<typename T>
struct TIsTriviallyCopyConstructible : TIsTriviallyConstructible<T, const typename TClean<T>::Type&> {};

// [Is trivially move constructible]
// * Checks whether specific type has trivial move constructor

template<typename T>
struct TIsTriviallyMoveConstructible : TIsTriviallyConstructible<T, typename TClean<T>::Type&&> {};

// [Is trivially destructible]
// * Checks whether specific type has trivial destructor

template<typename T>
struct TIsTriviallyDestructible : TBoolValue<
#if KOR_COMPILER_GCC
	__has_trivial_destructor(T)
#else
	__is_trivially_destructible(T)
#endif
> {};

// [Is trivially copy assignable]
// * Checks whether specific type has trivial assignment operator

template<typename T, typename R>
struct TIsTriviallyAssignable : TBoolValue<__is_trivially_assignable(T, R)> {};

// [Is trivially copy assignable]
// * Checks whether specific type has trivial copy assignment operator

template<typename T>
struct TIsTriviallyCopyAssignable : TIsTriviallyAssignable<T, const typename TClean<T>::Type&> {};

// [Is trivially move assignable]
// * Checks whether specific type has trivial move assignment operator

template<typename T>
struct TIsTriviallyMoveAssignable : TIsTriviallyAssignable<T, typename TClean<T>::Type&&> {};

// [Is trivially copyable]
// * Checks whether specific type can be trivially copied

template<typename T>
struct TIsTriviallyCopyable : TBoolValue<__is_trivially_copyable(T)> {};

// [Is trivially movable]
// * Checks whether specific type can be trivially moved/relocated

template<typename T>
struct TIsTriviallyMovable : TBoolValue<
#if KOR_COMPILER_CLANG
	__builtin_is_cpp_trivially_relocatable(T)
#else
	TIsTriviallyDestructible<T>::Value && 
	TIsTriviallyMoveConstructible<T>::Value && 
	TIsTriviallyMoveAssignable<T>::Value
#endif
> {};

// [Is trivially relocatable]
// * Checks whether specific type can be trivially moved/relocated

template<typename T>
struct TIsTriviallyRelocatable : TIsTriviallyMovable<T> {};

KOR_NAMESPACE_END