// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/Math.h"
#include "Kor/Memory.h"

#include "Kor/ArrayAllocator.h"

KOR_NAMESPACE_BEGIN

template<typename ElementT, typename AllocatorT>
class TArray
{
public:
	// Types
	/////////////////////////////////

	typedef ElementT ElementType;
	typedef AllocatorT AllocatorType;

	typedef ElementT* ArrayIteratorType;
	typedef const ElementT* ConstArrayIteratorType;

	typedef typename AllocatorT::SizeType SizeType;
	typedef std::initializer_list<ElementT> ElementListType;

	// Asserts
	/////////////////////////////////

	static_assert(!TIsSame<ElementT, void>::Value && !TIsReference<ElementT>::Value, "Element type is not valid");
	static_assert(!TIsSame<AllocatorT, void>::Value && TIsSigned<SizeType>::Value, "Allocator type is not valid");

	// Constructors
	/////////////////////////////////

	KOR_FORCEINLINE TArray() : _allocator(), _num(0) {}
	KOR_FORCEINLINE TArray(const TArray& other) : _allocator(), _num(0) { AppendImpl(other); }
	KOR_FORCEINLINE TArray(TArray&& other) noexcept : _allocator(), _num(0) { ReplaceImpl(Move(other)); }
	KOR_FORCEINLINE TArray(SizeType num, bool reserveOnly = false) : _allocator(), _num(0)
	{
		if (reserveOnly) ReserveImpl(num);
		else GrowImpl(num);
	}
	KOR_FORCEINLINE TArray(const ElementListType& list) : _allocator() , _num(0)
	{
		AppendImpl(list.begin(), list.size());
	}

	KOR_FORCEINLINE TArray(const ElementT* data, SizeType num)
		: _allocator()
		, _num(0)
	{
		AppendImpl(data, num);
	}

	// Destructor
	/////////////////////////////////

	KOR_FORCEINLINE ~TArray() { EmptyImpl(true); }

	// Compare operators
	/////////////////////////////////

	KOR_FORCEINLINE bool operator==(const TArray& other) const { return CompareAllocatorsPrivate(&_allocator, &other._allocator, _num); }
	KOR_FORCEINLINE bool operator!=(const TArray& other) const { return CompareAllocatorsPrivate(&_allocator, &other._allocator, _num); }

	// Assign operators
	/////////////////////////////////

	KOR_FORCEINLINE TArray& operator=(const TArray& other) { EmptyImpl(true); AppendImpl(other); return *this; }
	KOR_FORCEINLINE TArray& operator=(TArray&& other) noexcept { ReplaceImpl(Move(other)); return *this; }

	KOR_FORCEINLINE TArray& operator=(const ElementListType& list) { EmptyImpl(true); AppendImpl(list.begin(), list.size()); return *this; }

	// Get operators
	/////////////////////////////////

	KOR_FORCEINLINE ElementT& operator[](SizeType idx) { return *GetElementAtImpl(idx); }
	KOR_FORCEINLINE const ElementT& operator[](SizeType idx) const { return *GetElementAtImpl(idx); }

	// Property getters
	/////////////////////////////////

	KOR_FORCEINLINE const ElementT* GetData() const { return _allocator.GetData(); }
	KOR_FORCEINLINE ElementT* GetData() { return _allocator.GetData(); }

	KOR_FORCEINLINE SizeType GetNum() const { return _num; }
	KOR_FORCEINLINE SizeType GetReservedNum() const { return _allocator.GetSize(); }

	// Validations
	/////////////////////////////////

	KOR_FORCEINLINE bool IsEmpty() const { return _num == 0; }
	KOR_FORCEINLINE bool IsValidIndex(SizeType idx) const { return idx >= 0 && idx < _num; }

	// Append
	/////////////////////////////////

	KOR_FORCEINLINE void Append(const TArray& other) { AppendImpl(other); }
	KOR_FORCEINLINE void Append(TArray&& other) { AppendImpl(Move(other)); }

	KOR_FORCEINLINE void Append(const ElementT& val, SizeType numToAdd) { AddImpl(val, numToAdd); }
	KOR_FORCEINLINE void Append(const ElementListType& list) { AppendImpl(list.begin(), list.size()); }
	KOR_FORCEINLINE void Append(const ElementT* data, SizeType num) { AppendImpl(data, num); }

