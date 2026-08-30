// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/Math.h"
#include "Kor/MemoryOps.h"

#include "Kor/Allocator.h"

KOR_NAMESPACE_BEGIN

// Forward declare for private array friend
namespace Internal::Array { struct SFriend; }

// [TArray]
// A dynamically-sized, heap-allocated array container.
//
// Contract:
// - ElementType must be a non-void, non-reference type
// - AllocatorType must expose a signed SizeType
// - All index parameters are bounds-checked via assert
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
		!TIsVoid<ElementT>::Value && TIsPure<ElementT>::Value,
		"ElementType must be a non-void and pure type");

	static_assert(TIsAllocator<AllocatorT>::Value,
		"AllocatorType must be a valid allocator type");

	static_assert(TIsSigned<typename TAllocatorTraits<AllocatorT>::SizeType>::Value,
		"SizeType must be a valid signed type");

	// Types
	// -------------------------------------------------------------------------

	using ElementType = ElementT;
	using AllocatorType = AllocatorT;
	using ElementAllocatorType = TTypedAllocator<AllocatorT, ElementType>;
	using SizeType = typename TAllocatorTraits<AllocatorT>::SizeType;
	using ILType = std::initializer_list<ElementType>;
	using ArrayIteratorType = ElementType*;
	using ConstArrayIteratorType = const ElementType*;

	// Constructors
	// -------------------------------------------------------------------------

	// Default-constructs an empty array with no allocation.
	constexpr TArray() noexcept;

	// Copy-constructs from another array. Performs a deep copy of all elements.
	// The new array's capacity is exactly Num of `other`; slack is not preserved.
	TArray(const TArray& other) noexcept;

	// Move-constructs from another array. Buffer, Num, and capacity are stolen
	// as-is. Source is left in a valid, empty state (Data=nullptr, Num=0, Max=0).
	constexpr TArray(TArray&& other) noexcept;

	// Constructs from an initializer list. Performs a deep copy of all elements.
	TArray(const ILType& list) noexcept;

	// Constructs by copying `num` elements from a raw pointer. Pointer must not
	// be null and must point to at least `num` valid elements.
	explicit TArray(const ElementType* data, SizeType num) noexcept;

	// Constructs without initializing internal state. Caller is responsible for
	// ensuring valid state before use.
	explicit constexpr TArray(Init::SNoInit) noexcept;

	// Reserves capacity for `num` elements. Num remains 0; no elements are
	// constructed.
	explicit TArray(SizeType num, Init::SNoInit) noexcept;

	// Reserves capacity for `num` elements and default-constructs each one.
	// Num becomes `num`.
	explicit TArray(SizeType num, Init::SDefault) noexcept;

	// Reserves capacity for `num` elements and zero-initializes the buffer.
	// Num becomes `num`. Only valid for trivially-constructible ElementType.
	explicit TArray(SizeType num, Init::SZero) noexcept;

	// Reserves capacity for `num` elements, each copy-constructed from `value`.
	// Num becomes `num`.
	explicit TArray(SizeType num, const ElementType& value) noexcept;

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
	ElementType* operator*() noexcept;
	const ElementType* operator*() const noexcept;

	// Index Operators
	// -------------------------------------------------------------------------

	// Asserts that `idx` is a valid index.
	ElementType& operator[](SizeType idx) noexcept;
	const ElementType& operator[](SizeType idx) const noexcept;

	// Property Getters
	// -------------------------------------------------------------------------

	// Returns allocator instance used for allocating elements
	ElementAllocatorType& GetAllocator() noexcept;
	const ElementAllocatorType& GetAllocator() const noexcept;

	// Returns a pointer to the underlying data buffer. May be null if empty
	// and no allocation has been made.
	ElementType* GetData() noexcept;
	const ElementType* GetData() const noexcept;

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

	// Returns a pointer to the element at `idx`. Asserts.
	// Returns null if `idx` is out of range in release builds.
	ElementType* GetAt(SizeType idx) noexcept;
	const ElementType* GetAt(SizeType idx) const noexcept;

	// Returns a pointer to the first element, or null if empty.
	ElementType* GetFirst() noexcept;
	const ElementType* GetFirst() const noexcept;

	// Returns a pointer to the last element, or null if empty.
	ElementType* GetLast() noexcept;
	const ElementType* GetLast() const noexcept;

	// Reserve / Resize / Reset
	// -------------------------------------------------------------------------

	// Ensures capacity for at least `num` elements without changing Num.
	// No-op if current capacity already satisfies the request.
	void Reserve(SizeType num) noexcept;

	// Sets the element count to `num`. New elements are default-constructed;
	// excess elements are destroyed. Does not release memory if shrinking.
	void Resize(SizeType num) noexcept;

	// Sets the element count to `num`. New elements are zero-constructed;
	// excess elements are destroyed. Does not release memory if shrinking.
	void ResizeZeroed(SizeType num) noexcept;

	// Sets the element count to `num`. New elements are unitialized;
	// excess elements are destroyed. Does not release memory if shrinking.
	void ResizeUnitialized(SizeType num) noexcept;

	// Releases excess capacity so that reserved == num.
	void ShrinkToFit() noexcept;

	// Destroys all elements. Num becomes 0. Capacity is unchanged.
	void Reset() noexcept;

	// Destroys all elements and optionally re-reserves `newNum` slots.
	void Empty(SizeType num = 0) noexcept;

	// Add
	// -------------------------------------------------------------------------

	// Appends a copy of `val` to the end
	// Returns the index of the added element.
	template<typename ValueType>
	SizeType Add(ValueType&& val) noexcept;

	// Appends a copy of `val` and returns a reference to the newly added element.
	template<typename ValueType>
	ElementType& Add_GetRef(ValueType&& val) noexcept;

	// Appends a copy of `val` only if no element equal to it already exists.
	// Returns the index of the existing or newly added element.
	template<typename ValueType>
	SizeType AddUnique(ValueType&& val) noexcept;

	// Appends `val` if no element equal to it already exists and returns a reference to the newly added element.
	template<typename ValueType>
	ElementType& AddUnique_GetRef(ValueType&& val) noexcept;

	// Appends `num` default-constructed elements. 
	// Returns the index of the first added element.
	SizeType AddDefaulted(SizeType num = 1) noexcept;

	// Appends a single default-constructed element and returns a reference to it.
	ElementType& AddDefaulted_GetRef() noexcept;

	// Appends `num` zero-constructed elements. 
	// Returns the index of the first added element.
	SizeType AddZeroed(SizeType num = 1) noexcept;

	// Appends a single zero-constructed element and returns a reference to it.
	ElementType& AddZeroed_GetRef() noexcept;

	// Appends `num` elements with uninitialized memory. 
	// Returns the index of the first added element. 
	// Caller must initialize all added elements before reading.
	SizeType AddUninitialized(SizeType num = 1) noexcept;

	// Appends a single uninitialized element and returns a reference to it.
	// Caller must initialize before reading.
	ElementType& AddUninitialized_GetRef() noexcept;

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

	// Appends all elements from `list`.
	void Append(const ILType& list) noexcept;

	// Appends `numToAdd` copies of `val`.
	void Append(const ElementType& val, SizeType num) noexcept;

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
	ElementType* FindByFunc(Functor&& func);

	template<typename Functor>
	const ElementType* FindByFunc(Functor&& func) const;

	// Returns a pointer to the first element whose key matches `key`,
	// or null if not found.
	template<typename KeyType>
	ElementType* FindByKey(KeyType key) noexcept;

	template<typename KeyType>
	const ElementType* FindByKey(KeyType key) const noexcept;

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
	// Allocator instance for _data
	ElementAllocatorType _allocator;

	// Allocated data
	ElementType* _data;

	// Number of initialized elements
	SizeType _num;

	// Number of reserved elements (size of _data buffer)
	SizeType _reservedNum;

	// Implementation friend
	friend Internal::Array::SFriend;
};

#include "Kor/Inl/Array.inl"

KOR_NAMESPACE_END
