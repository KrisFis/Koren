// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

// METHOD/FIELD CHECK TRAIT
// * Generates a trait struct with a static constexpr bool "Value"
// * "Value" is true if the given expression is well-formed for CheckType, false otherwise
// * Detection uses the TVoid (void_t) SFINAE idiom: the specialization only matches
//   when the tested expression compiles, falling back to the primary (false) template otherwise
// * CheckType is decayed via TClean<CheckType>::Type before testing, so cv/ref-qualified
//   types are handled consistently
// * @param1 -> Name of the generated trait
// * @param2 -> Expression to test for validity (see per-macro notes below)
// *
// * KOR_GENERATE_HAS_GLOBAL_METHOD_TRAIT(DeclareName, MethodCall)
// ** Tests whether a free/global function call expression is well-formed
// ** "TestType" may be referenced inside MethodCall
// ** Example: KOR_GENERATE_HAS_GLOBAL_METHOD_TRAIT(FHasToString, ToString<TestType>)
// **          FHasToString<FMyType>::Value
// *
// * KOR_GENERATE_HAS_METHOD_TRAIT(DeclareName, MethodCall)
// ** Tests whether TestType has a member method matching MethodCall
// ** MethodCall is appended to DeclVal<TestType>(), e.g. pass "Foo()" to test TestType::Foo()
// ** Example: KOR_GENERATE_HAS_METHOD_TRAIT(FHasIsSharedInitialized, IsSharedInitialized())
// **          FHasIsSharedInitialized<FMyType>::Value
// *
// * KOR_GENERATE_HAS_FIELD_TRAIT(DeclareName, FieldName)
// ** Tests whether TestType has a member field/member named FieldName
// ** Example: KOR_GENERATE_HAS_FIELD_TRAIT(FHasCount, Count)
// **          FHasCount<FMyType>::Value
// -------------------------------------------------------------------------

// In "MethodCall" parameter "TestType" can be used
#define KOR_DEFINE_HAS_GLOBAL_METHOD_TRAIT(DeclareName, MethodCall)															\
	template <typename CheckType>																								\
	struct DeclareName																											\
	{																															\
	private:																													\
																																\
		typedef typename TClean<CheckType>::Type PureType;																		\
																																\
		template<class TestType, typename = void> struct FGetTestValue : TFalseValue {};										\
		template<class TestType> struct FGetTestValue<TestType, TVoid<decltype(MethodCall())>> : TTrueValue {};					\
																																\
	public:																														\
																																\
		static constexpr bool Value = FGetTestValue<PureType>::Value;															\
	};

// In "MethodCall" parameter "TestType" can be used
#define KOR_DEFINE_HAS_METHOD_TRAIT(DeclareName, MethodCall)																		\
	template <typename CheckType>																									\
	struct DeclareName																												\
	{																																\
	private:																														\
																																	\
		typedef typename TClean<CheckType>::Type PureType;																			\
																																	\
		template<class TestType, typename = void> struct FGetTestValue : TFalseValue {};											\
		template<class TestType> struct FGetTestValue<TestType, TVoid<decltype(DeclVal<TestType>().MethodCall)>> : TTrueValue {};	\
																																	\
	public:																															\
																																	\
		static constexpr bool Value = FGetTestValue<PureType>::Value;																\
	};

#define KOR_DEFINE_HAS_FIELD_TRAIT(DeclareName, FieldName)																		\
	template <typename CheckType>																									\
	struct DeclareName																												\
	{																																\
	private:																														\
																																	\
		typedef typename TClean<CheckType>::Type PureType;																			\
																																	\
		template<class TestType, typename = void> struct FGetTestValue : TFalseValue {};											\
		template<class TestType> struct FGetTestValue<TestType, TVoid<decltype(&TestType::FieldName)>> : TTrueValue {};				\
																																	\
	public:																															\
																																	\
		static constexpr bool Value = FGetTestValue<PureType>::Value;																\
	};
	