	KOR_FORCEINLINE void AppendUninitialized(SizeType numToAdd) { GrowImpl(_num + numToAdd); }

	// Replace
	/////////////////////////////////

	KOR_FORCEINLINE void Replace(TArray& other) { ReplaceImpl(Move(other)); }
	KOR_FORCEINLINE void Replace(TArray&& other) { ReplaceImpl(Move(other)); }

	// Add
	/////////////////////////////////

	KOR_FORCEINLINE void Add(const ElementT& val)
	{
		AppendImpl(&val, 1);
	}

	KOR_FORCEINLINE void Add(ElementT&& val)
	{
		AppendImpl(&val, 1, true);
	}

	KOR_FORCEINLINE void AddDefaulted(SizeType num = 1)
	{
		AddDefaultedImpl(num);
	}

	KOR_FORCEINLINE void AddUninitialized(SizeType num = 1)
	{
		AddUninitializedImpl(num);
	}

	KOR_FORCEINLINE ElementT& Add_GetRef(const ElementT& val)
	{
		AppendImpl(&val, 1);
		return *GetElementAtImpl(_num - 1);
	}

	KOR_FORCEINLINE ElementT& Add_GetRef(ElementT&& val)
	{
		AppendImpl(&val, 1, true);
		return *GetElementAtImpl(_num - 1);
	}

	KOR_FORCEINLINE ElementT& AddDefaulted_GetRef()
	{
		AddDefaultedImpl();
		return *GetElementAtImpl(_num - 1);
	}

	KOR_FORCEINLINE ElementT& AddUninitialized_GetRef()
	{
		AddUninitializedImpl();
		return *GetElementAtImpl(_num - 1);
	}

	KOR_FORCEINLINE void Push(const ElementT& val) { AddImpl(val); }
	KOR_FORCEINLINE void Push(ElementT&& val) { AddImpl(Move(val)); }

	// Add
	/////////////////////////////////

	template<typename... ArgTypes>
	KOR_FORCEINLINE SizeType Emplace(ArgTypes&&... args)
	{
		AddUninitializedImpl();

		SMemory::Construct(GetElementAtImpl(_num - 1), Forward<ArgTypes>(args)...);
		return _num - 1;
	}

	template<typename... ArgTypes>
	KOR_FORCEINLINE ElementT& Emplace_GetRef(ArgTypes&&... args)
	{
		AddUninitializedImpl();

		ElementT* newEl = GetElementAtImpl(_num - 1);
		SMemory::Construct(newEl, Forward<ArgTypes>(args)...);
		return *newEl;
	}

	// Remove
	/////////////////////////////////
	// * Swap is faster version of Remove
	// * but does not preserve order

	// Returns number of removed elements
	int32 Remove(const ElementT& val, bool allowShrink = true)
	{
		int32 removedNum = 0;
		for(SizeType i = _num - 1; i >= 0; --i)
		{
			if(!SMemory::IsEqual(GetElementAtImpl(i), &val)) continue;
			RemoveImpl(i);
			++removedNum;
		}

		if (allowShrink && removedNum > 0) ShrinkIfNeededImpl();
		return removedNum;
	}

	// Returns number of removed elements
	int32 RemoveSwap(const ElementT& val, bool allowShrink = true)
	{
		int32 removedNum = 0;
		for(SizeType i = _num - 1; i >= 0; --i)
		{
			if(!SMemory::IsEqual(GetElementAtImpl(i), &val)) continue;
			RemoveSwapImpl(i);
			++removedNum;
		}

		if (allowShrink && removedNum > 0) ShrinkIfNeededImpl();
		return removedNum;
	}

	// Returns index of removed element or KOR_INDEX_NONE if not found
	KOR_FORCEINLINE int32 RemoveFirst(const ElementT& val)
	{
		const int32 foundIdx = FindIndex(val);
		if (foundIdx != KOR_INDEX_NONE) RemoveImpl(foundIdx);
		return foundIdx;
	}

	// Returns index of removed element or KOR_INDEX_NONE if not found
	KOR_FORCEINLINE int32 RemoveSwapFirst(const ElementT& val)
	{
		const int32 foundIdx = FindIndex(val);
		if (foundIdx != KOR_INDEX_NONE) RemoveSwapImpl(foundIdx);
		return foundIdx;
	}

