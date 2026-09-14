// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Macros/HasFieldCheck.h"

KOR_DETAIL_NAMESPACE_BEGIN

template<typename T>
struct TIsSharedClassType
{
private:
	KOR_DEFINE_HAS_METHOD_TRAIT(FGetHasSharedInitTest, IsSharedInitialized())
	KOR_DEFINE_HAS_METHOD_TRAIT(FGetHasAsSharedTest, AsShared())

public:
	enum
	{
		HasSharedInit = FGetHasSharedInitTest<T>::Value,
		HasAsShared = FGetHasAsSharedTest<T>::Value,

		Value = HasSharedInit && HasAsShared
	};
};

KOR_DETAIL_NAMESPACE_END