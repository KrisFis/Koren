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
	struct TIsInvocableHelper<
		TVoid<decltype(DeclVal<FunctorT>()(DeclVal<ArgsT>()...))>,
		FunctorT,
		ArgsT...> : TTrueValue {};

	// [ DereferenceIfNotRelated ]
	// * Returns target unchanged if TargetT is (or derives from) BaseT.
	// * Otherwise assumes target is a (possibly smart) pointer and dereferences it.
	// * Lets Invoke's member-pointer overloads accept objects and pointers uniformly.
	template<typename BaseT, typename TargetT>
	constexpr auto DereferenceIfNotRelated(TargetT&& target)
		-> TEnableIf<TIsBaseOf<BaseT, typename TRemoveReference<TargetT>::Type>::Value, TargetT&&>::Type
	{
		return Forward<TargetT>(target);
	}

	template<typename BaseT, typename TargetT>
	constexpr auto DereferenceIfNotRelated(TargetT&& target)
		-> TEnableIf<!TIsBaseOf<BaseT, typename TRemoveReference<TargetT>::Type>::Value, decltype(*Forward<TargetT>(target))>::Type
	{
		return *Forward<TargetT>(target);
	}
}

// [ LIFT ]
// * Wraps a named function into a callable, so it can be passed where a callable is
//   expected. Needed for overloaded functions or ones with default arguments, since
//   you can't take their address without a target type to resolve against - this
//   defers name lookup until the call, using the real argument types.
#define KOR_LIFT(FuncName) \
	[](auto&&... args) -> decltype(auto) { return FuncName((decltype(args)&&)args...); }

// [ LIFT_MEMBER ]
// * Wraps a named member function of `Class` into a callable taking the object as its
//   first argument, so overloaded/defaulted member functions can be passed like KOR_LIFT.
// * Accepts either an object or a (possibly smart) pointer to `Class` as the first argument.
#define KOR_LIFT_MEMBER(Class, FuncName) \
	[](auto&& obj, auto&&... args) -> decltype(auto) \
	{ \
		return Internal::DereferenceIfNotRelated<Class>((decltype(obj)&&)obj).FuncName((decltype(args)&&)args...); \
	}

// [ Invoke ]
// Invokes a callable
// Gets pointer to a data member
// Gets pointer to a member function with a set of arguments, uniformly.
// * Member overloads accept either an object reference or a (possibly smart) pointer.
//
// Examples:
//
// Plain callable (function pointer, lambda, functor)
// * int32 Square(int32 x) { return x * x; }
// * Invoke(Square, 5);
// * Invoke(KOR_LIFT(Square), 5);
// * Invoke([](int32 x) { return x + 1; }, 5);
//
// Pointer to data member - object, raw pointer, or smart pointer
// * struct FPoint { int32 x; };
// * FPoint p{1};
// * FPoint* pp = &p;
// * Invoke(&FPoint::x, p); // -> p.x
// * Invoke(&FPoint::x, pp); // -> pp->x
//
// Pointer to member function - object, raw pointer, or smart pointer
// * struct FWidget { FString GetName() const { return name; } FString name; };
// * FWidget w{"Button"};
// * FWidget* pw = &w;
// * Invoke(&FWidget::GetName, w); // -> w.GetName()
// * Invoke(&FWidget::GetName, pw); // -> pw->GetName()
// * Invoke(KOR_LIFT(FWidget, GetName), pw); // -> pw->GetName()
//
template<typename FunctorT, typename... ArgsT>
KOR_FORCEINLINE constexpr auto Invoke(FunctorT&& func, ArgsT&&... args)
	-> decltype(Forward<FunctorT>(func)(Forward<ArgsT>(args)...))
{
	return Forward<FunctorT>(func)(Forward<ArgsT>(args)...);
}

template<typename RetT, typename BaseT, typename TargetT>
KOR_FORCEINLINE constexpr auto Invoke(RetT BaseT::*memFunc, TargetT&& target)
	-> decltype(Internal::DereferenceIfNotRelated<BaseT>(Forward<TargetT>(target)).*memFunc)
{
	return Internal::DereferenceIfNotRelated<BaseT>(Forward<TargetT>(target)).*memFunc;
}

template<
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