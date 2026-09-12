// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTraits.h"

namespace KOR_NAMESPACE::Internal
{
	template<typename T>
	struct TIsSharedClassType
	{
	private:
		KOR_GENERATE_HAS_METHOD_TRAIT(FGetHasSharedInitTest, IsSharedInitialized())
		KOR_GENERATE_HAS_METHOD_TRAIT(FGetHasAsSharedTest, AsShared())

	public:
		enum
		{
			HasSharedInit = FGetHasSharedInitTest<T>::Value,
			HasAsShared = FGetHasAsSharedTest<T>::Value,

			Value = HasSharedInit && HasAsShared
		};
	};
}