	KOR_FORCEINLINE void RemoveAt(SizeType idx)
	{
		if(!IsValidIndex(idx)) return;
		RemoveImpl(idx);
	}

	KOR_FORCEINLINE void RemoveAtSwap(SizeType idx)
	{
		if(!IsValidIndex(idx)) return;
		RemoveSwapImpl(idx);
	}

	KOR_FORCEINLINE ElementT RemoveAt_GetCopy(SizeType idx)
	{
		if(!IsValidIndex(idx)) return {};

		// Tries to use move constructor
		ElementT copy(Move(*GetElementAtImpl(idx)));
		RemoveImpl(idx);

		return copy;
	}

	KOR_FORCEINLINE ElementT RemoveAtSwap_GetCopy(SizeType idx)
	{
		if(!IsValidIndex(idx)) return {};

		// Tries to use move constructor
		ElementT copy(Move(*GetElementAtImpl(idx)));
		RemoveSwapImpl(idx);

		return copy;
	}

	KOR_FORCEINLINE void Pop()
	{
		if(_num > 0)
		{
			RemoveImpl(_num - 1);
		}
	}

	// Swap
	/////////////////////////////////

	KOR_FORCEINLINE void Swap(SizeType firstIdx, SizeType secondIdx)
	{
		if(!IsValidIndex(firstIdx) || !IsValidIndex(secondIdx)) return;

		SwapImpl(firstIdx, secondIdx, 1);
	}

	KOR_FORCEINLINE void SwapRange(SizeType firstIdx, SizeType secondIdx, SizeType num = 1)
	{
		if(!IsValidIndex(firstIdx) || !IsValidIndex(secondIdx)) return;
		else if(firstIdx + num > _num || secondIdx + num > _num) return;

		SwapImpl(firstIdx, secondIdx, num);
	}

	// Get
	/////////////////////////////////

	KOR_FORCEINLINE const ElementT* GetAt(SizeType idx) const { return IsValidIndex(idx) ? GetElementAtImpl(idx) : nullptr; }
	KOR_FORCEINLINE ElementT* GetAt(SizeType idx) { return IsValidIndex(idx) ? GetElementAtImpl(idx) : nullptr; }

	KOR_FORCEINLINE const ElementT* GetFirst() const { return _num > 0 ? GetElementAtImpl(0) : nullptr; }
	KOR_FORCEINLINE ElementT* GetFirst() { return _num > 0 ? GetElementAtImpl(0) : nullptr; }

	KOR_FORCEINLINE const ElementT* GetLast() const { return _num > 0 ? GetElementAtImpl(_num - 1) : nullptr; }
	KOR_FORCEINLINE ElementT* GetLast() { return _num > 0 ? GetElementAtImpl(_num - 1) : nullptr; }

	// Find Index
	/////////////////////////////////

	SizeType FindIndex(const ElementT& val) const
	{
		for(SizeType i = 0; i < _num; ++i)
		{
			if(SMemory::IsEqual(GetElementAtImpl(i), &val))
			{
				return i;
			}
		}

		return KOR_INDEX_NONE;
	}

	template<typename Functor>
	SizeType FindIndexByFunc(Functor&& func) const
	{
		for(SizeType i = 0; i < _num; ++i)
		{
			if(func((const ElementT&)*GetElementAtImpl(i)))
			{
				return i;
			}
		}

		return KOR_INDEX_NONE;
	}

	template<typename KeyType>
	SizeType FindIndexByKey(KeyType key) const
	{
		for(SizeType i = 0; i < _num; ++i)
		{
			if(*GetElementAtImpl(i) == key)
			{
				return i;
			}
		}

		return KOR_INDEX_NONE;
	}

	// Find Element
	/////////////////////////////////

