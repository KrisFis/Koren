// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Container/Minimal.h"
#include "Kor/Container/ContainerTraits.h"

#include "Kor/TypeTrait/Category.h"
#include "Kor/Memory/MemoryOps.h"
#include "Kor/Utility/Invoke.h"
#include "Kor/Utility/Swap.h"

KOR_NAMESPACE_BEGIN

// Forward declare for private array view friend
namespace Detail::ArrayView { template<typename T> struct TFriend; }

// [ TArrayView ]
// A non-owning view over a contiguous sequence of elements.
// * ElementT must be non-void and non-reference (cv-qualification is allowed, e.g. const T)
// * SizeT must be a signed integer type
// * The view never allocates, copies, or frees elements; the viewed memory must outlive the view
// * Copying a view copies only the pointer and size (shallow); both views alias the same data
// * Use TConstArrayView<T> for a read-only view (alias for TArrayView<const T>)
// * Constructing from an initializer list does not extend its lifetime; don't keep such a view
//   beyond the full expression that created the initializer list
//
// Example:
//   TArray<int32> values = { 1, 2, 3 };
//   TArrayView<int32, int32> view(values.GetData(), values.GetNum());
//   for (int32 v : view) { ... }
//
// TODO: Support generic containers via constructor/comparison operators
//
template<typename ElementT, typename SizeT>
class TArrayView
{
public:
	static_assert(!TIsVoid<ElementT>::Value, "ElementType must be non-void type");
	static_assert(!TIsReference<ElementT>::Value, "ElementType must not be a reference type");

	using ElementType = ElementT;
	using SizeType = SizeT;

	using ILType = std::initializer_list<typename TRemoveConst<ElementType>::Type>;
	using IteratorType = ElementType*;

	// Constructors
	// -------------------------------------------------------------------------

	// Empty array, no view.
	constexpr TArrayView() noexcept = default;

	// Default construction from nullptr
	constexpr TArrayView(TNullptr) noexcept;

	// Makes view to initializer list
	constexpr TArrayView(const ILType& list) noexcept;

	// Makes view to array `data` of specific size
	explicit constexpr TArrayView(ElementType* data, const SizeType num) noexcept;

	template<TSize N> explicit TArrayView(ElementType (&data)[N]) noexcept;

	// Assignment Operators
	// -------------------------------------------------------------------------

	TArrayView& operator=(const ILType& list) noexcept;

	// Comparison Operators
	// Element-wise equality; both size and content must match.
	// * Two empty arrays are equal.
	// -------------------------------------------------------------------------

	bool operator==(const TArrayView& other) const noexcept;
	bool operator!=(const TArrayView& other) const noexcept;

	// Dereference & Index Operators
	// -------------------------------------------------------------------------

	// Raw pointer to the data buffer. Null if empty
	// * Safe to call on an empty array; it's a query, not a read.
	constexpr ElementType* operator*() const noexcept;

	// Unsafe access; requires 0 <= idx < GetNum().
	// * Asserts on a bad index or an empty array; undefined behavior in release.
	constexpr ElementType& operator[](SizeType idx) const noexcept;

	// Property Getters
	// -------------------------------------------------------------------------

	// Pointer to the data buffer. Null if empty and never allocated.
	constexpr ElementType* GetData() const noexcept;

	// Number of elements currently stored.
	constexpr SizeType GetNum() const noexcept;

	// Validation
	// -------------------------------------------------------------------------

	// True if the array has no elements.
	constexpr bool IsEmpty() const noexcept;

	// True if idx is in [0, GetNum()).
	// * Never asserts; the standard guard to use before operator[]
	constexpr bool IsValidIndex(SizeType idx) const noexcept;

	// Get
	// Safe, non-crashing access; unlike operator[] these never read out of bounds.
	// -------------------------------------------------------------------------

	// Requires 0 <= idx < GetNum() to read a real element.
	// * Asserts on a bad index; release builds return nullptr instead.
	constexpr ElementType* GetAt(SizeType idx) const noexcept;

	// Pointer to the first element, or null if empty. Never asserts.
	constexpr ElementType* GetFirst() const noexcept;

	// Pointer to the last element, or null if empty. Never asserts.
	constexpr ElementType* GetLast() const noexcept;

	// Find Index
	// Returns the index of the first match, or KOR_INDEX_NONE.
	// * Never asserts, even on an empty array.
	// -------------------------------------------------------------------------

	// Matches by operator==.
	constexpr SizeType FindIndex(const ElementType& val) const noexcept;

	// Matches by predicate; not noexcept, since func may throw.
	template<typename FunctorT>
	constexpr SizeType FindIndexByFunc(FunctorT&& func) const;

	// Matches by key.
	template<typename KeyType>
	constexpr SizeType FindIndexByKey(const KeyType& key) const noexcept;

	// Find Element
	// Returns a pointer to the first match, or nullptr. Never asserts.
	// -------------------------------------------------------------------------

	// Matches by predicate; not noexcept, since func may throw.
	template<typename FunctorT>
	constexpr ElementType* FindByFunc(FunctorT&& func) const;

	// Matches by key.
	template<typename KeyType>
	constexpr ElementType* FindByKey(const KeyType& key) const noexcept;

	// Contains
	// True if any element matches. Never asserts; false on an empty array.
	// -------------------------------------------------------------------------

	// Matches by operator==.
	constexpr bool Contains(const ElementType& val) const noexcept;

	// Matches by predicate; not noexcept, since func may throw.
	template<typename FunctorT>
	constexpr bool ContainsByFunc(FunctorT&& func) const;

	// Matches by key.
	template<typename KeyType>
	constexpr bool ContainsByKey(const KeyType& key) const noexcept;

	// Iterators
	// -------------------------------------------------------------------------

	constexpr IteratorType begin() noexcept;
	constexpr IteratorType end() noexcept;

private:
	// Non-owned view data
	ElementType* _data = nullptr;

	// Number of viewed elements
	SizeType _num = 0;

	// Friend
	// -------------------------------------------------------------------------

	using SFriend = Detail::ArrayView::TFriend<TArrayView>;
	friend SFriend;
};

#include "Kor/Container/Detail/ArrayView.inl"

template<typename ElementT, typename SizeT = int32>
using TConstArrayView = TArrayView<const ElementT, SizeT>;

// [ Is TArrayView ]
// Checks if type is TArrayView type

template<typename T> struct TIsTArrayView : TFalseValue {};
template<typename ElementT, typename SizeT> struct TIsTArrayView<TArrayView<ElementT, SizeT>> : TTrueValue {};

KOR_NAMESPACE_END