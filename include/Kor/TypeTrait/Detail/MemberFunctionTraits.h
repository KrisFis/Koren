// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"
#include "Kor/TypeTrait/Pack.h"

KOR_NAMESPACE_BEGIN

// The matrix.
// ---------------------------------------------------------------------------
//
// X(K, V, Rf, Nx, bK, bV, bL, bR, bNx)
//   K, V, Rf, Nx : the tokens (const, volatile, & / &&, noexcept), or empty
//   bK, bV       : const / volatile flags
//   bL, bR       : lvalue-ref / rvalue-ref flags
//   bNx          : noexcept flag
//
// Token order is always: cv, ref, noexcept (the order the language requires).
// ---------------------------------------------------------------------------

#define KOR_MF_NONE

#define KOR_MF_REF(X, K, V, Rf, bK, bV, bL, bR) \
	X(K, V, Rf, KOR_MF_NONE, bK, bV, bL, bR, false) \
	X(K, V, Rf, noexcept,    bK, bV, bL, bR, true)

#define KOR_MF_CV(X, K, V, bK, bV) \
	KOR_MF_REF(X, K, V, KOR_MF_NONE, bK, bV, false, false) \
	KOR_MF_REF(X, K, V, &,           bK, bV, true,  false) \
	KOR_MF_REF(X, K, V, &&,          bK, bV, false, true)

#define KOR_MEMFN_MATRIX(X) \
	KOR_MF_CV(X, KOR_MF_NONE, KOR_MF_NONE, false, false) \
	KOR_MF_CV(X, const,       KOR_MF_NONE, true,  false) \
	KOR_MF_CV(X, KOR_MF_NONE, volatile,    false, true) \
	KOR_MF_CV(X, const,       volatile,    true,  true)

namespace Detail
{
	template<typename R, typename C, bool bConst, bool bVolatile, bool bL, bool bR, bool bNoexcept, typename... Args>
	struct TMemberFnBuilder;

#define KOR_X(K, V, Rf, Nx, bK, bV, bL, bR, bNx) \
	template<typename R, typename C, typename... Args> \
	struct TMemberFnBuilder<R, C, bK, bV, bL, bR, bNx, Args...> \
	{ using Type = R (C::*)(Args...) K V Rf Nx; };
	KOR_MEMFN_MATRIX(KOR_X)
#undef KOR_X
}

template<typename Fn>
struct TMemberFunctionTraits
{
	static_assert(TAlwaysFalse<Fn>::Value, "TMemberFunctionTraits: Fn must be a pointer to member function");
};

#define KOR_X(K, V, Rf, Nx, bK, bV, bL, bR, bNx)																\
template<typename R, typename Cls, typename... Args>															\
struct TMemberFunctionTraits<R (Cls::*)(Args...) K V Rf Nx>														\
{																												\
	using ReturnType = R;																						\
	using ClassType  = Cls;																						\
	template<TSize N> using ArgType = typename TNthArg<N, Args...>::Type;										\
	static constexpr TSize Arity = sizeof...(Args);																\
																												\
	static constexpr bool IsConst           = bK;																\
	static constexpr bool IsVolatile        = bV;																\
	static constexpr bool IsCV              = bK || bV;															\
	static constexpr bool IsLValue          = bL; 																\
	static constexpr bool IsRValue          = bR; 																\
	static constexpr bool IsRef             = bL || bR;															\
	static constexpr bool IsNoexcept        = bNx;																\
	static constexpr bool IsClean           = !(bK || bV || bL || bR);											\
	static constexpr bool IsPlain           = !(bK || bV || bL || bR || bNx);									\
																												\
	using RemoveConst    = TType<R (Cls::*)(Args...) V Rf Nx>; 													\
	using RemoveVolatile = TType<R (Cls::*)(Args...) K Rf Nx>; 													\
	using RemoveCV       = TType<R (Cls::*)(Args...) Rf Nx>;													\
	using RemoveRef      = TType<R (Cls::*)(Args...) K V Nx>; 													\
	using RemoveNoexcept = TType<R (Cls::*)(Args...) K V Rf>; 													\
	using Clean          = TType<R (Cls::*)(Args...) Nx>;														\
	using Plain          = TType<R (Cls::*)(Args...)>;															\
																												\
	using AddConst      = typename Detail::TMemberFnBuilder<R, Cls, true, bV,   bL,    bR,    bNx,  Args...>; 	\
	using AddVolatile   = typename Detail::TMemberFnBuilder<R, Cls, bK,   true, bL,    bR,    bNx,  Args...>; 	\
	using AddNoexcept   = typename Detail::TMemberFnBuilder<R, Cls, bK,   bV,   bL,    bR,    true, Args...>; 	\
	using AddLValue     = typename Detail::TMemberFnBuilder<R, Cls, bK,  bV,   true,  false, bNx,  Args...>; 	\
	using AddRValue     = typename Detail::TMemberFnBuilder<R, Cls, bK,  bV,   false, true,  bNx,  Args...>; 	\
};
KOR_MEMFN_MATRIX(KOR_X)

#undef KOR_X
#undef KOR_MEMFN_MATRIX
#undef KOR_MF_CV
#undef KOR_MF_REF
#undef KOR_MF_NONE

KOR_NAMESPACE_END