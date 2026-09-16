// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Macros/HasFieldCheck.h"

KOR_NAMESPACE_BEGIN

namespace Detail
{
	KOR_DEFINE_HAS_METHOD_TRAIT(THasSharedInit, IsSharedInitialized())
	KOR_DEFINE_HAS_METHOD_TRAIT(THasAsShared, AsShared())

	template<typename T>
	struct TIsSharedClassType
	{
		enum
		{
			HasSharedInit = THasSharedInit<T>::Value,
			HasAsShared = THasAsShared<T>::Value,

			Value = HasSharedInit && HasAsShared
		};
	};
}

KOR_NAMESPACE_END