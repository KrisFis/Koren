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

// [ TArray ]
// A dynamically-sized, heap-allocated array container.
// * ElementType must be non-void and pure (non-reference, non-cv)
// * AllocatorType must expose a signed SizeType
// * Ownership is exclusive; copying performs a deep copy, moving transfers ownership
//
// Example:
//   TArray<int32> values = { 1, 2, 3 };
//   values.Add(4);
//
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

	// Deep copy.
	// * Capacity becomes exactly Num of `other`; slack is not preserved.
	TArray(const TArray& other) noexcept;

	// Steals buffer/Num/capacity from `other`.
	// * `other` is left empty (Data=nullptr, Num=0, Max=0).
	constexpr TArray(TArray&& other) noexcept;

	// Deep copy from an initializer list. `{}` is valid and yields an empty array.
	TArray(const ILType& list) noexcept;

	// Leaves internal state uninitialized.
	// * Caller must bring it to a valid state before any other use.
	explicit constexpr TArray(Init::SNoInit) noexcept;

	// Reserves `num` elements without constructing any. Num stays 0.
	explicit TArray(SizeType num, Init::SNoInit) noexcept;

	// Reserves and default-constructs `num` elements. Num becomes `num`.
	explicit TArray(SizeType num, Init::SDefault) noexcept;

	// Reserves `num` elements and zero-fills them. Num becomes `num`.
	// * Only for trivially-constructible ElementType.
	explicit TArray(SizeType num, Init::SZero) noexcept;

	// Copies `num` elements from `data`.
	// * `data` may be null only if `num == 0`.
	explicit TArray(const ElementType* data, SizeType num) noexcept;

	// Reserves `num` elements, each copy-constructed from `value`. Num becomes `num`.
	explicit TArray(const ElementType& value, SizeType num) noexcept;

	// Destructor
	// -------------------------------------------------------------------------

	~TArray() noexcept;

	// Assignment Operators
	// -------------------------------------------------------------------------

	TArray& operator=(const TArray& other) noexcept;
	TArray& operator=(TArray&& other) noexcept;
	TArray& operator=(const ILType& list) noexcept;

	// Comparison Operators
	// Element-wise equality; both size and content must match.
	// * Two empty arrays are equal.
	// -------------------------------------------------------------------------

	bool operator==(const TArray& other) const noexcept;
	bool operator!=(const TArray& other) const noexcept;

	// Dereference Operators
	// Raw pointer to the data buffer. Null if empty and never allocated.
	// * Safe to call on an empty array; it's a query, not a read.
	// -------------------------------------------------------------------------

	ElementType* operator*() noexcept;
	const ElementType* operator*() const noexcept;

	// Index Operators
	// Unsafe access; requires 0 <= idx < GetNum().
	// * Asserts on a bad index or an empty array; undefined behavior in release.
	// -------------------------------------------------------------------------

	ElementType& operator[](SizeType idx) noexcept;
	const ElementType& operator[](SizeType idx) const noexcept;

	// Property Getters
	// -------------------------------------------------------------------------

	// Allocator instance backing this array.
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

	// True if the array has no elements.
	bool IsEmpty() const noexcept;

	// True if idx is in [0, GetNum()).
	// * Never asserts; the standard guard to use before operator[]/Insert/RemoveAt.
	bool IsValidIndex(SizeType idx) const noexcept;

	// Get
	// Safe, non-crashing access; unlike operator[] these never read out of bounds.
	// -------------------------------------------------------------------------

	// Requires 0 <= idx < GetNum() to read a real element.
	// * Asserts on a bad index; release builds return nullptr instead.
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
	// * No-op if capacity already satisfies the request.
	void Reserve(SizeType num) noexcept;

	// Sets Num to `num`, never releasing memory.
	// * Grown elements are default-constructed; shrunk elements are destroyed.
	void Resize(SizeType num) noexcept;

	// Same as Resize, but grown elements are zero-constructed.
	void ResizeZeroed(SizeType num) noexcept;

	// Same as Resize, but grown elements are left uninitialized.
	// * Caller must initialize them before reading.
	// * Intended for trivially-constructible ElementType.
	void ResizeUninitialized(SizeType num) noexcept;

	// Frees excess capacity so GetReservedNum() == GetNum().
	// * No-op if already tight or empty.
	void ShrinkToFit() noexcept;

	// Destroys all elements. Num becomes 0, capacity is unchanged.
	void Reset() noexcept;

	// Destroys all elements, then reserves `num` slots (default 0).
	void Empty(SizeType num = 0) noexcept;

	// Add
	// Appends element(s) to the end of the array.
	// * _GetRef variants behave the same but return a reference instead of an index.
	// -------------------------------------------------------------------------

	// Appends a copy of `val`. Returns its index.
	SizeType Add(const ElementType& val) noexcept;
	SizeType Add(ElementType&& val) noexcept;

	ElementType& Add_GetRef(const ElementType& val) noexcept;
	ElementType& Add_GetRef(ElementType&& val) noexcept;

	// Appends `val` only if no equal element already exists (requires operator==).
	// * Returns the index of the existing or newly added element.
	SizeType AddUnique(const ElementType& val) noexcept;
	SizeType AddUnique(ElementType&& val) noexcept;

	ElementType& AddUnique_GetRef(const ElementType& val) noexcept;
	ElementType& AddUnique_GetRef(ElementType&& val) noexcept;

	// Appends `num` default-constructed elements. Returns the index of the first one added.
	SizeType AddDefaulted(SizeType num = 1) noexcept;
	ElementType& AddDefaulted_GetRef() noexcept;

	// Appends `num` zero-constructed elements. Returns the index of the first one added.
	SizeType AddZeroed(SizeType num = 1) noexcept;
	ElementType& AddZeroed_GetRef() noexcept;

	// Appends `num` uninitialized elements. Returns the index of the first one added.
	// * Caller must initialize each added element before reading it.
	SizeType AddUninitialized(SizeType num = 1) noexcept;
	ElementType& AddUninitialized_GetRef() noexcept;

	// Push/Pop aliases for stack-style usage; Push behaves like Add.
	void Push(const ElementType& val) noexcept;
	void Push(ElementType&& val) noexcept;

	// Emplace
	// -------------------------------------------------------------------------

	// Constructs an element in place at the end from `args`. Returns its index.
	template<typename... ArgTypes>
	SizeType Emplace(ArgTypes&&... args) noexcept;

	template<typename... ArgTypes>
	ElementType& Emplace_GetRef(ArgTypes&&... args) noexcept;

	// Insert
	// Shifts elements at/after `idx` right to make room.
	// * Requires 0 <= idx <= GetNum(); idx == GetNum() behaves like Add/Append.
	// -------------------------------------------------------------------------

	// Inserts a copy of `val` at `idx`.
	void Insert(SizeType idx, const ElementType& val) noexcept;
	void Insert(SizeType idx, ElementType&& val) noexcept;

	// Inserts `num` elements from `data` at `idx`.
	// * `data` may be null only if `num == 0`.
	void Insert(SizeType idx, const ElementType* data, SizeType num) noexcept;

	// Inserts all elements from `list` at `idx`.
	void Insert(SizeType idx, const ILType& list) noexcept;

	// Append
	// Appends element(s) to the end. Returns the index of the first one added.
	// -------------------------------------------------------------------------

	// Appends all elements from `other`.
	SizeType Append(const TArray& other) noexcept;
	SizeType Append(TArray&& other) noexcept;

	// Appends all elements from `list`.
	SizeType Append(const ILType& list) noexcept;

	// Appends `num` copies of `val`.
	SizeType Append(const ElementType& val, SizeType num) noexcept;

	// Appends `num` elements from `data`.
	// * `data` may be null only if `num == 0`.
	SizeType Append(const ElementType* data, SizeType num) noexcept;

	// Remove
	// Removes elements matching a value or predicate.
	// * Remove* preserves order; RemoveSwap* uses swap-with-last and does not,
	//   but is faster for large arrays.
	// * *Single variants stop after the first match; the base variants remove all.
	// * *ByFunc variants take a predicate instead of a value and are not
	//   noexcept, since the functor may throw.
	// * All variants return the number removed, and are no-ops on an empty
	//   or non-matching array.
	// -------------------------------------------------------------------------

	// Requires operator==. Optionally shrinks capacity afterward.
	SizeType Remove(const ElementType& val, bool allowShrink = true) noexcept;
	SizeType RemoveSingle(const ElementType& val) noexcept;

	template<typename FunctorT>
	SizeType RemoveByFunc(FunctorT&& func, bool allowShrink = true);

	template<typename FunctorT>
	SizeType RemoveSingleByFunc(FunctorT&& func);

	SizeType RemoveSwap(const ElementType& val, bool allowShrink = true) noexcept;
	SizeType RemoveSwapSingle(const ElementType& val) noexcept;

	template<typename FunctorT>
	SizeType RemoveSwapByFunc(FunctorT&& func, bool allowShrink = true);

	template<typename FunctorT>
	SizeType RemoveSwapSingleByFunc(FunctorT&& func);

	// Removes `num` elements starting at `idx`, shifting later elements left.
	// * Requires 0 <= idx, idx + num <= GetNum().
	void RemoveAt(SizeType idx, SizeType num = 1) noexcept;

	// Same as RemoveAt, but uses swap-with-last and does not preserve order.
	void RemoveAtSwap(SizeType idx, SizeType num = 1) noexcept;

	// Same as RemoveAt(idx), but returns a copy of the removed element.
	ElementType RemoveAt_GetCopy(SizeType idx) noexcept;

	// Same as RemoveAtSwap(idx), but returns a copy of the removed element.
	ElementType RemoveAtSwap_GetCopy(SizeType idx) noexcept;

	// Removes and returns the last element.
	// * Requires GetNum() > 0; asserts on an empty array, unlike GetLast().
	ElementType Pop() noexcept;

	// Swap
	// Both index ranges must lie within [0, GetNum()).
	// -------------------------------------------------------------------------

	// Swaps the elements at `firstIdx` and `secondIdx`.
	void Swap(SizeType firstIdx, SizeType secondIdx) noexcept;

	// Swaps `num` contiguous elements starting at `firstIdx` with `num` starting at `secondIdx`.
	// * Ranges must not overlap.
	void SwapRange(SizeType firstIdx, SizeType secondIdx, SizeType num = 1) noexcept;

	// Fill / Assign
	// -------------------------------------------------------------------------

	// Overwrites every existing element with a copy of `val`. Num is unchanged.
	void Fill(const ElementType& val) noexcept;

	// Replaces this array's contents with a copy of `other`.
	void Assign(const TArray& other) noexcept;

	// Replaces this array's contents by taking `other`'s.
	// * `other` is left empty.
	void Assign(TArray&& other) noexcept;

	// Replaces the contents with `num` copies of `val`.
	void Assign(const ElementType& val, SizeType num) noexcept;

	// Replaces the contents by copying `num` elements from `data`.
	// * `data` may be null only if `num == 0`.
	void Assign(const ElementType* data, SizeType num) noexcept;

	// Find Index
	// Returns the index of the first match, or KOR_INDEX_NONE.
	// * Never asserts, even on an empty array.
	// -------------------------------------------------------------------------

	// Matches by operator==.
	SizeType FindIndex(const ElementType& val) const noexcept;

	// Matches by predicate; not noexcept, since func may throw.
	template<typename FunctorT>
	SizeType FindIndexByFunc(FunctorT&& func) const;

	// Matches by key.
	template<typename KeyType>
	SizeType FindIndexByKey(const KeyType& key) const noexcept;

	// Find Element
	// Returns a pointer to the first match, or nullptr. Never asserts.
	// -------------------------------------------------------------------------

	// Matches by predicate; not noexcept, since func may throw.
	template<typename FunctorT>
	ElementType* FindByFunc(FunctorT&& func);

	template<typename FunctorT>
	const ElementType* FindByFunc(FunctorT&& func) const;

	// Matches by key.
	template<typename KeyType>
	ElementType* FindByKey(const KeyType& key) noexcept;

	template<typename KeyType>
	const ElementType* FindByKey(const KeyType& key) const noexcept;

	// Contains
	// True if any element matches. Never asserts; false on an empty array.
	// -------------------------------------------------------------------------

	// Matches by operator==.
	bool Contains(const ElementType& val) const noexcept;

	// Matches by predicate; not noexcept, since func may throw.
	template<typename FunctorT>
	bool ContainsByFunc(FunctorT&& func) const;

	// Matches by key.
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