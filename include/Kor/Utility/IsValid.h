// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/CallTraits.h"

KOR_NAMESPACE_BEGIN

namespace Detail
{
	KOR_DEFINE_HAS_FIELD_TRAIT(THasValidateField, Validate)
	KOR_DEFINE_HAS_GLOBAL_METHOD_TRAIT(THasGlobalIsValid, IsValid(DeclVal<T>()))
	KOR_DEFINE_HAS_METHOD_TRAIT(THasIsInstanceValidMethod, IsValid())

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

	// Pointer, no member IsValid(), no global IsValid(obj) - falls back to null check
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsPointer<T>::Value && !THasIsInstanceValidMethod<T>::Value && !THasGlobalIsValid<T>::Value>::Type>
	{
		KOR_FORCEINLINE static constexpr bool Validate(T Object)
		{
			return Object != nullptr;
		}
	};

	// Reference and HAS valid method
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsReference<T>::Value && THasIsInstanceValidMethod<T>::Value>::Type>
	{
		KOR_FORCEINLINE static constexpr bool Validate(T Object)
		{
			return Object.IsValid();
		}
	};

	// Pointer, no member IsValid(), but has global IsValid(obj)
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsPointer<T>::Value && !THasIsInstanceValidMethod<T>::Value && THasGlobalIsValid<T>::Value>::Type>
	{
		KOR_FORCEINLINE static constexpr bool Validate(T Object)
		{
			return Object && IsValid(*Object); // or IsValid(Object), depending on desired semantics
		}
	};

	// Reference, no member IsValid(), but has global IsValid(obj)
	template<typename T>
	struct TValidProvider<T, typename TEnableIf<TIsReference<T>::Value && !THasIsInstanceValidMethod<T>::Value && THasGlobalIsValid<T>::Value>::Type>
	{
		KOR_FORCEINLINE static constexpr bool Validate(T Object)
		{
			return IsValid(Object);
		}
	};

	template<typename T>
	struct TValidFinder
	{
	private:
		typedef typename TClean<T>::Type ClearType;

	public:
		typedef typename TChoose<
			TIsPointer<ClearType>::Value,
			typename TTypeVariants<ClearType>::ConstPointer,
			typename TTypeVariants<ClearType>::ConstReference
		>::Type DesiredType;

		enum {
			HasGlobalValid = THasGlobalIsValid<DesiredType>::Value,
			HasBaseValid = THasValidateField<TValidProvider<DesiredType>>::Value,

			ValidProvided = HasGlobalValid || HasBaseValid
		};
	};
}

// [IsValid]
// * Uniform validity check dispatched at compile-time based on what T provides, in priority order:
//   - Pointer with an IsValid() method -> obj && obj->IsValid()
//   - Pointer without an IsValid() method, with a global IsValid(obj) -> obj && IsValid(*obj)
//   - Pointer without an IsValid() method, without a global IsValid(obj) -> obj != nullptr
//   - Reference with an IsValid() method -> obj.IsValid()
//   - Reference without an IsValid() method, with a global IsValid(obj) -> IsValid(obj)
// * Lets calling code write IsValid(obj) uniformly regardless of which convention T follows
// IMPORTANT: Fails to compile (via static_assert) if no compatible overload/member is found for T -
//       add an IsValid() member or a free/global IsValid(obj) overload for T to resolve
template<typename T, typename TEnableIf<Detail::TValidFinder<T>::HasBaseValid>::Type* = nullptr>
KOR_FORCEINLINE static constexpr bool IsValid(const T& obj)
{
	return Detail::TValidProvider<typename Detail::TValidFinder<T>::DesiredType>::Validate(obj);
}

template<typename T, typename TEnableIf<!Detail::TValidFinder<T>::ValidProvided>::Type* = nullptr>
KOR_FORCEINLINE static constexpr bool IsValid(...)
{
	static_assert(sizeof(T) < 0, "IsValid() function overload for type is not implemented");
	return false;
}

KOR_NAMESPACE_END