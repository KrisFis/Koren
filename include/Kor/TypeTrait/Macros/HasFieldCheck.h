// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/Utility/Forward.h"

// HAS-MEMBER CHECK TRAITS (METHOD / MEMBER / TYPE / TEMPLATE)
// -------------------------------------------------------------------------
// * Each macro generates a trait struct deriving from TTrueValue or TFalseValue ("Value")
// * "Value" is true if the tested expression is well-formed for T, false otherwise
// * Detection uses the TVoid / TVoidTemplate SFINAE idiom: the specialization only matches
//   when the tested expression compiles, falling back to the primary (false) template otherwise
// * T is tested as given: cv/ref qualifiers are NOT stripped (wrap with TClean<T>::Type at
//   the call site if needed)
// * @param1 -> Name of the generated trait
// * @param2 -> Expression or member name to test (see per-macro notes below)

// KOR_DEFINE_HAS_GLOBAL_METHOD_TRAIT(TraitName, MethodCall)
// -------------------------------------------------------------------------
// Tests whether a free/global function call expression is well-formed
// "T" may be referenced inside MethodCall, which must be a complete call expression
//
// Example: KOR_DEFINE_HAS_GLOBAL_METHOD_TRAIT(THasToString, ToString(DeclVal<T>()))
//          THasToString<SMyType>::Value

#define KOR_DEFINE_HAS_GLOBAL_METHOD_TRAIT(TraitName, MethodCall)													\
	template<typename T, typename = void> struct TraitName : TFalseValue {};										\
	template<typename T> struct TraitName<T, TVoid<decltype(MethodCall)>> : TTrueValue {};

// KOR_DEFINE_HAS_METHOD_TRAIT(TraitName, MethodCall)
// -------------------------------------------------------------------------
// Tests whether T has a member method callable as MethodCall
// MethodCall is appended to DeclVal<T>(), e.g. pass "Foo()" to test T::Foo()
// Arguments are part of the test: "Foo(1)" tests T::Foo accepting an int
// T is tested as an rvalue (DeclVal<T>()), so &-qualified overloads are not matched
//
// Example: KOR_DEFINE_HAS_METHOD_TRAIT(THasIsSharedInitialized, IsSharedInitialized())
//          THasIsSharedInitialized<SMyType>::Value

#define KOR_DEFINE_HAS_METHOD_TRAIT(TraitName, MethodCall)															\
	template<typename T, typename = void> struct TraitName : TFalseValue {};										\
	template<typename T> struct TraitName<T, TVoid<decltype(DeclVal<T>().MethodCall)>> : TTrueValue {};

// KOR_DEFINE_HAS_MEMBER_TRAIT(TraitName, MemberName)
// -------------------------------------------------------------------------
// Tests whether &T::MemberName is well-formed (data member, static member or method)

// NOTE:
// Fails for overloaded methods (ambiguous address), bit-fields and members that are not accessible.
// Use KOR_DEFINE_HAS_METHOD_TRAIT for overloaded methods.
//
// Example: KOR_DEFINE_HAS_MEMBER_TRAIT(THasCount, Count)
//          THasCount<SMyType>::Value

#define KOR_DEFINE_HAS_MEMBER_TRAIT(TraitName, MemberName)															\
	template<typename T, typename = void> struct TraitName : TFalseValue {};										\
	template<typename T> struct TraitName<T, TVoid<decltype(&T::MemberName)>> : TTrueValue {};

// KOR_DEFINE_HAS_TYPE_TRAIT(TraitName, TypeName)
// -------------------------------------------------------------------------
// Tests whether T has a nested type named TypeName
//
// NOTE:
// Does not detect nested templates (a template name is not a type), use KOR_DEFINE_HAS_TEMPLATE_TRAIT for those
//
// Example: KOR_DEFINE_HAS_TYPE_TRAIT(THasUntyped, Untyped)
//          THasUntyped<SMyType>::Value

#define KOR_DEFINE_HAS_TYPE_TRAIT(TraitName, TypeName)																\
	template<typename T, typename = void> struct TraitName : TFalseValue {};										\
	template<typename T> struct TraitName<T, TVoid<typename T::TypeName>> : TTrueValue {};

// KOR_DEFINE_HAS_TEMPLATE_TRAIT(TraitName, TemplateName)
// -------------------------------------------------------------------------
// Tests whether T has a nested class/alias template named TemplateName taking type parameters
// No arguments are supplied, so constrained templates are still detected
//
// NOTE:
// Does not detect non-template nested types, and does not match templates with non-type parameters (see TVoidTemplate)
//
// Example: KOR_DEFINE_HAS_TEMPLATE_TRAIT(THasTyped, Typed)
//          THasTyped<SMyType>::Value

#define KOR_DEFINE_HAS_TEMPLATE_TRAIT(TraitName, TemplateName)														\
	template<typename T, typename = void> struct TraitName : TFalseValue {};										\
	template<typename T> struct TraitName<T, TVoidTemplate<T::template TemplateName>> : TTrueValue {};

// KOR_DEFINE_HAS_TEMPLATE_TYPE_TRAIT(TraitName, TemplateName, ...)
// -------------------------------------------------------------------------
// Tests whether T::TemplateName<...> is a valid type for the given arguments
// Implies TemplateName is a template, but reports false when the arguments are rejected (constraints, wrong arity),
// * so prefer KOR_DEFINE_HAS_TEMPLATE_TRAIT for pure existence
//
// At least one argument must be supplied (an empty __VA_ARGS__ warns pre-C++20)
//
// Example: KOR_DEFINE_HAS_TEMPLATE_TYPE_TRAIT(THasTypedInt32, Typed, int32)
//          THasTypedInt32<SMyType>::Value

#define KOR_DEFINE_HAS_TEMPLATE_TYPE_TRAIT(TraitName, TemplateName, ...)											\
	template<typename T, typename = void> struct TraitName : TFalseValue {};										\
	template<typename T> struct TraitName<T, TVoid<typename T::template TemplateName<__VA_ARGS__>>> : TTrueValue {};