// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/TypeTrait/Minimal.h"

#include "Kor/TypeTrait/Detail/MemberFunctionTraits.h"

KOR_NAMESPACE_BEGIN

// [Member function traits layout]
// * Overview of TMemberFunctionTraits<Fn>, which lives in Detail/MemberFunctionTraits.h
// * Everything below is a thin wrapper over its members, so each function type is deduced only once
// * Specializations are emitted from one qualifier matrix (cv x ref x noexcept = 24 shapes)
// * Only pointers to non-overloaded member functions are supported (C-style variadics are not)
// * Primary template only has "Valid = false", every other member exists only when "Valid = true"
// * Strip and Add members are lazy holders, the resulting type is read through "::Type"
//
// template<typename Fn>
// struct TMemberFunctionTraits
// {
//     static constexpr bool Valid = false;
// };
//
// template<typename R, typename Cls, typename... Args>
// struct TMemberFunctionTraits<R (Cls::*)(Args...) [const] [volatile] [& or &&] [noexcept]>
// {
//     static constexpr bool Valid = true;
//
//     // Signature
//     using ReturnType = R;
//     using ClassType = Cls;
//     template<TSize N> using ArgType = /* Nth of Args */;
//     static constexpr TSize Arity = sizeof...(Args);
//
//     // Qualifier queries
//     static constexpr bool IsConst, IsVolatile, IsCV;
//     static constexpr bool IsLValue, IsRValue, IsRef;
//     static constexpr bool IsNoexcept;
//     static constexpr bool IsClean;  // no cv and no ref, noexcept is allowed
//     static constexpr bool IsPlain;  // no qualifiers at all, including noexcept
//
//     // Strip (same function type, qualifier dropped, result is ::Type)
//     using RemoveConst, RemoveVolatile, RemoveCV, RemoveRef, RemoveNoexcept;
//     using Clean;  // cv and ref removed, noexcept kept
//     using Plain;  // everything removed
//
//     // Add (same function type, qualifier added, result is ::Type, no-op when already present)
//     using AddConst, AddVolatile, AddNoexcept;
//     using AddLValue, AddRValue;  // replace the existing ref qualifier
// };

// [Define member function trait]
// * Defines alias template "DefineName<T>" that gives traits of member function "FuncName" of type T
// * Member function can not be overloaded, because address of overload set can not be taken
// * Access is checked where the macro is used, so private members need the macro inside the class (or a friend)
//
// Example:
//   KOR_DEFINE_MEMBER_FUNCTION_TRAIT(TAllocateTrait, Allocate)
//   using SizeType = typename TAllocateTrait<T>::template ArgType<0>;

#define KOR_DEFINE_MEMBER_FUNCTION_TRAIT(DefineName, FuncName) template<typename T>	using DefineName = TMemberFunctionTraits<decltype(&T::FuncName)>;

// [Define member function type]
// * Defines alias template "DefineName<T>" that gives type of pointer to member function "FuncName" of type T
// * Same restrictions as above, result can be passed to any trait that takes a member function pointer
//
// Example:
//   KOR_DEFINE_MEMBER_FUNCTION_TYPE(TAllocateFn, Allocate)
//   using SizeType = TMemberFunctionArg<TAllocateFn<T>, 0>::Type;

#define KOR_DEFINE_MEMBER_FUNCTION_TYPE(DefineName, FuncName) template<typename T> using DefineName = decltype(&T::FuncName);

// [Is Member Function Pointer]
// * Checks whether provided type is a pointer to member function of any qualifiers
// * The only trait below that is safe to use with any type, others require a member function pointer

template<typename Fn> struct TIsMemberFunctionPointer : TBoolValue<TMemberFunctionTraits<Fn>::Valid> {};