	template<typename Functor>
	const ElementT* FindByFunc(Functor&& func) const
	{
		const SizeType foundIdx = FindIndexByFunc(Move(func));
		return foundIdx != KOR_INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename Functor>
	ElementT* FindByFunc(Functor&& func)
	{
		const SizeType foundIdx = FindIndexByFunc(Move(func));
		return foundIdx != KOR_INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	const ElementT* FindByKey(KeyType key) const
	{
		const SizeType foundIdx = FindByKey(key);
		return foundIdx != KOR_INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	ElementT* FindByKey(KeyType key)
	{
		const SizeType foundIdx = FindByKey(key);
		return foundIdx != KOR_INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	// Contains
	/////////////////////////////////

	KOR_FORCEINLINE bool Contains(const ElementT& val) const { return FindIndex(val) != KOR_INDEX_NONE; }

	template<typename Functor>
	KOR_FORCEINLINE bool ContainsByFunc(Functor&& func) const { return !!FindByFunc(Move(func)); }

	template<typename KeyType>
	KOR_FORCEINLINE bool ContainsByKey(KeyType key) const { return FindByKey(key) != KOR_INDEX_NONE; }

	// Other
	/////////////////////////////////

	KOR_FORCEINLINE void ShrinkToFit() { if(_num < _allocator.GetSize()) ShrinkImpl(_num); }

	KOR_FORCEINLINE void Resize(SizeType num) { ResizeImpl(num); }
	KOR_FORCEINLINE void Reserve(SizeType num) { if (num > _num) { ReserveImpl(num); } else { ShrinkImpl(num); }; }

	KOR_FORCEINLINE void Reset() { EmptyImpl(_allocator.GetSize()); }
	KOR_FORCEINLINE void Empty(SizeType newNum = 0) { EmptyImpl(newNum); }

	// Iterators
	/////////////////////////////////

	KOR_FORCEINLINE ArrayIteratorType begin() { return _num > 0 ? GetElementAtImpl(0) : nullptr; }
	KOR_FORCEINLINE ConstArrayIteratorType begin() const { return _num > 0 ? GetElementAtImpl(0) : nullptr; }
	KOR_FORCEINLINE ArrayIteratorType end() { return _num > 0 ? GetElementAtImpl(_num) : nullptr; }
	KOR_FORCEINLINE ConstArrayIteratorType end() const { return _num > 0 ? GetElementAtImpl(_num) : nullptr; }

private:

	KOR_FORCEINLINE ElementT* GetElementAtImpl(SizeType idx) const { return _allocator.GetData() + idx; }

	void AddDefaultedImpl(SizeType num = 1)
	{
		if(num <= 0) return;

		_num += num;
		GrowIfNeededImpl();

		SMemory::ZeroTyped(GetElementAtImpl(_num - num), num);
	}

	KOR_FORCEINLINE void AddUninitializedImpl(SizeType num = 1)
	{
		if(num <= 0) return;

		_num += num;
		GrowIfNeededImpl();
	}

	void RemoveSwapImpl(SizeType idx)
	{
		DestructElementsPrivate(GetElementAtImpl(idx));

		if(idx != _num - 1)
		{
			// Swaps last element with this
			SMemory::Move(
				GetElementAtImpl(idx),
				GetElementAtImpl(_num - 1),
				sizeof(ElementT)
			);
		}

		--_num;
	}

	void RemoveImpl(SizeType idx)
	{
		DestructElementsPrivate(GetElementAtImpl(idx));

		if(idx != _num - 1)
		{
			// Moves entire allocation by one index down

			// NOTE(jan.kristian.fisera):
			// * Is it worth to cache start index and try to move from start in case that would be fewer iterations ?
			SMemory::Move(
				GetElementAtImpl(idx),
				GetElementAtImpl(idx + 1),
				sizeof(ElementT) * (_num - idx - 1)
			);
		}

		--_num;
	}

	void SwapImpl(SizeType firstIdx, SizeType secondIdx, SizeType num)
	{
		// Copy to temporary storage
		AllocatorT tmp;
		tmp.Allocate(num);
		SMemory::CopyTyped(
			tmp.GetData(),
			GetElementAtImpl(firstIdx),
			sizeof(ElementT) * num
		);

		// Do swap to first index
		// * elements from second idx to first
		SMemory::CopyTyped(
			GetElementAtImpl(firstIdx),
			GetElementAtImpl(secondIdx),
			sizeof(ElementT) * num
		);

		// Do swap to second index
		// * copied elements from first idx to second
		SMemory::CopyTyped(
			GetElementAtImpl(secondIdx),
			tmp.GetData(),
			sizeof(ElementT) * num
		);
	}

	void ShrinkImpl(SizeType num)
	{
		if (_allocator.GetSize() == num)
		{
			// No Shrink
			return;
		}

		if (_num > num)
		{
			// Destruct what needs to be destructed
			DestructElementsPrivate(GetElementAtImpl(num), _num - num);
			_num = num;
		}

		// Copy to temporary allocator
		AllocatorT tmp;
		tmp.Allocate(num);
		SMemory::CopyTyped(
			tmp.GetData(),
			_allocator.GetData(),
			num
		);

		// Move data back to main allocator
		_allocator.Release();
		_allocator.Allocate(num);
		SMemory::CopyTyped(
			_allocator.GetData(),
			tmp.GetData(),
			num
		);

		tmp.SetData(nullptr);
		tmp.SetSize(0);
	}

	KOR_FORCEINLINE void GrowImpl(SizeType num)
	{
		_num = num;
		GrowIfNeededImpl();
	}

	KOR_FORCEINLINE void ReserveImpl(SizeType num)
	{
		_allocator.Allocate(num - _allocator.GetSize());
	}

	void EmptyImpl(SizeType newNum)
	{
		if (_num > 0)
		{
			DestructElementsPrivate(_allocator.GetData(), _num);
			_num = 0;
		}

		if (newNum > _allocator.GetSize())
		{
			ReserveImpl(newNum);
		}
		else if (newNum < _allocator.GetSize())
		{
			if (newNum == 0)
			{
				_allocator.Release();
			}
			else
			{
				ShrinkImpl(newNum);
			}
		}
	}

	void AppendImpl(const ElementT* data, SizeType num)
	{
		if(num > 0)
		{
			const SizeType oldCount = _num;

			_num += num;
			GrowIfNeededImpl();
			SMemory::CopyTyped(_allocator.GetData() + oldCount, data, num);
		}
	}

	void AppendImpl(ElementT* data, SizeType num, bool preferMove)
	{
		if(preferMove && _num == 1)
		{
			const SizeType oldCount = _num;

			_num += num;
			GrowIfNeededImpl();

			SMemory::MoveTyped(_allocator.GetData() + oldCount, data);
		}
		else
		{
			AppendImpl(data, num);
		}
	}

	void AppendImpl(TArray&& other)
	{
		AppendImpl(other.GetData(), other._num, true);
		other._allocator.Release();
		other._num = 0;
	}

	KOR_FORCEINLINE void AppendImpl(const TArray& other) { AppendImpl(other.GetData(), other._num); }

	void ReplaceImpl(TArray&& other)
	{
		EmptyImpl(true);

		_allocator.SetData(other._allocator.GetData());
		_allocator.SetSize(other._allocator.GetSize());
		_num = other._num;

		other._allocator.SetData(nullptr);
		other._allocator.SetSize(0);
		other._num = 0;
	}

	KOR_FORCEINLINE void ResizeImpl(SizeType num)
	{
		if(num > _num) GrowImpl(num);
		else if(num < _num) ShrinkImpl(num);
	}

	void ShrinkIfNeededImpl()
	{
		const SizeType reserved = _allocator.GetSize();
		const SizeType preferred = SMath::CeilToPowerOfTwo((uint64)_num);
		if (preferred < reserved)
		{
			ShrinkImpl(preferred);
		}
	}

	void GrowIfNeededImpl()
	{
		const SizeType reserved = _allocator.GetSize();
		if(_num > reserved)
		{
			ReserveImpl(SMath::CeilToPowerOfTwo((uint64)_num));
		}
	}

	KOR_FORCEINLINE static void DestructElementsPrivate(ElementT* element, SizeType num = 1)
	{
		for(SizeType i = 0; i < num; ++i)
		{
			SMemory::Destruct(element);
			++element;
		}
	}

	KOR_FORCEINLINE static bool CompareAllocatorsPrivate(const AllocatorT* lhs, const AllocatorT* rhs, SizeType size)
	{
		return (lhs->GetSize() >= size && rhs->GetSize() >= size) ?
			SMemory::IsEqual(lhs->GetData(), rhs->GetData(), size) : false;
	}

	AllocatorT _allocator = {};
	SizeType _num = KOR_INDEX_NONE;
};

template<typename ElementT, typename AllocatorT>
struct TContainerTypeTraits<TArray<ElementT, AllocatorT>> : public TContainerTypeTraits<void>
{
	using ElementType = ElementT;
	using AllocatorType = AllocatorT;

	enum
	{
		IsContainer = true,
		IsDynamic = true,
		InlineMemory = TIsSame<AllocatorT, TArrayAllocator<ElementT>>::Value
	};
};

KOR_NAMESPACE_END