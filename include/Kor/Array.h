// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/Math.h"
#include "Kor/MemoryOps.h"

#include "Kor/ArrayAllocator.h"

KOR_NAMESPACE_BEGIN

// [TArray]
// A dynamically-sized, heap-allocated array container.
//
// Contract:
// - ElementType must be a non-void, non-reference type
// - AllocatorType must expose a signed SizeType
// - All index parameters are bounds-checked via assert in debug builds
// - Ownership is exclusive; copying performs a deep copy, moving transfers ownership
// - Iterators and pointers into the array are invalidated by any operation that
//   modifies capacity (Reserve, Add, Remove, Resize, etc.)
template<typename ElementT, typename AllocatorT>
class TArray
{
public:
	// Asserts
	// -------------------------------------------------------------------------

	static_assert(
		!TIsSame<ElementT, void>::Value && !TIsReference<ElementT>::Value,
		"ElementType must be a non-void, non-reference type");

	static_assert(TIsAllocator<AllocatorT>::Value,
		"AllocatorType must be a valid allocator type");

	static_assert(TIsSigned<typename TAllocatorTraits<AllocatorT>::SizeType>::Value,
		"SizeType must be a valid signed type");

	// Types
	// -------------------------------------------------------------------------

	using ElementType = ElementT;
	using AllocatorType = AllocatorT;
	using SizeType = typename TAllocatorTraits<AllocatorT>::SizeType;
	using ILType = std::initializer_list<ElementType>;
	using ArrayIteratorType = ElementType*;
	using ConstArrayIteratorType = const ElementType*;

	// Constructors
	// -------------------------------------------------------------------------

	// Default-constructs an empty array with no allocation.
	constexpr TArray() noexcept;

	// Constructs without initializing internal state. Caller is responsible for
	// ensuring valid state before use.
	constexpr TArray(Init::SNoInit) noexcept;

	// Constructs an empty array with all memory zero-initialized.
	constexpr TArray(Init::SZero) noexcept;

	// Copy-constructs from another array. Performs a deep copy of all elements.
	TArray(const TArray& other) noexcept;

	// Move-constructs from another array. Source is left in a valid empty state.
	TArray(TArray&& other) noexcept;

	// Constructs with `num` elements. If `reserveOnly` is true, capacity is
	// reserved but no elements are added (Num remains 0).
	TArray(SizeType num, bool reserveOnly = false) noexcept;

	// Constructs from an initializer list.
	TArray(const ILType& list) noexcept;

	// Constructs by copying `num` elements from a raw pointer. Pointer must not
	// be null and must point to at least `num` valid elements.
	TArray(const ElementType* data, SizeType num) noexcept;

	// Destructor
	// -------------------------------------------------------------------------

	~TArray() noexcept;

	// Assign Operators
	// -------------------------------------------------------------------------

	TArray& operator=(const TArray& other) noexcept;
	TArray& operator=(TArray&& other) noexcept;
	TArray& operator=(const ILType& list) noexcept;

	// Compare Operators
	// -------------------------------------------------------------------------

	// Element-wise equality. Both size and content must match.
	bool operator==(const TArray& other) const noexcept;
	bool operator!=(const TArray& other) const noexcept;

	// Dereference Operators
	// -------------------------------------------------------------------------

	// Returns a raw pointer to the underlying data buffer. May be null if the
	// array has never allocated (i.e. empty with no reserve).
	const ElementType* operator*() const noexcept;
	ElementType* operator*() noexcept;

	// Index Operators
	// -------------------------------------------------------------------------

	// Asserts that `idx` is a valid index in debug builds.
	ElementType& operator[](SizeType idx) noexcept;
	const ElementType& operator[](SizeType idx) const noexcept;

	// Property Getters
	// -------------------------------------------------------------------------

	// Returns a pointer to the underlying data buffer. May be null if empty
	// and no allocation has been made.
	const ElementType* GetData() const noexcept;
	ElementType* GetData() noexcept;