// [Is Member Function Qualified]
// * Checks qualifiers of the member function itself (not of the pointer to it)
// * Const     - "const" is present
// * Volatile  - "volatile" is present
// * CV        - any of "const", "volatile" or both, matches semantics of TIsConst
// * LValueRef - "&" ref qualifier is present
// * RValue - "&&" ref qualifier is present
// * Ref       - any of "&" or "&&"
// * Noexcept  - "noexcept" is present
// * Clean     - none of "const", "volatile", "&" or "&&" is present, "noexcept" is allowed (like TClean)
// * Plain     - no qualifiers at all, including "noexcept"

template<typename Fn> struct TIsConstMemberFunction     : TBoolValue<TMemberFunctionTraits<Fn>::IsConst> {};
template<typename Fn> struct TIsVolatileMemberFunction  : TBoolValue<TMemberFunctionTraits<Fn>::IsVolatile> {};
template<typename Fn> struct TIsCVMemberFunction        : TBoolValue<TMemberFunctionTraits<Fn>::IsCV> {};
template<typename Fn> struct TIsReferenceMemberFunction : TBoolValue<TMemberFunctionTraits<Fn>::IsRef> {};
template<typename Fn> struct TIsLValueMemberFunction    : TBoolValue<TMemberFunctionTraits<Fn>::IsLValue> {};
template<typename Fn> struct TIsRValueMemberFunction    : TBoolValue<TMemberFunctionTraits<Fn>::IsRValue> {};
template<typename Fn> struct TIsNoexceptMemberFunction  : TBoolValue<TMemberFunctionTraits<Fn>::IsNoexcept> {};
template<typename Fn> struct TIsCleanMemberFunction     : TBoolValue<TMemberFunctionTraits<Fn>::IsClean> {};
template<typename Fn> struct TIsPlainMemberFunction     : TBoolValue<TMemberFunctionTraits<Fn>::IsPlain> {};

// [Member Function Signature]
// * Retrieves parts of member function signature, qualifiers do not matter
// * Return - return type
// * Class  - class that owns the function
// * Arity  - number of arguments
// * Arg    - type of Nth argument (zero based), out of range index is a compile error

template<typename Fn> struct TMemberFunctionReturn       : TType<typename TMemberFunctionTraits<Fn>::ReturnType> {};
template<typename Fn> struct TMemberFunctionClass        : TType<typename TMemberFunctionTraits<Fn>::ClassType> {};

template<typename Fn> struct TMemberFunctionArity        : TValue<TSize, TMemberFunctionTraits<Fn>::Arity> {};
template<typename Fn, TSize N> struct TMemberFunctionArg : TType<typename TMemberFunctionTraits<Fn>::template ArgType<N>> {};

// [Remove Member Function Qualifiers]
// * Removes qualifiers from the member function and keeps the rest of signature
// * Const    - removes "const"
// * Volatile - removes "volatile"
// * CV       - removes "const" and "volatile"
// * Ref      - removes "&" or "&&"
// * Noexcept - removes "noexcept"
// * Clean    - removes "const", "volatile" and "&" or "&&", keeps "noexcept" (like TClean)
// * Plain    - removes everything

template<typename Fn> using TRemoveMemberConst        = typename TMemberFunctionTraits<Fn>::RemoveConst;
template<typename Fn> using TRemoveMemberVolatile     = typename TMemberFunctionTraits<Fn>::RemoveVolatile;
template<typename Fn> using TRemoveMemberCV           = typename TMemberFunctionTraits<Fn>::RemoveCV;
template<typename Fn> using TRemoveMemberRef          = typename TMemberFunctionTraits<Fn>::RemoveRef;
template<typename Fn> using TRemoveMemberNoexcept     = typename TMemberFunctionTraits<Fn>::RemoveNoexcept;
template<typename Fn> using TMemberClean              = typename TMemberFunctionTraits<Fn>::Clean;
template<typename Fn> using TMemberPlain              = typename TMemberFunctionTraits<Fn>::Plain;

