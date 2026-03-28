// Copyright (c) 2025 Alternity Arts. All rights reserved.

#pragma once

#include "ASTD/_internal/TypeTraitsCore.h"

// [Move]
// * Indicates that a specific object should be "moved"

template<typename T>
FORCEINLINE constexpr typename TRemoveReference<T>::Type&& Move(T& obj) noexcept { return static_cast<typename TRemoveReference<T>::Type&&>(obj); }

template<typename T>
FORCEINLINE constexpr typename TRemoveReference<T>::Type&& Move(T&& obj) noexcept { return static_cast<typename TRemoveReference<T>::Type&&>(obj); }

// [MoveIfPossible]
// * Indicates that a specific object should be "moved"
// * Does NOT enforce move if it is not possible in the context
// * Useful when we are not sure whether type is x-value or not, but still want to support move schematics

template <typename T>
FORCEINLINE constexpr typename TRemoveReference<T>::Type&& MoveIfPossible(T&& obj) noexcept { return (typename TRemoveReference<T>::Type&&)obj; }

// [Forward]
// * "Forwards" object's "passed by" type

template<typename T>
FORCEINLINE constexpr T&& Forward(typename TRemoveReference<T>::Type& obj) noexcept { return (T&&)obj; }

template<typename T>
FORCEINLINE constexpr T&& Forward(typename TRemoveReference<T>::Type&& obj) noexcept { return (T&&)obj; }

// [Decl Val]
// * Converts to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors.

template<typename T>
T&& DeclVal();