	// Returns the number of elements currently stored.
	SizeType GetNum() const noexcept;

	// Returns the total number of elements the array can hold before
	// reallocating.
	SizeType GetReservedNum() const noexcept;

	// Validation
	// -------------------------------------------------------------------------

	// Returns true if the array contains no elements.
	bool IsEmpty() const noexcept;

	// Returns true if `idx` is within [0, GetNum()).
	bool IsValidIndex(SizeType idx) const noexcept;

	// Get
	// -------------------------------------------------------------------------

	// Returns a pointer to the element at `idx`. Asserts in debug builds.
	// Returns null if `idx` is out of range in release builds.
	const ElementType* GetAt(SizeType idx) const noexcept;
	ElementType* GetAt(SizeType idx) noexcept;

	// Returns a pointer to the first element, or null if empty.
	const ElementType* GetFirst() const noexcept;
	ElementType* GetFirst() noexcept;

	// Returns a pointer to the last element, or null if empty.
	const ElementType* GetLast() const noexcept;
	ElementType* GetLast() noexcept;

	// Reserve / Resize / Reset
	// -------------------------------------------------------------------------

	// Ensures capacity for at least `num` elements without changing Num.
	// No-op if current capacity already satisfies the request.
	void Reserve(SizeType num) noexcept;

	// Sets the element count to `num`. New elements are default-constructed;
	// excess elements are destroyed. Does not release memory if shrinking.
	void SetNum(SizeType num) noexcept;

	// Alias for SetNum. Sets element count to `num`, constructing or
	// destroying elements as needed.
	void Resize(SizeType num) noexcept;

	// Releases excess capacity so that reserved == num.
	void ShrinkToFit() noexcept;

	// Destroys all elements. Num becomes 0. Capacity is unchanged.
	void Reset() noexcept;

	// Destroys all elements and optionally re-reserves `newNum` slots.
	// Equivalent to Reset() + Reserve(newNum).
	void Empty(SizeType newNum = 0) noexcept;

	// Add
	// -------------------------------------------------------------------------

	// Appends a copy of `val` to the end. Returns the index of the added element.
	SizeType Add(const ElementType& val) noexcept;

	// Appends `val` to the end via move. Returns the index of the added element.
	SizeType Add(ElementType&& val) noexcept;

	// Appends a copy of `val` and returns a reference to the newly added element.
	ElementType& Add_GetRef(const ElementType& val) noexcept;

	// Appends `val` via move and returns a reference to the newly added element.
	ElementType& Add_GetRef(ElementType&& val) noexcept;

	// Appends `num` default-constructed elements. Returns the index of the
	// first added element.
	SizeType AddDefaulted(SizeType num = 1) noexcept;

	// Appends a single default-constructed element and returns a reference to it.
	ElementType& AddDefaulted_GetRef() noexcept;

	// Appends `num` elements with uninitialized memory. Returns the index of
	// the first added element. Caller must initialize all added elements before
	// reading.
	SizeType AddUninitialized(SizeType num = 1) noexcept;

	// Appends a single uninitialized element and returns a reference to it.
	// Caller must initialize before reading.
	ElementType& AddUninitialized_GetRef() noexcept;

	// Appends a copy of `val` only if no element equal to it already exists.
	// Returns the index of the existing or newly added element.
	SizeType AddUnique(const ElementType& val) noexcept;

	// Push/Pop aliases for stack-style usage.
	void Push(const ElementType& val) noexcept;
	void Push(ElementType&& val) noexcept;

	// Emplace
	// -------------------------------------------------------------------------

	// Constructs an element in-place at the end from `args`. Returns its index.
	template<typename... ArgTypes>
	SizeType Emplace(ArgTypes&&... args) noexcept;

	// Constructs an element in-place at the end from `args` and returns a
	// reference to it.
	template<typename... ArgTypes>
	ElementType& Emplace_GetRef(ArgTypes&&... args) noexcept;

	// Insert
	// -------------------------------------------------------------------------

