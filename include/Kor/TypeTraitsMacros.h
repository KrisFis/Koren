// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"

#include "ASTD/_internal/TypeTraitsCore.h"

////////////////////////////////////////////////////////
// VARIADIC ITERATOR TRAIT
// * Iterates through var args and calls method with each type
// * Iterates from the very last var type to first var type
// * !!ONLY STATIC METHODS!! for now
// * @param1 -> Name of iterator
// * @param2 -> Return value
// * @param3 -> Operator to use between each iteration
// * @param4 -> Method name to call
// * @param... -> Values to pass to call
// * Example usage:
// **
// ** DECLARE_VARIADIC_ITERATOR_RET_TRAIT(FMyVarIterator, uint32, *, MyMethod)
// **
// ** template<typename T> uint32 MyMethod() { /* implementation dependent to T */ }
// ** template<typename... MyArgs> void CalculateMyMethodResults() { uint32 result = EXECUTE_VARIADIC_ITERATOR_TRAIT(FMyVarIterator, MyArgs); }
// **
////////////////////////////////////////////////////////

#define EXECUTE_VARIADIC_ITERATOR_TRAIT(DeclareName, VarTypesName) DeclareName<sizeof...(VarTypesName)-1, VarTypesName...>::Execute()

#define DECLARE_VARIADIC_ITERATOR_RET_TRAIT(DeclareName, RetType, Operator, InMethodName)										\
	template<uint32 N, typename T, typename... A>																				\
	struct DeclareName 																											\
	{																															\
	public:																														\
																																\
		FORCEINLINE static RetType Execute() { return InMethodName<T>() Operator DeclareName<N-1, A...>::Execute(); }			\
																																\
	};																															\
																																\
	template<typename T, typename... A>																							\
	struct DeclareName<0, T, A...> 																								\
	{																															\
	public:																														\
																																\
		FORCEINLINE static RetType Execute() { return InMethodName<T>(); }														\
																																\
	};

#define DECLARE_VARIADIC_ITERATOR_TRAIT(DeclareName, InMethodName) DECLARE_VARIADIC_ITERATOR_RET_TRAIT(DeclareName, void, ;, InMethodName)

////////////////////////////////////////////////////////
// GENERATES METHOD/FIELD CHECK TRAIT
// * TODO(kristian.fisera): MISSING COMMENT
////////////////////////////////////////////////////////

// In "MethodCall" parameter "TestType" can be used
#define GENERATE_HAS_GLOBAL_METHOD_TRAIT(DeclareName, MethodCall)																\
	template <typename CheckType>																								\
	struct DeclareName																											\
	{																															\
	private:																													\
																																\
		typedef typename TPure<CheckType>::Type PureType;																		\
																																\
		template<typename TestType> static auto TestHasMethod(int32)->TValue<decltype(MethodCall())>;							\
																																\
		template<typename> static auto TestHasMethod(int64)->TBoolValue<false>;													\
																																\
		template<class TestType> struct FGetTestValue : decltype(TestHasMethod<TestType>(0)){};									\
																																\
	public:																														\
																																\
		static constexpr bool Value = FGetTestValue<PureType>::Value;															\
	};

// In "MethodCall" parameter "TestType" can be used
#define GENERATE_HAS_METHOD_TRAIT(DeclareName, MethodCall)																		\
	template <typename CheckType>																								\
	struct DeclareName																											\
	{																															\
	private:																													\
																																\
		typedef typename TPure<CheckType>::Type PureType;																		\
																																\
		template<typename TestType> static auto TestHasMethod(int32)->TValue<decltype(DeclVal<TestType>().MethodCall)>;			\
																																\
		template<typename> static auto TestHasMethod(int64)->TBoolValue<false>;													\
																																\
		template<class TestType> struct FGetTestValue : decltype(TestHasMethod<TestType>(0)){};									\
																																\
	public:																														\
																																\
		static constexpr bool Value = FGetTestValue<PureType>::Value;															\
	};

#define GENERATE_HAS_FIELD_TRAIT(DeclareName, FieldName)																		\
	template <typename CheckType>																								\
	struct DeclareName																											\
	{																															\
	private:																													\
																																\
		typedef typename TPure<CheckType>::Type PureType;																		\
																																\
		template<typename TestType> static auto TestHasField(int32)->TValue<decltype(&TestType::FieldName)>;					\
																																\
		template<typename> static auto TestHasField(int64)->TBoolValue<false>;													\
																																\
		template<class TestType> struct FGetTestValue : decltype(TestHasField<TestType>(0)){};									\
																																\
	public:																														\
																																\
		static constexpr bool Value = FGetTestValue<PureType>::Value;															\
	};
	
