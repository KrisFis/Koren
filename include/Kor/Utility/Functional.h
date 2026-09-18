// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Utility/Minimal.h"
#include "Kor/Utility/Forward.h"

KOR_NAMESPACE_BEGIN

#define KOR_GENERATE_COMPARISON_FUNCTOR(FuncName, Op)											\
	template<typename T>																		\
	struct FuncName																				\
	{																							\
		static_assert(																			\
			TIsConvertible<decltype(DeclVal<const T&>() Op DeclVal<const T&>()), bool>::Value,	\
			#FuncName " requires T::operator" #Op " to return something convertible to bool"	\
		);																						\
																								\
		constexpr bool operator()(const T& lhs, const T& rhs) const								\
			KOR_NOEXCEPT_EXPR(DeclVal<const T&>() Op DeclVal<const T&>())						\
		{																						\
			return lhs Op rhs;																	\
		}																						\
	};

KOR_GENERATE_COMPARISON_FUNCTOR(TLess, <)
KOR_GENERATE_COMPARISON_FUNCTOR(TGreater, >)
KOR_GENERATE_COMPARISON_FUNCTOR(TLessEqual, <=)
KOR_GENERATE_COMPARISON_FUNCTOR(TGreaterEqual, >=)
KOR_GENERATE_COMPARISON_FUNCTOR(TEqual, ==)
KOR_GENERATE_COMPARISON_FUNCTOR(TNotEqual, !=)

#undef KOR_GENERATE_COMPARISON_FUNCTOR

KOR_NAMESPACE_END