// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Container/Minimal.h"
#include "Kor/Container/ContainerTraits.h"

#include "Kor/Math/MathOps.h"

#include "Kor/Memory/Allocator.h"
#include "Kor/Memory/AllocatorOps.h"
#include "Kor/Memory/AllocatorTraits.h"
#include "Kor/Memory/MemoryOps.h"

#include "Kor/Utility/Swap.h"

KOR_NAMESPACE_BEGIN

// Forward declare for private bit array friend
namespace Detail::BitArray { template<typename T> struct TFriend; }

template<typename WordT>
struct TBitReference
{
	static_assert(TIsInteger<WordT>::Value && TIsUnsigned<WordT>::Value,
		"WordT must be an unsigned integer type"
	);

	using WordType = WordT;

	KOR_FORCEINLINE explicit constexpr TBitReference(WordType& data, WordType mask) noexcept : _data(data), _mask(mask) {}

	KOR_FORCEINLINE constexpr operator bool() const noexcept { return Get(); }
	KOR_FORCEINLINE constexpr void operator=(bool value) noexcept { return Set(value); }
	KOR_FORCEINLINE constexpr void operator|=(bool value) const noexcept { if (value) { _data |= _mask; } }
	KOR_FORCEINLINE constexpr void operator&=(bool value) const noexcept { if (!value) { _data &= ~_mask; } }

	KOR_FORCEINLINE constexpr bool Get() const noexcept { return (_data & _mask) != 0; }
	KOR_FORCEINLINE constexpr void Set(bool value) noexcept { if (value) { _data |= _mask; } else { _data &= ~_mask; } }

private:
	WordType& _data;
	WordType _mask;
};

template<typename WordT>
struct TConstBitReference
{
	static_assert(TIsInteger<WordT>::Value && TIsUnsigned<WordT>::Value,
		"WordT must be an unsigned integer type"
	);

	using WordType = WordT;

	KOR_FORCEINLINE explicit constexpr TConstBitReference(const WordType& data, WordType mask) noexcept : _data(data), _mask(mask) {}

	KOR_FORCEINLINE constexpr operator bool() const noexcept { return Get(); }
	KOR_FORCEINLINE constexpr bool Get() const noexcept { return (_data & _mask) != 0; }

private:
	const WordType& _data;
	WordType _mask;
};

// [ TBitArray ]
// A dynamically-sized, heap-allocated packed bit container.
// * Stores boolean values as bits packed into uint32 blocks
// * WordType must be an unsigned integer type
//
// Example:
//   TBitArray<> flags;
//   flags.Add(true);
//   flags.Add(false);
//   flags.SetBit(1, true);
//
template<typename WordT, typename AllocatorFamilyT>
class TBitArray
{
	static_assert(TIsInteger<WordT>::Value && TIsUnsigned<WordT>::Value,
		"WordT must be an unsigned integer type"
	);

	using AllocatorFamilyTraits = TAllocatorFamilyTraits<AllocatorFamilyT>;

public:
	using WordType = WordT;
	using AllocatorFamilyType = AllocatorFamilyT;

	using SizeType = typename AllocatorFamilyTraits::SizeType;
	using AllocatorType = AllocatorFamilyTraits::template TypedAllocator<uint32>::Type;

	using ReferenceType = TBitReference<WordType>;
	using ConstReferenceType = TConstBitReference<WordType>;

	// Constructors
	// -------------------------------------------------------------------------

	// Empty bit array, no allocation.
	constexpr TBitArray() noexcept;
	explicit constexpr TBitArray(AllocatorType&& allocator) noexcept;

	// Leaves Detail state uninitialized.
	// * Caller must bring it to a valid state before any other use.
	explicit consteval TBitArray(Init::SConstEval) noexcept;

	// Deep copy.
	// * Capacity becomes exactly Num of `other`; slack is not preserved.
	TBitArray(const TBitArray& other) noexcept;

	// Steals buffer/Num/capacity from `other`.
	// * `other` is left empty.
	constexpr TBitArray(TBitArray&& other) noexcept;

