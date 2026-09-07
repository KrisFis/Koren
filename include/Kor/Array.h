// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/Math.h"
#include "Kor/MemoryOps.h"

#include "Kor/Allocator.h"

KOR_NAMESPACE_BEGIN

// Forward declare for private array friend
namespace Internal::Array { template<typename T> struct TFriend; }

// [TArray]
// A dynamically-sized, heap-allocated array container.
//
// Contract:
// - ElementType must be a non-void, non-reference type
// - AllocatorType must expose a signed SizeType
// - Ownership is exclusive; copying performs a deep copy, moving transfers ownership
// - Iterators and pointers into the array are invalidated by any operation that
//   modifies capacity (Reserve, Add, Remove, Resize, etc.)
//
// Value contract:
// - num parameters are 0-safe; negative values assert.
// - Asserts fire in all build configs, not just debug.
// - A data+num pair only needs a non-null pointer when num > 0.
// - GetAt/GetFirst/GetLast never crash on bad input: they assert and return
//   nullptr. operator[] and Pop have no fallback - treat them as unchecked.
// - Find/Contains never assert, even on an empty array.
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

	// Empty array, no allocation.
	constexpr TArray() noexcept;

	// Deep copy. Capacity becomes exactly Num of `other`; slack is not preserved.
	TArray(const TArray& other) noexcept;

	// Steals buffer/Num/capacity from `other`. `other` is left empty
	// (Data=nullptr, Num=0, Max=0).
	constexpr TArray(TArray&& other) noexcept;
	
	// Deep copy from an initializer list. `{}` is valid and yields an empty array.
	TArray(const ILType& list) noexcept;

	// Leaves internal state uninitialized. Caller must bring it to a valid
	// state before any other use.
	explicit constexpr TArray(Init::SNoInit) noexcept;

	// Reserves `num` elements without constructing any. Num stays 0.
	// `num == 0` is valid (no allocation).
	explicit TArray(SizeType num, Init::SNoInit) noexcept;

	// Reserves and default-constructs `num` elements. Num becomes `num`.
	// `num == 0` is valid (empty result).
	explicit TArray(SizeType num, Init::SDefault) noexcept;

	// Reserves `num` elements and zero-fills them. Num becomes `num`.
	// `num == 0` is valid. Only for trivially-constructible ElementType.
	explicit TArray(SizeType num, Init::SZero) noexcept;

	// Copies `num` elements from `data`. `data` may be null only if `num == 0`;
	// otherwise it must point to at least `num` valid elements.
	explicit TArray(const ElementType* data, SizeType num) noexcept;
	
	// Reserves `num` elements, each copy-constructed from `value`. Num becomes `num`.
	// `num == 0` is valid (empty result).
	explicit TArray(const ElementType& value, SizeType num) noexcept;

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

	// Element-wise equality; both size and content must match. Two empty
	// arrays are equal.
	bool operator==(const TArray& other) const noexcept;
	bool operator!=(const TArray& other) const noexcept;

	// Dereference Operators
	// -------------------------------------------------------------------------

	// Raw pointer to the data buffer. Null if empty and never allocated.
	// Safe to call on an empty array (it's a query, not a read).
	ElementType* operator*() noexcept;
	const ElementType* operator*() const noexcept;

	// Index Operators
	// -------------------------------------------------------------------------

	// Unsafe access. Requires 0 <= idx < GetNum(). Asserts on a bad
	// index or an empty array; undefined behavior in release.
	ElementType& operator[](SizeType idx) noexcept;
	const ElementType& operator[](SizeType idx) const noexcept;

	// Property Getters
	// -------------------------------------------------------------------------

	ElementAllocatorType& GetAllocator() noexcept;
	const ElementAllocatorType& GetAllocator() const noexcept;

	// Pointer to the data buffer. Null if empty and never allocated.
	ElementType* GetData() noexcept;
	const ElementType* GetData() const noexcept;

	// Number of elements currently stored.
	SizeType GetNum() const noexcept;

	// Number of elements storable before the next reallocation.
	SizeType GetReservedNum() const noexcept;

	// Validation
	// -------------------------------------------------------------------------

	bool IsEmpty() const noexcept;

	// True if idx is in [0, GetNum()). Never asserts -- the standard guard to
	// use before operator[]/Insert/RemoveAt.
	bool IsValidIndex(SizeType idx) const noexcept;

	// Get
	// -------------------------------------------------------------------------

	// Safe access. Requires 0 <= idx < GetNum() to read a real element.
	// Asserts on a bad index, but unlike operator[] it never reads out
	// of bounds: release builds return nullptr instead.
	ElementType* GetAt(SizeType idx) noexcept;
	const ElementType* GetAt(SizeType idx) const noexcept;

	// Pointer to the first element, or null if empty. Never asserts.
	ElementType* GetFirst() noexcept;
	const ElementType* GetFirst() const noexcept;

	// Pointer to the last element, or null if empty. Never asserts.
	ElementType* GetLast() noexcept;
	const ElementType* GetLast() const noexcept;

	// Reserve / Resize / Reset
	// -------------------------------------------------------------------------

	// Ensures capacity for at least `num` elements; Num is unchanged.
	// No-op if capacity already satisfies the request. `num == 0` is valid.
	void Reserve(SizeType num) noexcept;

	// Sets Num to `num`. Grown elements are default-constructed; shrunk
	// elements are destroyed. Never releases memory. `num == 0` empties the
	// array without freeing capacity.
	void Resize(SizeType num) noexcept;

	// Same as Resize, but grown elements are zero-constructed.
	void ResizeZeroed(SizeType num) noexcept;

	// Same as Resize, but grown elements are left uninitialized. Caller must
	// initialize them before reading. Intended for trivially-constructible
	// ElementType.
	void ResizeUninitialized(SizeType num) noexcept;

	// Frees excess capacity so GetReservedNum() == GetNum(). No-op if already
	// tight or empty.
	void ShrinkToFit() noexcept;

	// Destroys all elements. Num becomes 0. Capacity is unchanged. No-op if
	// already empty.
	void Reset() noexcept;

	// Destroys all elements, then reserves `num` slots (default 0). `num == 0`
	// on an already-empty array is a no-op.
	void Empty(SizeType num = 0) noexcept;

	// Add
	// -------------------------------------------------------------------------

	// Appends a copy of `val`. Returns its index.
	SizeType Add(const ElementType& val) noexcept;
	SizeType Add(ElementType&& val) noexcept;

	// Appends a copy of `val`. Returns a reference to it.
	ElementType& Add_GetRef(const ElementType& val) noexcept;
	ElementType& Add_GetRef(ElementType&& val) noexcept;

	// Appends `val` only if no equal element already exists (requires
	// operator==). Returns the index of the existing or newly added element.
	// No-op (beyond the lookup) if an equal element is already present.
	SizeType AddUnique(const ElementType& val) noexcept;
	SizeType AddUnique(ElementType&& val) noexcept;

	// Same as AddUnique, but returns a reference to the existing/new element.
	ElementType& AddUnique_GetRef(const ElementType& val) noexcept;
	ElementType& AddUnique_GetRef(ElementType&& val) noexcept;

	// Appends `num` default-constructed elements. Returns the index of the
	// first one added. `num == 0` is a no-op.
	SizeType AddDefaulted(SizeType num = 1) noexcept;

	// Appends one default-constructed element. Returns a reference to it.
	ElementType& AddDefaulted_GetRef() noexcept;

	// Appends `num` zero-constructed elements. Returns the index of the
	// first one added. `num == 0` is a no-op.
	SizeType AddZeroed(SizeType num = 1) noexcept;

	// Appends one zero-constructed element. Returns a reference to it.
	ElementType& AddZeroed_GetRef() noexcept;

	// Appends `num` uninitialized elements. Returns the index of the first
	// one added. `num == 0` is a no-op. Caller must initialize each added
	// element before reading it.
	SizeType AddUninitialized(SizeType num = 1) noexcept;

	// Appends one uninitialized element. Returns a reference to it. Caller
	// must initialize it before reading.
	ElementType& AddUninitialized_GetRef() noexcept;

	// Push/Pop aliases for stack-style usage. Push behaves like Add.
	void Push(const ElementType& val) noexcept;
	void Push(ElementType&& val) noexcept;

	// Emplace
	// -------------------------------------------------------------------------

	// Constructs an element in place at the end from `args`. Returns its index.
	template<typename... ArgTypes>
	SizeType Emplace(ArgTypes&&... args) noexcept;

	// Same as Emplace, but returns a reference to the new element.
	template<typename... ArgTypes>
	ElementType& Emplace_GetRef(ArgTypes&&... args) noexcept;

	// Insert
	// -------------------------------------------------------------------------

	// Inserts a copy of `val` at `idx`, shifting elements at/after `idx` right.
	// Requires 0 <= idx <= GetNum(); idx == GetNum() behaves like Add.
	void Insert(SizeType idx, const ElementType& val) noexcept;
	void Insert(SizeType idx, ElementType&& val) noexcept;

	// Inserts `num` elements from `data` at `idx`. Requires
	// 0 <= idx <= GetNum(). `data` may be null only if `num == 0`; otherwise
	// it must point to at least `num` elements. `num == 0` is a no-op.
	void Insert(SizeType idx, const ElementType* data, SizeType num) noexcept;

	// Inserts all elements from `list` at `idx`. Requires 0 <= idx <= GetNum().
	// An empty list is a no-op.
	void Insert(SizeType idx, const ILType& list) noexcept;

	// Append
	// -------------------------------------------------------------------------

	// Appends all elements from `other`. An empty `other` is a no-op.
	SizeType Append(const TArray& other) noexcept;
	SizeType Append(TArray&& other) noexcept;

	// Appends all elements from `list`. An empty list is a no-op.
	SizeType Append(const ILType& list) noexcept;

	// Appends `num` copies of `val`. `num == 0` is a no-op.
	SizeType Append(const ElementType& val, SizeType num) noexcept;

	// Appends `num` elements from `data`. `data` may be null only if
	// `num == 0`; otherwise it must point to at least `num` elements.
	SizeType Append(const ElementType* data, SizeType num) noexcept;

	// Remove
	// -------------------------------------------------------------------------

	// Removes every/single element equal to `val` (requires operator==), preserving
	// order. Returns the number removed; 0 if none matched or the array is
	// empty. Optionally shrinks capacity afterward.
	SizeType Remove(const ElementType& val, bool allowShrink = true) noexcept;
	SizeType RemoveSingle(const ElementType& val) noexcept;

	// Removes every/single element for which `func(element)` is true, preserving
	// order. Returns the number removed; 0 if none matched or the array is
	// empty. Not noexcept: func may throw.
	template<typename FunctorT>
	SizeType RemoveByFunc(FunctorT&& functor, bool allowShrink = true);

	template<typename FunctorT>
	SizeType RemoveSingleByFunc(FunctorT&& functor);

	// Same as Remove, but uses swap-with-last and does not preserve order.
	SizeType RemoveSwap(const ElementType& val, bool allowShrink = true) noexcept;
	SizeType RemoveSwapSingle(const ElementType& val) noexcept;

	// Same as RemoveByFunc, but uses swap-with-last and does not preserve order.
	template<typename FunctorT>
	SizeType RemoveSwapByFunc(FunctorT&& functor, bool allowShrink = true);

	template<typename FunctorT>
	SizeType RemoveSwapSingleByFunc(FunctorT&& functor);

	// Removes `num` elements starting at `idx`, shifting later elements left.
	// Requires: 0 <= idx, idx + num <= GetNum(), num >= 0
	void RemoveAt(SizeType idx, SizeType num = 1) noexcept;

	// Removes the element at `idx` via swap-with-last (order not preserved; 
	// faster than RemoveAt for large arrays).
	// Requires: 0 <= idx, idx + num <= GetNum(), num >= 0
	void RemoveAtSwap(SizeType idx, SizeType num = 1) noexcept;

	// Same as RemoveAt(idx), but returns a copy of the removed element.
	ElementType RemoveAt_GetCopy(SizeType idx) noexcept;

	// Same as RemoveAtSwap(idx), but returns a copy of the removed element.
	ElementType RemoveAtSwap_GetCopy(SizeType idx) noexcept;

	// Removes and returns the last element. Requires GetNum() > 0; asserts on
	// an empty array (no safe-empty fallback, unlike GetLast()).
	ElementType Pop() noexcept;

	// Swap
	// -------------------------------------------------------------------------

	// Swaps the elements at `firstIdx` and `secondIdx`. Both must be in
	// [0, GetNum()). `firstIdx == secondIdx` is a no-op.
	void Swap(SizeType firstIdx, SizeType secondIdx) noexcept;

	// Swaps `num` contiguous elements starting at `firstIdx` with `num`
	// starting at `secondIdx`. Both ranges must lie within [0, GetNum()) and
	// must not overlap. `num == 0` is a no-op.
	void SwapRange(SizeType firstIdx, SizeType secondIdx, SizeType num = 1) noexcept;

	// Fill / Assign
	// -------------------------------------------------------------------------

	// Overwrites every existing element with a copy of `val`. Num is
	// unchanged. No-op on an empty array.
	void Fill(const ElementType& val) noexcept;

	// Replaces this array's contents with a copy of `other`.
	void Assign(const TArray& other) noexcept;

	// Replaces this array's contents by taking `other`'s. `other` is left empty.
	void Assign(TArray&& other) noexcept;

	// Replaces the contents with `num` copies of `val`. Equivalent to
	// Reset() + Append(val, num). `num == 0` empties the array.
	void Assign(const ElementType& val, SizeType num) noexcept;

	// Replaces the contents by copying `num` elements from `data`. `data` may
	// be null only if `num == 0`; otherwise it must point to at least `num`
	// valid elements.
	void Assign(const ElementType* data, SizeType num) noexcept;

	// Sort
	// -------------------------------------------------------------------------

	// Ascending sort using operator<. No-op on an empty or single-element array.
	void Sort() noexcept;

	// Sorts using predicate(a, b) == true if `a` belongs before `b`. No-op on
	// an empty or single-element array. Not noexcept: predicate may throw.
	template<typename Predicate>
	void Sort(Predicate&& predicate);

	// Stable sort: equal elements keep their relative order. No-op on an
	// empty or single-element array.
	void StableSort() noexcept;

	// Not noexcept: predicate may throw.
	template<typename Predicate>
	void StableSort(Predicate&& predicate);

	// Reverses element order in place. No-op on an empty or single-element array.
	void Reverse() noexcept;

	// Find Index
	// -------------------------------------------------------------------------

	// Index of the first element equal to `val`, or KOR_INDEX_NONE. Never
	// asserts; safe on an empty array.
	SizeType FindIndex(const ElementType& val) const noexcept;

	// Index of the first element for which func(element) is true, or
	// KOR_INDEX_NONE. Never asserts. Not noexcept: functor may throw.
	template<typename FunctorT>
	SizeType FindIndexByFunc(FunctorT&& func) const;

	// Index of the first element whose key matches `key`, or KOR_INDEX_NONE.
	// Never asserts.
	template<typename KeyType>
	SizeType FindIndexByKey(const KeyType& key) const noexcept;

	// Find Element
	// -------------------------------------------------------------------------

	// Pointer to the first element for which func(element) is true, or
	// nullptr. Never asserts. Not noexcept: functor may throw.
	template<typename FunctorT>
	ElementType* FindByFunc(FunctorT&& func);

	template<typename FunctorT>
	const ElementType* FindByFunc(FunctorT&& func) const;

	// Pointer to the first element whose key matches `key`, or nullptr.
	// Never asserts.
	template<typename KeyType>
	ElementType* FindByKey(const KeyType& key) noexcept;

	template<typename KeyType>
	const ElementType* FindByKey(const KeyType& key) const noexcept;

	// Contains
	// -------------------------------------------------------------------------

	// True if any element equals `val`. Never asserts; false on an empty array.
	bool Contains(const ElementType& val) const noexcept;

	// True if any element satisfies func(element). Never asserts. Not
	// noexcept: functor may throw.
	template<typename FunctorT>
	bool ContainsByFunc(FunctorT&& func) const;

	// True if any element's key matches `key`. Never asserts.
	template<typename KeyType>
	bool ContainsByKey(const KeyType& key) const noexcept;

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

	// Friend
	// -------------------------------------------------------------------------

	using SFriend = Internal::Array::TFriend<TArray>;
	friend SFriend;
};

#include "Kor/Inl/Array.inl"

KOR_NAMESPACE_END