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

// KOR_DEFINE_HAS_GLOBAL_METHOD_TRAIT(TraitName, MethodName)
// KOR_DEFINE_HAS_GLOBAL_METHOD_ARGS_TRAIT(TraitName, MethodName, ...)
// -------------------------------------------------------------------------
// Tests whether a free/global function call MethodName(args...) is well-formed
// Generates: template<typename T, typename ArgsT = <default args>> struct TraitName
//
// - Default args come from the macro: none, or the listed types ("T" is the trait's own parameter)
// - Override at the use site with TArgs<...> to test a different overload
// - Args are forwarded via DeclVal<Arg>(): T& = lvalue, T = rvalue, const T& = const lvalue
// - Qualified names (Kor::Begin) disable ADL; unqualified names find class-type overloads via ADL
//
// Example: KOR_DEFINE_HAS_GLOBAL_METHOD_ARGS_TRAIT(THasGlobalSwap, Swap, T&, T&);
//          THasGlobalSwap<SMyType>::Value                    // Swap(SMyType&, SMyType&)
//          THasGlobalSwap<SMyType, TArgs<SMyType&, SOther&>>::Value
//
//          KOR_DEFINE_HAS_GLOBAL_METHOD_TRAIT(THasGlobalInit, Init);
//          THasGlobalInit<void>::Value                       // Init(), T is just a tag

#define KOR_DEFINE_HAS_GLOBAL_METHOD_TRAIT(TraitName, MethodName)												\
	template<typename T, typename ArgsT = TArgs<>, typename = void> struct TraitName : TFalseValue {};			\
	template<typename T, typename... ArgsT> struct TraitName<T, TArgs<ArgsT...>,								\
		TVoid<decltype(MethodName(DeclVal<ArgsT>()...))>> : TTrueValue {};

#define KOR_DEFINE_HAS_GLOBAL_METHOD_ARGS_TRAIT(TraitName, MethodName, ...)											\
	template<typename T, typename ArgsT = TArgs<__VA_ARGS__>, typename = void> struct TraitName : TFalseValue {};	\
	template<typename T, typename... ArgsT> struct TraitName<T, TArgs<ArgsT...>,									\
		TVoid<decltype(MethodName(DeclVal<ArgsT>()...))>> : TTrueValue {};

// KOR_DEFINE_HAS_METHOD_TRAIT(TraitName, MethodName)
// KOR_DEFINE_HAS_METHOD_ARGS_TRAIT(TraitName, MethodName, ...)
// -------------------------------------------------------------------------
// Tests whether T has a member method callable as obj.MethodName(args...)
// Generates: template<typename T, typename ArgsT = <default args>> struct TraitName
//
// - Default args come from the macro: none, or the listed types ("T" is the trait's own parameter)
// - Override at the use site with TArgs<...> to test a different overload
// - Args are forwarded via DeclVal<Arg>(): T& = lvalue, T = rvalue, const T& = const lvalue
// - The object is tested as an lvalue (DeclVal<T&>): &-qualified overloads match, &&-qualified do not
// - Pass "const T" to test const-callability
//
// Example: KOR_DEFINE_HAS_METHOD_TRAIT(THasIsSharedInitialized, IsSharedInitialized);
//          THasIsSharedInitialized<SMyType>::Value           // obj.IsSharedInitialized()
//
//          KOR_DEFINE_HAS_METHOD_ARGS_TRAIT(THasSwap, Swap, T&);
//          THasSwap<SMyType>::Value                          // obj.Swap(SMyType&)
//          THasSwap<SMyType, TArgs<SOther&>>::Value          // obj.Swap(SOther&)

#define KOR_DEFINE_HAS_METHOD_TRAIT(TraitName, MethodName)													\
	template<typename T, typename ArgsT = TArgs<>, typename = void> struct TraitName : TFalseValue {};		\
	template<typename T, typename... ArgsT> struct TraitName<T, TArgs<ArgsT...>,							\
		TVoid<decltype(DeclVal<T&>().MethodName(DeclVal<ArgsT>()...))>> : TTrueValue {};

#define KOR_DEFINE_HAS_METHOD_ARGS_TRAIT(TraitName, MethodName, ...)												\
	template<typename T, typename ArgsT = TArgs<__VA_ARGS__>, typename = void> struct TraitName : TFalseValue {};	\
	template<typename T, typename... ArgsT> struct TraitName<T, TArgs<ArgsT...>,									\
		TVoid<decltype(DeclVal<T&>().MethodName(DeclVal<ArgsT>()...))>> : TTrueValue {};

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