	// Inserts a copy of `val` at `idx`, shifting subsequent elements right.
	// Asserts that `idx` is in [0, GetNum()].
	void Insert(SizeType idx, const ElementType& val) noexcept;

	// Inserts `val` via move at `idx`, shifting subsequent elements right.
	void Insert(SizeType idx, ElementType&& val) noexcept;

	// Inserts `num` elements from `data` at `idx`, shifting subsequent elements
	// right. `data` must not be null and must point to at least `num` elements.
	void Insert(SizeType idx, const ElementType* data, SizeType num) noexcept;

	// Inserts all elements from `list` at `idx`, shifting subsequent elements
	// right.
	void Insert(SizeType idx, const ILType& list) noexcept;

	// Append
	// -------------------------------------------------------------------------

	// Appends all elements from `other` to the end.
	void Append(const TArray& other) noexcept;
	void Append(TArray&& other) noexcept;

	// Appends `numToAdd` copies of `val`.
	void Append(const ElementType& val, SizeType numToAdd) noexcept;

	// Appends all elements from `list`.
	void Append(const ILType& list) noexcept;

	// Appends `num` elements from `data`. `data` must not be null.
	void Append(const ElementType* data, SizeType num) noexcept;

	// Appends `numToAdd` elements with uninitialized memory. Caller must
	// initialize before reading.
	void AppendUninitialized(SizeType numToAdd) noexcept;

	// Remove
	// -------------------------------------------------------------------------

	// Removes all elements equal to `val`, preserving order of remaining
	// elements. Returns the number of elements removed.
	// Optionally shrinks capacity if `allowShrink` is true.
	SizeType Remove(const ElementType& val, bool allowShrink = true) noexcept;

	// Removes all elements equal to `val` using swap-with-last (does not
	// preserve order). Returns the number of elements removed.
	SizeType RemoveSwap(const ElementType& val, bool allowShrink = true) noexcept;

	// Removes all elements for which `predicate(element)` returns true,
	// preserving order. Returns the number of elements removed.
	// Note: not marked noexcept; predicate exception behavior is unknown.
	template<typename Predicate>
	SizeType RemoveByPredicate(Predicate&& predicate, bool allowShrink = true);

	// Removes all elements for which `predicate(element)` returns true using
	// swap-with-last (does not preserve order). Returns the number removed.
	// Note: not marked noexcept; predicate exception behavior is unknown.
	template<typename Predicate>
	SizeType RemoveSwapByPredicate(Predicate&& predicate, bool allowShrink = true);

	// Removes the first element equal to `val`, preserving order.
	// Returns its former index, or KOR_INDEX_NONE if not found.
	SizeType RemoveFirst(const ElementType& val) noexcept;

	// Removes the first element equal to `val` using swap-with-last.
	// Returns its former index, or KOR_INDEX_NONE if not found.
	SizeType RemoveSwapFirst(const ElementType& val) noexcept;

	// Removes the element at `idx`, shifting subsequent elements left.
	void RemoveAt(SizeType idx) noexcept;

	// Removes `count` elements starting at `idx`, shifting subsequent elements
	// left.
	void RemoveAt(SizeType idx, SizeType count) noexcept;

	// Removes the element at `idx` using swap-with-last (does not preserve
	// order). Faster than RemoveAt for large arrays.
	void RemoveAtSwap(SizeType idx) noexcept;

	// Removes the element at `idx`, shifts remaining elements, and returns a
	// copy of the removed element.
	ElementType RemoveAt_GetCopy(SizeType idx) noexcept;

	// Removes the element at `idx` using swap-with-last and returns a copy of
	// the removed element.
	ElementType RemoveAtSwap_GetCopy(SizeType idx) noexcept;

	// Removes and returns the last element. Array must not be empty.
	ElementType Pop() noexcept;

	// Replace
	// -------------------------------------------------------------------------

	// Replaces the contents of this array with `other` via copy.
	void Replace(const TArray& other) noexcept;

