// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/TypeTrait/Composite.h"
#include "Kor/TypeTrait/Decay.h"
#include "Kor/TypeTrait/MemberFunction.h"
#include "Kor/TypeTrait/Macros/HasFieldCheck.h"

KOR_NAMESPACE_BEGIN

// See Allocator.h for model concept

namespace Detail::Allocator
{
	KOR_DEFINE_HAS_TYPE_TRAIT(THasUntypedTypeTrait, Untyped);
	KOR_DEFINE_HAS_TEMPLATE_TRAIT(THasTypedTypeTrait, Typed);

	KOR_DEFINE_HAS_METHOD_TRAIT(THasAllocateTrait, Allocate)
	KOR_DEFINE_HAS_METHOD_TRAIT(THasReallocateTrait, Reallocate)
	KOR_DEFINE_HAS_METHOD_TRAIT(THasDeallocateTrait, Deallocate)

	KOR_DEFINE_MEMBER_FUNCTION_TRAIT(TAllocateFunctionTrait, Allocate)

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

	struct SNoType {};

	template<typename FamilyT, typename = void>
	struct TUntypedOf : TType<SNoType> {};

	template<typename FamilyT>
	struct TUntypedOf<FamilyT, TVoid<typename FamilyT::Untyped>> : TType<typename FamilyT::Untyped> {};

	template<typename FamilyT, typename ElementT, typename = void>
	struct TTypedOf : TType<SNoType> {};

	template<typename FamilyT, typename ElementT>
	struct TTypedOf<FamilyT, ElementT, TVoid<typename FamilyT::template Typed<ElementT>>>
		: TType<typename FamilyT::template Typed<ElementT>> {};

	template<typename T>
	inline constexpr bool HasAllocate =
		THasAllocateTrait<T, TArgs<SAnyInteger>>::Value ||
		THasAllocateTrait<T, TArgs<SAnyInteger, uint32>>::Value;

	template<typename T>
	inline constexpr bool HasReallocate =
		THasReallocateTrait<T, TArgs<SAnyPointer, SAnyInteger>>::Value ||
		THasReallocateTrait<T, TArgs<SAnyPointer, SAnyInteger, uint32>>::Value;

	template<typename T>
	inline constexpr bool HasDeallocate =
		THasDeallocateTrait<T, TArgs<SAnyInteger>>::Value ||
		THasDeallocateTrait<T, TArgs<SAnyInteger, uint32>>::Value;

	template<typename T>
	inline constexpr bool IsAllocator = HasAllocate<T> && HasDeallocate<T>;

	template<typename FamilyT>
	struct TIsFamily
	{
	private:
		using UntypedT = typename TUntypedOf<FamilyT>::Type;
		using TypedT = typename TTypedOf<FamilyT, uint8>::Type; // probe with a dummy element

		static constexpr bool Declared = !TIsSame<UntypedT, SNoType>::Value && !TIsSame<TypedT, SNoType>::Value;
		static constexpr bool Usable = !TIsVoid<UntypedT>::Value || !TIsVoid<TypedT>::Value;
	public:
		static constexpr bool Value = Declared && Usable;
	};

	template<typename ResolvedT, bool IsTyped>
	struct TValidateMake
	{
	private:
		static constexpr bool Missing     = TIsSame<ResolvedT, SNoType>::Value;
		static constexpr bool Unsupported = TIsSame<ResolvedT, void>::Value;
		static constexpr bool Resolved    = !Missing && !Unsupported;

		// Family doesn't declare the type at all
		static_assert(IsTyped || !Missing,
			"Allocator Family must declare `Untyped` (use `using Untyped = void` if unsupported)");
		static_assert(!IsTyped || !Missing,
			"Allocator Family must declare `template<typename T> using Typed = ...` (use `void` if unsupported)");

		// Family declares it as void
		static_assert(IsTyped || !Unsupported,
			"This allocator family does not support untyped allocation");
		static_assert(!IsTyped || !Unsupported,
			"This allocator family does not support typed allocation");

		// Declared and non-void, but doesn't look like an allocator
		static_assert(!Resolved || IsAllocator<ResolvedT>,
			"Allocator must provide Allocate(size[, alignment]) and Deallocate(ptr[, alignment])");

	public:
		using Type = ResolvedT;
	};
}

KOR_NAMESPACE_END