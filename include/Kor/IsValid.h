// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"

#include "ASTD/TypeTraits.h"
#include "ASTD/TypeTraitsMacros.h"

namespace NValidationInternals
{
	GENERATE_HAS_FIELD_TRAIT(THasValidateField, Validate)
	GENERATE_HAS_GLOBAL_METHOD_TRAIT(THasGlobalIsValid, IsValid(DeclVal<TestType>()))
	GENERATE_HAS_METHOD_TRAIT(THasIsInstanceValidMethod, IsValid())

	// Template definition for SFINAE
	template<typename T, typename Enable = void>
	struct TValidProvider;

	// Pointer and HAS valid method
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsPointer<T>::Value && THasIsInstanceValidMethod<T>::Value>::Type>
	{
		FORCEINLINE static constexpr bool Validate(T Object)
		{
			return Object && Object->IsValid();
		}
	};

	// Pointer and is missing valid method (still supported)
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsPointer<T>::Value && !THasIsInstanceValidMethod<T>::Value>::Type>
	{
		FORCEINLINE static constexpr bool Validate(T Object)
		{
			return Object != nullptr;
		}
	};

	// Reference and is HAS valid method
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsReference<T>::Value && THasIsInstanceValidMethod<T>::Value>::Type>
	{
		FORCEINLINE static constexpr bool Validate(T Object)
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

template<typename T, typename TEnableIf<NValidationInternals::TValidFinder<T>::HasBaseValid>::Type* = nullptr>
FORCEINLINE static constexpr bool IsValid(const T& obj)
{
	return NValidationInternals::TValidProvider<typename NValidationInternals::TValidFinder<T>::DesiredType>::Validate(obj);
}

template<typename T, typename TEnableIf<!NValidationInternals::TValidFinder<T>::ValidProvided>::Type* = nullptr>
FORCEINLINE static constexpr bool IsValid(...)
{
	static_assert(sizeof(T) < 0, "IsValid() function overload for type is not implemented");
	return false;
}
