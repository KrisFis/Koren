// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/TypeTraits.h"

namespace _NShared
{
	// Checks whether WeakThis field exists
	// * Thats indicator of SharedClass
	template<typename T>
	struct TIsSharedClassType
	{

	private: // Typedefs

		typedef typename TDecay<T>::Type PureType;

	private: // WeakThis test

		template<typename TestType> static auto TestHasSharedInit(int32)->TValue<decltype(DeclVal<TestType>().IsSharedInitialized())>;
		template<typename TestType>	static auto TestHasSharedInit(int64)->TBoolValue<false>;

		template<typename TestType> struct FGetHasSharedInitTest : decltype(TestHasSharedInit<TestType>(0)) {};

	private: // Private init method

		template<typename TestType> static auto TestHasAsShared(int32)->TValue<decltype(DeclVal<TestType>().AsShared())>;
		template<typename TestType>	static auto TestHasAsShared(int64)->TBoolValue<false>;

		template<typename TestType> struct FGetHasAsSharedTest : decltype(TestHasAsShared<TestType>(0)) {};

	public:

		enum
		{
			HasSharedInit = FGetHasSharedInitTest<PureType>::Value,
			HasAsShared = FGetHasAsSharedTest<PureType>::Value,

			Value = HasSharedInit && HasAsShared
		};

	};
}
