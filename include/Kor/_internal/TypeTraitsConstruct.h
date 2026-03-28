// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/_internal/TypeTraitsCore.h"

// [Is constructible]
// * Checks whether specific type is constructible from specific types

template<typename T, typename... ArgTypes>
struct TIsConstructible { enum { Value = __is_constructible(T, ArgTypes...) }; };

// [Is copy constructible]
// * Checks whether specific type is copy constructible

template<typename T>
struct TIsCopyConstructible : TIsConstructible<T, const typename TRemoveConstReference<T>::Type&> {};

// [Is move constructible]
// * Checks whether specific type is move constructible

template<typename T>
struct TIsMoveConstructible : TIsConstructible<T, typename TRemoveConstReference<T>::Type&&> {};

// Trivial type
////////////////////////////////////////////////////////////////

// [Is trivially constructible]
// * Checks whether specific type is trivially constructible from specific types

template<typename T, typename... ArgTypes>
struct TIsTriviallyConstructible { enum { Value = __is_trivially_constructible(T, ArgTypes...) }; };

// [Is trivially copy constructible]
// * Checks whether specific type can be trivially constructed from its own copy

template<typename T>
struct TIsTriviallyCopyConstructible : TIsTriviallyConstructible<T, const typename TRemoveConstReference<T>::Type&> {};

// [Is trivially move constructible]
// * Checks whether specific type can be trivially constructed from its own move

template<typename T>
struct TIsTriviallyMoveConstructible : TIsTriviallyConstructible<T, typename TRemoveConstReference<T>::Type&&> {};

// [Is trivially destructible]
// * Checks whether specific type has trivial destructor

template<typename T>
struct TIsTriviallyDestructible
{
	enum
	{
#if PLATFORM_WINDOWS || PLATFORM_APPLE
		Value = __is_trivially_destructible(T)
#elif PLATFORM_LINUX
		Value = __has_trivial_destructor(T)
#endif
	};
};

// [Is trivially assignable]

template<typename T, typename R>
struct TIsTriviallyAssignable { enum { Value = __is_trivially_assignable(T, R) };};

// [Is trivially copy assignable]
// * Checks whether specific type can be trivially assigned from its own copy

template<typename T>
struct TIsTriviallyCopyAssignable : TIsTriviallyAssignable<T, const typename TRemoveConstReference<T>::Type&> {};

// [Is trivially move assignable]
// * Checks whether specific type can be trivially assigned from its own move

template<typename T>
struct TIsTriviallyMoveAssignable : TIsTriviallyAssignable<T, typename TRemoveConstReference<T>::Type&&> {};

// [Is trivially copyable]
// * Checks whether specific type can be trivially copied

template<typename T>
struct TIsTriviallyCopyable { enum { Value = __is_trivially_copyable(T) };};

// [Is trivially movable]
// * Checks whether specific type can be trivially moved

template<typename T>
struct TIsTriviallyMovable { enum { Value = TIsTriviallyMoveConstructible<T>::Value && TIsTriviallyMoveAssignable<T>::Value };};