	// Replaces the contents of this array with `other` via move. `other` is
	// left in a valid empty state.
	void Replace(TArray&& other) noexcept;

	// Swap
	// -------------------------------------------------------------------------

	// Swaps the elements at `firstIdx` and `secondIdx`.
	void Swap(SizeType firstIdx, SizeType secondIdx) noexcept;

	// Swaps `num` contiguous elements starting at `firstIdx` with those
	// starting at `secondIdx`. Ranges must not overlap.
	void SwapRange(SizeType firstIdx, SizeType secondIdx, SizeType num = 1) noexcept;

	// Fill / Assign
	// -------------------------------------------------------------------------

	// Sets all existing elements to a copy of `val`. Does not change Num.
	void Fill(const ElementType& val) noexcept;

	// Replaces the contents with `num` copies of `val`. Equivalent to
	// Reset() + Append(val, num).
	void Assign(const ElementType& val, SizeType num) noexcept;

	// Replaces the contents by copying `num` elements from `data`. `data`
	// must not be null and must point to at least `num` valid elements.
	void Assign(const ElementType* data, SizeType num) noexcept;

	// Sort
	// -------------------------------------------------------------------------

	// Sorts elements in ascending order using the default less-than operator.
	void Sort() noexcept;

	// Sorts elements using `predicate(a, b)` returning true if `a` should come
	// before `b`.
	// Note: not marked noexcept; predicate exception behavior is unknown.
	template<typename Predicate>
	void Sort(Predicate&& predicate);

	// Stable sort: preserves relative order of equivalent elements.
	void StableSort() noexcept;

	// Note: not marked noexcept; predicate exception behavior is unknown.
	template<typename Predicate>
	void StableSort(Predicate&& predicate);

	// Reverses the order of all elements in-place.
	void Reverse() noexcept;

	// Find Index
	// -------------------------------------------------------------------------

	// Returns the index of the first element equal to `val`, or KOR_INDEX_NONE.
	SizeType FindIndex(const ElementType& val) const noexcept;

	// Returns the index of the first element for which `func(element)` returns
	// true, or KOR_INDEX_NONE.
	// Note: not marked noexcept; functor exception behavior is unknown.
	template<typename Functor>
	SizeType FindIndexByFunc(Functor&& func) const;

	// Returns the index of the first element whose key matches `key`,
	// or KOR_INDEX_NONE.
	template<typename KeyType>
	SizeType FindIndexByKey(KeyType key) const noexcept;

	// Find Element
	// -------------------------------------------------------------------------

	// Returns a pointer to the first element for which `func(element)` returns
	// true, or null if not found.
	// Note: not marked noexcept; functor exception behavior is unknown.
	template<typename Functor>
	const ElementType* FindByFunc(Functor&& func) const;

	template<typename Functor>
	ElementType* FindByFunc(Functor&& func);

	// Returns a pointer to the first element whose key matches `key`,
	// or null if not found.
	template<typename KeyType>
	const ElementType* FindByKey(KeyType key) const noexcept;

	template<typename KeyType>
	ElementType* FindByKey(KeyType key) noexcept;

	// Contains
	// -------------------------------------------------------------------------

	// Returns true if any element is equal to `val`.
	bool Contains(const ElementType& val) const noexcept;

	// Returns true if any element satisfies `func(element)`.
	// Note: not marked noexcept; functor exception behavior is unknown.
	template<typename Functor>
	bool ContainsByFunc(Functor&& func) const;

	// Returns true if any element's key matches `key`.
	template<typename KeyType>
	bool ContainsByKey(KeyType key) const noexcept;

	// Iterators
	// -------------------------------------------------------------------------

	ArrayIteratorType begin() noexcept;
	ConstArrayIteratorType begin() const noexcept;
	ArrayIteratorType end() noexcept;
	ConstArrayIteratorType end() const noexcept;

private:
	AllocatorType _allocator = {};
	SizeType _num = 0;
};

#include "Kor/Inl/Array.inl"

KOR_NAMESPACE_END
