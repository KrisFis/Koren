// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/Math.h"
#include "Kor/MemoryOps.h"

#include "Kor/ArrayAllocator.h"

KOR_NAMESPACE_BEGIN

template<typename ElementT, typename AllocatorT>
class TArray
{
public:
	// Types
	// -------------------------------------------------------------------------

	using ElementType = ElementT;
	using AllocatorType = AllocatorT;

	using ArrayIteratorType = ElementType*;
	using ConstArrayIteratorType = const ElementType*;

	using SizeType = typename AllocatorType::SizeType;
	using ILType = std::initializer_list<ElementType>;

	// Asserts
	// -------------------------------------------------------------------------

	static_assert(!TIsSame<ElementType, void>::Value && !TIsReference<ElementType>::Value, "ElementType must be non-reference/non-void type");
	static_assert(!TIsSame<AllocatorType, void>::Value && TIsSigned<SizeType>::Value, "AllocatorType must array allocator type");

	// Constructors
	// -------------------------------------------------------------------------

	constexpr TArray() noexcept;
	constexpr TArray(Init::SNoInit);
	constexpr TArray(Init::SZero);

	TArray(const TArray& other) noexcept;
	TArray(TArray&& other) noexcept;

	TArray(SizeType num, bool reserveOnly = false) noexcept;
	TArray(const ILType& list) noexcept;
	TArray(const ElementType* data, SizeType num) noexcept;

	// Destructor
	// -------------------------------------------------------------------------

	~TArray();

	// Compare operators
	// -------------------------------------------------------------------------

	bool operator==(const TArray& other) const;
	bool operator!=(const TArray& other) const;

	// Assign operators
	// -------------------------------------------------------------------------

	TArray& operator=(const TArray& other);
	TArray& operator=(TArray&& other) noexcept;

	TArray& operator=(const ILType& list);

	// Dereference operators
	// -------------------------------------------------------------------------

	const ElementType* operator*() const noexcept;
	ElementType* operator*() noexcept;

	// Get operators
	// -------------------------------------------------------------------------

	ElementType& operator[](SizeType idx);
	const ElementType& operator[](SizeType idx) const;

	// Property getters
	// -------------------------------------------------------------------------

	const ElementType* GetData() const;
	ElementType* GetData();

	SizeType GetNum() const;
	SizeType GetReservedNum() const;

	// Validations
	// -------------------------------------------------------------------------

	bool IsEmpty() const;
	bool IsValidIndex(SizeType idx) const;

	// Append
	// -------------------------------------------------------------------------

	void Append(const TArray& other);
	void Append(TArray&& other);

	void Append(const ElementType& val, SizeType numToAdd);
	void Append(const ILType& list);
	void Append(const ElementType* data, SizeType num);

	void AppendUninitialized(SizeType numToAdd);

	// Replace
	// -------------------------------------------------------------------------

	void Replace(TArray& other);
	void Replace(TArray&& other);

	// Add
	// -------------------------------------------------------------------------

	void Add(const ElementType& val);
	void Add(ElementType&& val);
	void AddDefaulted(SizeType num = 1);
	void AddUninitialized(SizeType num = 1);

	ElementType& Add_GetRef(const ElementType& val);
	ElementType& Add_GetRef(ElementType&& val);
	ElementType& AddDefaulted_GetRef();
	ElementType& AddUninitialized_GetRef();

	void Push(const ElementType& val);
	void Push(ElementType&& val);

	// Add
	// -------------------------------------------------------------------------

	template<typename... ArgTypes>
	SizeType Emplace(ArgTypes&&... args);

	template<typename... ArgTypes>
	ElementType& Emplace_GetRef(ArgTypes&&... args);

	// Remove
	// -------------------------------------------------------------------------
	// * Swap is faster version of Remove
	// * but does not preserve order

	// Returns number of removed elements
	int32 Remove(const ElementType& val, bool allowShrink = true);

	// Returns number of removed elements
	int32 RemoveSwap(const ElementType& val, bool allowShrink = true);

	// Returns index of removed element or KOR_INDEX_NONE if not found
	int32 RemoveFirst(const ElementType& val);

	// Returns index of removed element or KOR_INDEX_NONE if not found
	int32 RemoveSwapFirst(const ElementType& val);
	void RemoveAt(SizeType idx);
	void RemoveAt(SizeType idx, SizeType count);
	void RemoveAtSwap(SizeType idx);
	ElementType RemoveAt_GetCopy(SizeType idx);
	ElementType RemoveAtSwap_GetCopy(SizeType idx);
	void Insert(SizeType idx, const ElementType* data, SizeType num);
	void Pop();

	// Swap
	// -------------------------------------------------------------------------

	void Swap(SizeType firstIdx, SizeType secondIdx);
	void SwapRange(SizeType firstIdx, SizeType secondIdx, SizeType num = 1);

	// Get
	// -------------------------------------------------------------------------

	const ElementType* GetAt(SizeType idx) const;
	ElementType* GetAt(SizeType idx);

	const ElementType* GetFirst() const;
	ElementType* GetFirst();

	const ElementType* GetLast() const;
	ElementType* GetLast();

	// Find Index
	// -------------------------------------------------------------------------

	SizeType FindIndex(const ElementType& val) const;

	template<typename Functor>
	SizeType FindIndexByFunc(Functor&& func) const;

	template<typename KeyType>
	SizeType FindIndexByKey(KeyType key) const;

	// Find Element
	// -------------------------------------------------------------------------

	template<typename Functor>
	const ElementType* FindByFunc(Functor&& func) const;

	template<typename Functor>
	ElementType* FindByFunc(Functor&& func);

	template<typename KeyType>
	const ElementType* FindByKey(KeyType key) const;

	template<typename KeyType>
	ElementType* FindByKey(KeyType key);

	// Contains
	// -------------------------------------------------------------------------

	bool Contains(const ElementType& val) const;

	template<typename Functor>
	bool ContainsByFunc(Functor&& func) const;

	template<typename KeyType>
	bool ContainsByKey(KeyType key) const;

	// Other
	// -------------------------------------------------------------------------

	void ShrinkToFit();

	void SetNum(SizeType num);
	void Resize(SizeType num);
	void Reserve(SizeType num);

	void Reset();
	void Empty(SizeType newNum = 0);

	// Iterators
	// -------------------------------------------------------------------------

	ArrayIteratorType begin();
	ConstArrayIteratorType begin() const;
	ArrayIteratorType end();
	ConstArrayIteratorType end() const;

private:
	AllocatorType _allocator = {};
	SizeType _num = 0;
};

template<typename ElementT, typename AllocatorT>
struct TContainerTraits<TArray<ElementT, AllocatorT>> : public TContainerTraits<void>
{
	using ElementType = ElementType;
	using AllocatorType = AllocatorType;

	enum
	{
		IsDynamic = true,
		InlineMemory = TIsSame<AllocatorType, TArrayAllocator<ElementType>>::Value
	};
};

KOR_NAMESPACE_END