// [Add Member Function Qualifiers]
// * Adds qualifier to the member function and keeps the rest of signature
// * Adding a qualifier that is already present changes nothing
// * Ref qualifiers replace each other, adding "&" to a "&&" function results in "&"
// * Const     - adds "const"
// * Volatile  - adds "volatile"
// * Noexcept  - adds "noexcept"
// * LValue - adds "&"
// * RValue - adds "&&"

template<typename Fn> using TAddMemberConst          = typename TMemberFunctionTraits<Fn>::AddConst;
template<typename Fn> using TAddMemberVolatile       = typename TMemberFunctionTraits<Fn>::AddVolatile;
template<typename Fn> using TAddMemberNoexcept       = typename TMemberFunctionTraits<Fn>::AddNoexcept;
template<typename Fn> using TAddMemberLValue         = typename TMemberFunctionTraits<Fn>::AddLValue;
template<typename Fn> using TAddMemberRValue         = typename TMemberFunctionTraits<Fn>::AddRValue;

#ifdef KOR_ALLOW_STATIC_TESTS
namespace Detail::MemberFnTraitsTest
{
	struct Probe
	{
		int* Allocate(int, unsigned) noexcept;
		int  Full(char) const volatile & noexcept;
		void Plain();
	};

	using A = TMemberFunctionTraits<decltype(&Probe::Allocate)>;
	using F = TMemberFunctionTraits<decltype(&Probe::Full)>;

	// Validity
	static_assert(A::Valid && F::Valid);
	static_assert(!TIsMemberFunctionPointer<int>::Value);
	static_assert(!TIsMemberFunctionPointer<int(*)(int)>::Value);

	// Signature
	static_assert(TIsSame<A::ReturnType, int*>::Value);
	static_assert(TIsSame<A::ClassType, Probe>::Value);
	static_assert(TIsSame<A::ArgType<0>, int>::Value);
	static_assert(TIsSame<A::ArgType<1>, unsigned>::Value);
	static_assert(A::Arity == 2);

	// Queries
	static_assert(!A::IsConst && A::IsNoexcept && !A::IsRef);
	static_assert(F::IsConst && F::IsVolatile && F::IsCV);
	static_assert(F::IsLValue && !F::IsRValue && F::IsNoexcept);

	// Strip
	static_assert(TIsSame<F::RemoveConst::Type,    int (Probe::*)(char) volatile & noexcept>::Value);
	static_assert(TIsSame<F::RemoveVolatile::Type, int (Probe::*)(char) const & noexcept>::Value);
	static_assert(TIsSame<F::RemoveCV::Type,       int (Probe::*)(char) & noexcept>::Value);
	static_assert(TIsSame<F::RemoveRef::Type,      int (Probe::*)(char) const volatile noexcept>::Value);
	static_assert(TIsSame<F::RemoveNoexcept::Type, int (Probe::*)(char) const volatile &>::Value);
	static_assert(TIsSame<F::Clean::Type,          int (Probe::*)(char) noexcept>::Value);
	static_assert(TIsSame<F::Plain::Type,          int (Probe::*)(char)>::Value);

	// Add
	using P = TMemberFunctionTraits<decltype(&Probe::Plain)>;
	static_assert(TIsSame<P::AddConst::Type,    void (Probe::*)() const>::Value);
	static_assert(TIsSame<P::AddNoexcept::Type, void (Probe::*)() noexcept>::Value);
	static_assert(TIsSame<F::AddConst::Type,    int (Probe::*)(char) const volatile & noexcept>::Value); // no-op
	static_assert(TIsSame<F::AddRValue::Type, int (Probe::*)(char) const volatile && noexcept>::Value);

	// Wrappers
	static_assert(TIsSame<TRemoveMemberCV<decltype(&Probe::Full)>::Type, int (Probe::*)(char) & noexcept>::Value);
	static_assert(TIsNoexceptMemberFunction<decltype(&Probe::Allocate)>::Value);
	static_assert(TIsSame<TMemberFunctionArg<decltype(&Probe::Allocate), 0>::Type, int>::Value);
}
#endif

KOR_NAMESPACE_END