// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.
#pragma once

#include "Kor/Internal/TypeTraitsCore.h"
#include "Kor/Internal/TypeTraitsType.h"
#include "Kor/Internal/TypeTraitsBaseOf.h"

KOR_NAMESPACE_BEGIN

namespace Internal
{
	// [ TIsInvocableHelper ]
	// * Detects whether FunctorT is callable with ArgsT via TVoid SFINAE.
	// * Primary template is the "not invocable" fallback; specialization below
	//   only matches when the call expression is well-formed.
	template<typename, typename FunctorT, typename... ArgsT>
	struct TIsInvocableHelper : TFalseValue {};

	template<typename FunctorT, typename... ArgsT>
	struct TIsInvocableHelper
		TVoid<decltype(DeclVal<FunctorT>()(DeclVal<ArgsT>()...))>,
		FunctorT,
		ArgsT...> : TTrueValue {};

	// [ DereferenceIfNotRelated ]
	// * Returns target unchanged if TargetT is (or derives from) BaseT.
	// * Otherwise assumes target is a (possibly smart) pointer and dereferences it.
	// * Lets Invoke's member-pointer overloads accept objects and pointers uniformly.
	template<typename BaseT, typename TargetT>
	KOR_FORCEINLINE constexpr decltype(auto) DereferenceIfNotRelated(TargetT&& target)
	{
		if constexpr (TIsBaseOf<BaseT, typename TRemoveReference<TargetT>::Type>::Value)
		{
			return Forward<TargetT>(target);
		}
		else
		{
			static_assert(THasDereferenceOperator<TargetT>::Value, "Invoke call is malformed");
			return *Forward<TargetT>(target);
		}
	}
}

// [ LIFT ]
// * Wraps a named function into a callable, so it can be passed where a callable is
//   expected. Needed for overloaded functions or ones with default arguments, since
//   you can't take their address without a target type to resolve against - this
//   defers name lookup until the call, using the real argument types.
// * Example: Algo::SortByFunc(range, KOR_LIFT(toString));
#define KOR_LIFT(FuncName) \
	[](auto&&... args) -> decltype(auto) { return FuncName((decltype(args)&&)args...); }

// [ LIFT_MEMBER ]
// * Wraps a named member function of Class into a callable taking the object as its
//   first argument, so overloaded/defaulted member functions can be passed like KOR_LIFT.
// * Accepts either an object or a (possibly smart) pointer to Class as the first argument.
// * Example: Algo::SortByFunc(range, KOR_LIFT_MEMBER(UObject, GetFullName));
#define KOR_LIFT_MEMBER(Class, FuncName) \
	[](auto&& obj, auto&&... args) -> decltype(auto) \
	{ \
		return Internal::DereferenceIfNotRelated<Class>((decltype(obj)&&)obj).FuncName((decltype(args)&&)args...); \
	}

// [ Invoke ]
// * Invokes a callable, a pointer to a data member, or a pointer to a member function
//   with a set of arguments, uniformly.
// * Member overloads accept either an object reference or a (possibly smart) pointer.
template<typename FunctorT, typename... ArgsT>
KOR_FORCEINLINE constexpr auto Invoke(FunctorT&& func, ArgsT&&... args)
	-> decltype(((FunctorT&&)func)(Forward<ArgsT>(args)...))
{
	return ((FunctorT&&)func)(Forward<ArgsT>(args)...);
}

template<typename RetT, typename BaseT, typename TargetT>
KOR_FORCEINLINE constexpr auto Invoke(RetT BaseT::*memFunc, TargetT&& target)
	-> decltype(Internal::DereferenceIfNotRelated<BaseT>(Forward<TargetT>(target)).*memFunc)
{
	return Internal::DereferenceIfNotRelated<BaseT>(Forward<TargetT>(target)).*memFunc;
}

template
	typename MemberFunctorT,
	typename TargetT,
	typename... ArgsT,
	typename BaseT = typename TMemberPointerBase<MemberFunctorT>::Type
>
KOR_FORCEINLINE constexpr auto Invoke(MemberFunctorT memFunc, TargetT&& target, ArgsT&&... args)
	-> decltype((Internal::DereferenceIfNotRelated<BaseT>(Forward<TargetT>(target)).*memFunc)(Forward<ArgsT>(args)...))
{
	return (Internal::DereferenceIfNotRelated<BaseT>(Forward<TargetT>(target)).*memFunc)(Forward<ArgsT>(args)...);
}

// [ Is Invocable ]
// * True if FunctorT can be called with ArgsT.
template<typename FunctorT, typename... ArgsT>
struct TIsInvocable : Internal::TIsInvocableHelper<void, FunctorT, ArgsT...>
{};

KOR_NAMESPACE_END