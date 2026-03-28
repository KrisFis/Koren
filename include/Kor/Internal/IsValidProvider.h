// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTraits.h"
#include "Kor/TypeTraitsMacros.h"

namespace KOR_NAMESPACE Internal
{
	KOR_GENERATE_HAS_FIELD_TRAIT(THasValidateField, Validate)
	KOR_GENERATE_HAS_GLOBAL_METHOD_TRAIT(THasGlobalIsValid, IsValid(DeclVal<TestType>()))
	KOR_GENERATE_HAS_METHOD_TRAIT(THasIsInstanceValidMethod, IsValid())

	// Template definition for SFINAE
	template<typename T, typename Enable = void>
	struct TValidProvider;

	// Pointer and HAS valid method
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsPointer<T>::Value && THasIsInstanceValidMethod<T>::Value>::Type>
	{
		KOR_FORCEINLINE static constexpr bool Validate(T Object)
		{
			return Object && Object->IsValid();
		}
	};

	// Pointer and is missing valid method (still supported)
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsPointer<T>::Value && !THasIsInstanceValidMethod<T>::Value>::Type>
	{
		KOR_FORCEINLINE static constexpr bool Validate(T Object)
		{
			return Object != nullptr;
		}
	};

	// Reference and is HAS valid method
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsReference<T>::Value && THasIsInstanceValidMethod<T>::Value>::Type>
	{
		KOR_FORCEINLINE static constexpr bool Validate(T Object)
		{
			return Object.IsValid();
		}
	};

	template<typename T>
	struct TValidFinder
	{

	private:

		typedef typename TRemoveConstReference<T>::Type ClearType;

	public:

		typedef typename TChoose<
			TIsPointer<ClearType>::Value,
			typename TGetType<ClearType>::ConstPointer,
			typename TGetType<ClearType>::ConstReference
		>::Type DesiredType;

		enum {
			HasGlobalValid = THasGlobalIsValid<DesiredType>::Value,
			HasBaseValid = THasValidateField<TValidProvider<DesiredType>>::Value,

			ValidProvided = HasGlobalValid || HasBaseValid
		};
	};
}