	// Reserves `num` bits and initializes them to `value`. Num becomes `num`.
	explicit TBitArray(SizeType num, bool value = false) noexcept;
	explicit TBitArray(AllocatorType&& allocator, SizeType num, bool value = false) noexcept;

	// Destructor
	// -------------------------------------------------------------------------

	~TBitArray() noexcept;

	// Assignment Operators
	// -------------------------------------------------------------------------

	TBitArray& operator=(const TBitArray& other) noexcept;
	TBitArray& operator=(TBitArray&& other) noexcept;

	// Comparison Operators
	// -------------------------------------------------------------------------

	bool operator==(const TBitArray& other) const noexcept;
	bool operator!=(const TBitArray& other) const noexcept;

	// Index Operators
	// -------------------------------------------------------------------------

	ReferenceType operator[](SizeType idx) noexcept;
	ConstReferenceType operator[](SizeType idx) const noexcept;

	// Property Getters
	// -------------------------------------------------------------------------

	AllocatorType& GetAllocator() noexcept;
	const AllocatorType& GetAllocator() const noexcept;

	// Raw packed word buffer. Null if empty and never allocated.
	WordType* GetData() noexcept;
	const WordType* GetData() const noexcept;

	// Number of stored bits.
	SizeType GetNum() const noexcept;

	// Number of bits storable before the next reallocation.
	SizeType GetReservedNum() const noexcept;

	// Validation
	// -------------------------------------------------------------------------

	bool IsEmpty() const noexcept;
	bool IsValidIndex(SizeType idx) const noexcept;

	// Get
	// -------------------------------------------------------------------------

	ReferenceType GetAt(SizeType idx) noexcept;
	ConstReferenceType GetAt(SizeType idx) const noexcept;

	// Reserve / SetNum / Reset
	// -------------------------------------------------------------------------

	void Reserve(SizeType num) noexcept;
	void ReserveExact(SizeType num) noexcept;

	void SetNum(SizeType num, bool value = false) noexcept;

	void ShrinkToFit() noexcept;

	void Reset() noexcept;
	void Empty(SizeType num = 0) noexcept;

	// Add
	// -------------------------------------------------------------------------

	SizeType Add(bool value) noexcept;
	SizeType Add(bool value, SizeType num) noexcept;

	// Remove
	// -------------------------------------------------------------------------

	void RemoveAt(SizeType idx, SizeType num = 1) noexcept;
	void RemoveAtSwap(SizeType idx, SizeType num = 1) noexcept;

	bool Pop() noexcept;

	// Fill / Assign
	// -------------------------------------------------------------------------

	void Fill(bool value) noexcept;

	void Assign(const TBitArray& other) noexcept;
	void Assign(TBitArray&& other) noexcept;

	// Query
	// -------------------------------------------------------------------------

	SizeType FindIndex(bool value) const noexcept;
	bool Contains(bool value) const noexcept;

	// Iterators
	// -------------------------------------------------------------------------

	// IteratorType begin() noexcept;
	// ConstIteratorType begin() const noexcept;
	// IteratorType end() noexcept;
	// ConstIteratorType end() const noexcept;

private:
	// Allocator instance for _data
	AllocatorType _allocator;

	// Allocated packed words
	WordType* _data;

	// Number of initialized bits
	SizeType _num;

	// Number of reserved bits
	SizeType _reservedNum;

	// Friend
	// -------------------------------------------------------------------------

	using SFriend = Detail::BitArray::TFriend<TBitArray>;
	friend SFriend;
};

#include "Kor/Container/Detail/BitArray.inl"

// Convenience type for default bit array
using CBitArray = TBitArray<>;

// [ Is TBitArray ]
// Checks if type is TBitArray type

template<typename T> struct TIsTBitArray : TFalseValue {};
template<typename WordT, typename AllocatorFamilyT> struct TIsTBitArray<TBitArray<WordT, AllocatorFamilyT>> : TTrueValue {};

KOR_NAMESPACE_END