// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Core/Build.h"
#include "Kor/Internal/IsValidProvider.h"

KOR_NAMESPACE_BEGIN

template<typename T, typename TEnableIf<Internal::TValidFinder<T>::HasBaseValid>::Type* = nullptr>
KOR_FORCEINLINE static constexpr bool IsValid(const T& obj)
{
	return Internal::TValidProvider<typename Internal::TValidFinder<T>::DesiredType>::Validate(obj);
}

template<typename T, typename TEnableIf<!Internal::TValidFinder<T>::ValidProvided>::Type* = nullptr>
KOR_FORCEINLINE static constexpr bool IsValid(...)
{
	static_assert(sizeof(T) < 0, "IsValid() function overload for type is not implemented");
	return false;
}

KOR_NAMESPACE_END