// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

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
// ** KOR_DECLARE_VARIADIC_ITERATOR_RET_TRAIT(FMyVarIterator, uint32, *, MyMethod)
// **
// ** template<typename T> uint32 MyMethod() { /* implementation dependent to T */ }
// ** template<typename... MyArgs> void CalculateMyMethodResults() { uint32 result = KOR_EXECUTE_VARIADIC_ITERATOR_TRAIT(FMyVarIterator, MyArgs); }
// -------------------------------------------------------------------------

#define KOR_EXECUTE_VARIADIC_ITERATOR_TRAIT(DeclareName, VarTypesName) DeclareName<sizeof...(VarTypesName)-1, VarTypesName...>::Execute()

#define KOR_DECLARE_VARIADIC_ITERATOR_RET_TRAIT(DeclareName, RetType, Operator, InMethodName)										\
	template<uint32 N, typename T, typename... A>																				\
	struct DeclareName 																											\
	{																															\
	public:																														\
																																\
		KOR_FORCEINLINE static RetType Execute() { return InMethodName<T>() Operator DeclareName<N-1, A...>::Execute(); }			\
																																\
	};																															\
																																\
	template<typename T, typename... A>																							\
	struct DeclareName<0, T, A...> 																								\
	{																															\
	public:																														\
																																\
		KOR_FORCEINLINE static RetType Execute() { return InMethodName<T>(); }														\
																																\
	};

#define KOR_DECLARE_VARIADIC_ITERATOR_TRAIT(DeclareName, InMethodName) KOR_DECLARE_VARIADIC_ITERATOR_RET_TRAIT(DeclareName, void, ;, InMethodName)