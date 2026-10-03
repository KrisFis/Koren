// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include <ranges>

#include "Kor/Memory/Minimal.h"

#include "Kor/TypeTrait/Composite.h"
#include "Kor/TypeTrait/Decay.h"
#include "Kor/TypeTrait/MemberFunction.h"
#include "Kor/TypeTrait/Macros/HasFieldCheck.h"

KOR_NAMESPACE_BEGIN

// See Allocator.h for model concept

namespace Detail::Allocator
{
	KOR_DEFINE_HAS_METHOD_TRAIT(THasAllocateTrait, Allocate);
	KOR_DEFINE_HAS_METHOD_TRAIT(THasReallocateTrait, Reallocate);
	KOR_DEFINE_HAS_METHOD_TRAIT(THasDeallocateTrait, Deallocate);

	KOR_DEFINE_HAS_METHOD_TRAIT(THasCalculateGrow, CalculateGrow)
	KOR_DEFINE_HAS_METHOD_TRAIT(THasCalculateShrink, CalculateShrink)

	KOR_DEFINE_MEMBER_FUNCTION_TRAIT(TAllocateFunctionTraits, Allocate)

	struct SAnyInteger
	{
		template<typename IntegerT, typename = TEnableIf<TIsIntegral<IntegerT>::Value>>
		operator IntegerT() const;
	};

	struct SAnyPointer
	{
		template<typename PointerT>
		operator PointerT*() const;
	};

	template<typename FamilyT, typename = void>
	struct TSizeTypeOf : TType<void> {};

	template<typename FamilyT>
	struct TSizeTypeOf<FamilyT, TVoid<typename FamilyT::SizeType>>
		: TType<typename FamilyT::SizeType> {};

	// T* Allocate(SizeT [, uint32])
	template<typename T>
	inline constexpr bool HasAllocate =
		THasAllocateTrait<T, TArgs<SAnyInteger>>::Value ||
		THasAllocateTrait<T, TArgs<SAnyInteger, uint32>>::Value;

	// T* Reallocate(T*, SizeT [, uint32])
	template<typename T>
	inline constexpr bool HasReallocate =
		THasReallocateTrait<T, TArgs<SAnyPointer, SAnyInteger>>::Value ||
		THasReallocateTrait<T, TArgs<SAnyPointer, SAnyInteger, uint32>>::Value;

	// void Deallocate(T* [, uint32])
	template<typename T>
	inline constexpr bool HasDeallocate =
		THasDeallocateTrait<T, TArgs<SAnyInteger>>::Value ||
		THasDeallocateTrait<T, TArgs<SAnyInteger, uint32>>::Value;

	// SizeT CalculateGrow(SizeT, SizeT [, uint32])
	template<typename T>
	inline constexpr bool HasCalculateGrow =
		THasCalculateGrow<T, TArgs<SAnyInteger, SAnyInteger>>::Value ||
		THasCalculateGrow<T, TArgs<SAnyInteger, SAnyInteger, uint32>>::Value;

	// SizeT CalculateShrink(SizeT, SizeT [, uint32])
	template<typename T>
	inline constexpr bool HasCalculateShrink =
		THasCalculateShrink<T, TArgs<SAnyInteger, SAnyInteger>>::Value ||
		THasCalculateShrink<T, TArgs<SAnyInteger, SAnyInteger, uint32>>::Value;

	template<typename FamilyT, typename = void>
	struct TUntypedOf : TType<void> {};

	template<typename FamilyT>
	struct TUntypedOf<FamilyT, TVoid<typename FamilyT::Untyped>> : TType<typename FamilyT::Untyped> {};

	template<typename FamilyT, typename ElementT, typename = void>
	struct TTypedOf : TType<void> {};

	template<typename FamilyT, typename ElementT>
	struct TTypedOf<FamilyT, ElementT, TVoid<typename FamilyT::template Typed<ElementT>>>
		: TType<typename FamilyT::template Typed<ElementT>> {};

	template<typename T>
	struct TIsAllocator : TBoolValue<HasAllocate<T> && HasDeallocate<T>> {};

	template<typename FamilyT>
	struct TIsFamily
	{
	private:
		using SizeTypeT = typename TSizeTypeOf<FamilyT>::Type;
		using UntypedAllocatorT = typename TUntypedOf<FamilyT>::Type;
		using TypedAllocatorT = typename TTypedOf<FamilyT, uint8>::Type; // probe with a dummy element

		static constexpr bool SizeTypeDeclared =
			!TIsVoid<SizeTypeT>::Value;

		static constexpr bool AllocatorsDeclared =
			!TIsVoid<UntypedAllocatorT>::Value ||
			!TIsVoid<TypedAllocatorT>::Value;

	public:
		static constexpr bool Value = SizeTypeDeclared && AllocatorsDeclared;
	};

	template<typename FamilyT>
	struct TSizeTypeOfValidated
	{
		using Type = TSizeTypeOf<FamilyT>::Type;
		static_assert(!TIsVoid<Type>::Value, "Allocator Family SizeType is undefined");
	};

	template<typename FamilyT>
	struct TUntypedOfValidated
	{
		using Type = TUntypedOf<FamilyT>::Type;

	private:
		static constexpr bool Supported = !TIsVoid<Type>::Value;

		static_assert(Supported,
			"Allocator Family does not support untyped allocator (FamilyT::Untyped undefined)");

		static_assert(!Supported || TIsAllocator<Type>::Value,
			"Allocator must provide Allocate(size[, alignment]) and Deallocate(ptr[, alignment])");
	};

	template<typename FamilyT, typename ElementT>
	struct TTypedOfValidated
	{
		using Type = TTypedOf<FamilyT, ElementT>::Type;

	private:
		static constexpr bool Supported = !TIsVoid<Type>::Value;

		static_assert(Supported,
			"Allocator Family does not support untyped allocator (FamilyT::Typed<T> undefined)");

		static_assert(!Supported || TIsAllocator<Type>::Value,
			"Allocator must provide Allocate(size[, alignment]) and Deallocate(ptr[, alignment])");
	};
}

KOR_NAMESPACE_END