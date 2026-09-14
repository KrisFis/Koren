// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

KOR_NAMESPACE_BEGIN

// [Member Pointer Outer]
// * Gets member pointer outer from provided member pointer

template<typename T> struct TMemberPointer : TType<T> {};
template<typename T, typename C> struct TMemberPointer<T C::*> : TType<T> {};

// [Member Pointer Base]
// * Gets member pointer base/outer from provided member pointer

template<typename T> struct TMemberPointerBase : TType<void> {};
template<typename T, typename C> struct TMemberPointerBase<T C::*> : TType<C> {};

// [Is Member Pointer]
// * Checks whether provided type is a member pointer
// * Member pointers are pointers to members in format 'member_type member_parent_class::*'

template<typename T> struct TIsMemberPointer : TFalseValue {};
template<typename T, typename C> struct TIsMemberPointer<T C::*> : TTrueValue {};

KOR_NAMESPACE_END
