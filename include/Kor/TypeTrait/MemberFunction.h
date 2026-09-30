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
//     static constexpr bool IsConst, IsVolatile, IsConstOrVolatile;
//     static constexpr bool IsLValueRef, IsRValueRef, IsRefQualified;
//     static constexpr bool IsNoexcept;
//
//     // Strip (same function type, qualifier dropped)
//     using RemoveConst, RemoveVolatile, RemoveCV, RemoveRef, RemoveNoexcept;
//     using Clean;  // cv and ref removed, noexcept kept
//     using Plain;  // everything removed
//
//     // Add (same function type, qualifier added, no-op when already present)
//     using AddConst, AddVolatile, AddNoexcept, AddLValueRef, AddRValueRef;
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

// [Member function pointer type helper]
// * Gets type of pointer to member function, without spelling out "decltype(&...)"

#define KOR_MEMBER_FUNCTION_PTR(Type, FuncName) decltype(&Type::FuncName)

// [Is Member Function Pointer]
// * Checks whether provided type is a pointer to member function of any qualifiers
// * The only trait below that is safe to use with any type, others require a member function pointer

template<typename Fn> struct TIsMemberFunctionPointer : TBoolValue<TMemberFunctionTraits<Fn>::Valid> {};

// [Is Member Function Qualified]
// * Checks qualifiers of the member function itself (not of the pointer to it)
// * Const        - "const" is present
// * Volatile     - "volatile" is present
// * ConstOrVolatile - any of "const", "volatile" or both, matches semantics of TIsConst
// * LValueRef    - "&" ref qualifier is present
// * RValueRef    - "&&" ref qualifier is present
// * Noexcept     - "noexcept" is present

template<typename Fn> struct TIsConstMemberFunction : TBoolValue<TMemberFunctionTraits<Fn>::IsConst> {};
template<typename Fn> struct TIsVolatileMemberFunction : TBoolValue<TMemberFunctionTraits<Fn>::IsVolatile> {};
template<typename Fn> struct TIsConstOrVolatileMemberFunction : TBoolValue<TMemberFunctionTraits<Fn>::IsConstOrVolatile> {};
template<typename Fn> struct TIsLValueRefMemberFunction : TBoolValue<TMemberFunctionTraits<Fn>::IsLValueRef> {};
template<typename Fn> struct TIsRValueRefMemberFunction : TBoolValue<TMemberFunctionTraits<Fn>::IsRValueRef> {};
template<typename Fn> struct TIsNoexceptMemberFunction : TBoolValue<TMemberFunctionTraits<Fn>::IsNoexcept> {};

// [Member Function Signature]
// * Retrieves parts of member function signature, qualifiers do not matter
// * Return - return type
// * Class  - class that owns the function
// * Arity  - number of arguments
// * Arg    - type of Nth argument (zero based), out of range index is a compile error

template<typename Fn> struct TMemberFunctionReturn : TType<typename TMemberFunctionTraits<Fn>::ReturnType> {};
template<typename Fn> struct TMemberFunctionClass : TType<typename TMemberFunctionTraits<Fn>::ClassType> {};

template<typename Fn> struct TMemberFunctionArity : TValue<TSize, TMemberFunctionTraits<Fn>::Arity> {};
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

template<typename Fn> struct TRemoveMemberConst : TType<typename TMemberFunctionTraits<Fn>::RemoveConst> {};
template<typename Fn> struct TRemoveMemberVolatile : TType<typename TMemberFunctionTraits<Fn>::RemoveVolatile> {};
template<typename Fn> struct TRemoveMemberCV : TType<typename TMemberFunctionTraits<Fn>::RemoveCV> {};
template<typename Fn> struct TRemoveMemberRef : TType<typename TMemberFunctionTraits<Fn>::RemoveRef> {};
template<typename Fn> struct TRemoveMemberNoexcept : TType<typename TMemberFunctionTraits<Fn>::RemoveNoexcept> {};
template<typename Fn> struct TMemberClean : TType<typename TMemberFunctionTraits<Fn>::Clean> {};
template<typename Fn> struct TMemberPlain : TType<typename TMemberFunctionTraits<Fn>::Plain> {};

// [Add Member Function Qualifiers]
// * Adds qualifier to the member function and keeps the rest of signature
// * Adding a qualifier that is already present changes nothing
// * Ref qualifiers replace each other, adding "&" to a "&&" function results in "&"
// * Const     - adds "const"
// * Volatile  - adds "volatile"
// * Noexcept  - adds "noexcept"
// * LValueRef - adds "&"
// * RValueRef - adds "&&"

template<typename Fn> struct TAddMemberConst : TType<typename TMemberFunctionTraits<Fn>::AddConst> {};
template<typename Fn> struct TAddMemberVolatile : TType<typename TMemberFunctionTraits<Fn>::AddVolatile> {};
template<typename Fn> struct TAddMemberNoexcept : TType<typename TMemberFunctionTraits<Fn>::AddNoexcept> {};
template<typename Fn> struct TAddMemberLValueRef : TType<typename TMemberFunctionTraits<Fn>::AddLValueRef> {};
template<typename Fn> struct TAddMemberRValueRef : TType<typename TMemberFunctionTraits<Fn>::AddRValueRef> {};

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

	// Signature (the case that started all this)
	static_assert(TIsSame<A::ReturnType, int*>::Value);
	static_assert(TIsSame<A::ClassType, Probe>::Value);
	static_assert(TIsSame<A::ArgType<0>, int>::Value);
	static_assert(TIsSame<A::ArgType<1>, unsigned>::Value);
	static_assert(A::Arity == 2);

	// Queries
	static_assert(!A::IsConst && A::IsNoexcept && !A::IsRefQualified);
	static_assert(F::IsConst && F::IsVolatile && F::IsConstOrVolatile);
	static_assert(F::IsLValueRef && !F::IsRValueRef && F::IsNoexcept);

	// Strip
	static_assert(TIsSame<F::RemoveConst,    int (Probe::*)(char) volatile & noexcept>::Value);
	static_assert(TIsSame<F::RemoveVolatile, int (Probe::*)(char) const & noexcept>::Value);
	static_assert(TIsSame<F::RemoveCV,       int (Probe::*)(char) & noexcept>::Value);
	static_assert(TIsSame<F::RemoveRef,      int (Probe::*)(char) const volatile noexcept>::Value);
	static_assert(TIsSame<F::RemoveNoexcept, int (Probe::*)(char) const volatile &>::Value);
	static_assert(TIsSame<F::Clean,          int (Probe::*)(char) noexcept>::Value);
	static_assert(TIsSame<F::Plain,          int (Probe::*)(char)>::Value);

	// Add (including no-op and ref replacement)
	using P = TMemberFunctionTraits<decltype(&Probe::Plain)>;
	static_assert(TIsSame<P::AddConst,    void (Probe::*)() const>::Value);
	static_assert(TIsSame<P::AddNoexcept, void (Probe::*)() noexcept>::Value);
	static_assert(TIsSame<F::AddConst,    int (Probe::*)(char) const volatile & noexcept>::Value); // no-op
	static_assert(TIsSame<F::AddRValueRef, int (Probe::*)(char) const volatile && noexcept>::Value);

	// Wrappers
	static_assert(TIsSame<TRemoveMemberCV<decltype(&Probe::Full)>::Type, int (Probe::*)(char) & noexcept>::Value);
	static_assert(TIsNoexceptMemberFunction<decltype(&Probe::Allocate)>::Value);
	static_assert(TIsSame<TMemberFunctionArg<decltype(&Probe::Allocate), 0>::Type, int>::Value);
}
#endif

KOR_NAMESPACE_END