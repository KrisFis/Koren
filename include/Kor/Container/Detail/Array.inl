// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::AllocatorType& TArray<ElementT, AllocatorFamilyT>::GetAllocator() noexcept
{
	return _storage.Allocator;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const TArray<ElementT, AllocatorFamilyT>::AllocatorType& TArray<ElementT, AllocatorFamilyT>::GetAllocator() const noexcept
{
	return _storage.Allocator;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::GetData() noexcept
{
	return _storage.Data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::GetData() const noexcept
{
	return _storage.Data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::GetNum() const noexcept
{
	return _storage.Num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::GetReservedNum() const noexcept
{
	return _storage.ReservedNum;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::IsEmpty() const noexcept
{
	return _storage.Num == 0;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::IsValidIndex(SizeType idx) const noexcept
{
	return SMathOps::IsWithin(idx, (SizeType)0, _storage.Num - 1);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::View() noexcept
{
	return TArrayView<ElementType, SizeType>(_storage.Data, _storage.Num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::View() const noexcept
{
	return TConstArrayView<ElementType, SizeType>(_storage.Data, _storage.Num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::SubView(SizeType idx, SizeType num) noexcept
{
	KOR_ASSERT(idx + num <= _storage.Num);
	return TArrayView<ElementType, SizeType>(_storage.Data + idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::SubView(SizeType idx, SizeType num) const noexcept
{
	KOR_ASSERT(idx + num <= _storage.Num);
	return TConstArrayView<ElementType, SizeType>(_storage.Data + idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::LeftView(SizeType num) noexcept
{
	KOR_ASSERT(num <= _storage.Num);
	return TArrayView<ElementType, SizeType>(_storage.Data, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::LeftView(SizeType num) const noexcept
{
	KOR_ASSERT(num <= _storage.Num);
	return TConstArrayView<ElementType, SizeType>(_storage.Data, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::RightView(SizeType num) noexcept
{
	KOR_ASSERT(num <= _storage.Num);
	return TArrayView<ElementType, SizeType>(_storage.Data + (_storage.Num - num), num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::RightView(SizeType num) const noexcept
{
	KOR_ASSERT(num <= _storage.Num);
	return TConstArrayView<ElementType, SizeType>(_storage.Data + (_storage.Num - num), num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorFamilyT>::TArray() noexcept
	: _storage()
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator) noexcept
	: _storage(Move(allocator))
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE consteval TArray<ElementT, AllocatorFamilyT>::TArray(Init::SConstEval) noexcept
	: _storage(Init::ConstEval)
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const TArray& other) noexcept
	: TArray()
{
	if (other._storage.Num == 0) return;
	_storage.template CopyFromOther<false>(other._storage);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(TArray&& other) noexcept
	: TArray()
{
	if (other._storage.Num == 0) return;
	_storage.template MoveFromOther<false>(Move(other._storage));
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const ILType& list) noexcept
	: TArray()
{
	const SizeType num = (SizeType)list.size();
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::CopyConstruct(_storage.Data, list.begin(), num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ILType& list) noexcept
	: TArray(Move(allocator))
{
	const SizeType num = (SizeType)list.size();
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::CopyConstruct(_storage.Data, list.begin(), num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(SizeType num, Init::SNoInit) noexcept
	: TArray()
{
	if (num <= 0) return;

	_storage.template Reallocate<false>(num);
	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, SizeType num, Init::SNoInit) noexcept
	: TArray(Move(allocator))
{
	if (num <= 0) return;

	_storage.template Reallocate<false>(num);
	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(SizeType num, Init::SDefault) noexcept
	: TArray()
{
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::DefaultConstruct(_storage.Data, num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, SizeType num, Init::SDefault) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::DefaultConstruct(_storage.Data, num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(SizeType num, Init::SZero) noexcept
	: TArray()
{
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::ZeroConstruct(_storage.Data, num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, SizeType num, Init::SZero) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::ZeroConstruct(_storage.Data, num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const ElementType* data, SizeType num) noexcept
	: TArray()
{
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::CopyConstruct(_storage.Data, data, num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ElementType* data, SizeType num) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::CopyConstruct(_storage.Data, data, num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
template<TSize N>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const ElementType(& data)[N]) noexcept
	: TArray(static_cast<const ElementType*>(data), N)
{}

template<typename ElementT, typename AllocatorFamilyT>
template<TSize N>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ElementType(& data)[N]) noexcept
	: TArray(Move(allocator), static_cast<const ElementType*>(data), N)
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const ElementType& val, SizeType num) noexcept
	: TArray()
{
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::FillConstruct(_storage.Data, val, num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ElementType& val, SizeType num) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	_storage.template Reallocate<false>(num);
	SMemoryOps::FillConstruct(_storage.Data, val, num);

	_storage.Num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::~TArray() noexcept
{
	if (_storage.ReservedNum == 0) return;
	_storage.Deallocate();
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE_DEBUG ElementT* TArray<ElementT, AllocatorFamilyT>::GetAt(SizeType idx) noexcept
{
	return IsValidIndex(idx) ? _storage.Data + idx : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE_DEBUG const ElementT* TArray<ElementT, AllocatorFamilyT>::GetAt(SizeType idx) const noexcept
{
	return IsValidIndex(idx) ? _storage.Data + idx : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::GetFirst() noexcept
{
	return _storage.Num > 0 ? _storage.Data : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::GetFirst() const noexcept
{
	return _storage.Num > 0 ? _storage.Data : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::GetLast() noexcept
{
	return _storage.Num > 0 ? _storage.Data + (_storage.Num - 1) : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::GetLast() const noexcept
{
	return _storage.Num > 0 ? _storage.Data + (_storage.Num - 1) : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::FindIndex(const KeyType& key) const noexcept
{
	return _storage.FindIndexBy(
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename PredT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::FindIndexBy(PredT&& pred) const
{
	return _storage.FindIndexBy(Forward<PredT>(pred));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::Find(const KeyType& key) noexcept
{
	return _storage.FindBy(
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::Find(const KeyType& key) const noexcept
{
	return _storage.FindBy(
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename PredT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::FindBy(PredT&& pred)
{
	return _storage.FindBy(Forward<PredT>(pred));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename PredT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::FindBy(PredT&& pred) const
{
	return _storage.FindBy(Forward<PredT>(pred));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::Contains(const KeyType& key) const noexcept
{
	return !!Find(key);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename PredT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::ContainsBy(PredT&& pred) const
{
	return !!FindBy(Forward<PredT>(pred));
}
template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Reserve(SizeType num) noexcept
{
	if (num <= _storage.ReservedNum) return;
	_storage.Grow(num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::ReserveExact(SizeType num) noexcept
{
	if (num <= _storage.ReservedNum) return;
	_storage.Reallocate(num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SetNum(SizeType num) noexcept
{
	if (num > _storage.Num)
	{
		if (num > _storage.ReservedNum) _storage.Grow(num);
		SMemoryOps::DefaultConstruct(_storage.Data + _storage.Num, num - _storage.Num);
		_storage.Num = num;
	}
	else if (num < _storage.Num)
	{
		SMemoryOps::Destruct(_storage.Data + num, _storage.Num - num);
		_storage.Num = num;
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SetNumZeroed(SizeType num) noexcept
{
	if (num > _storage.Num)
	{
		if (num > _storage.ReservedNum) _storage.Grow(num);
		SMemoryOps::ZeroConstruct(_storage.Data + _storage.Num, num - _storage.Num);
		_storage.Num = num;
	}
	else if (num < _storage.Num)
	{
		SMemoryOps::Destruct(_storage.Data + num, _storage.Num - num);
		_storage.Num = num;
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SetNumUninitialized(SizeType num) noexcept
{
	if (num > _storage.Num)
	{
		if (num > _storage.ReservedNum) _storage.Grow(num);
		_storage.Num = num;
	}
	else if (num < _storage.Num)
	{
		SMemoryOps::Destruct(_storage.Data + num, _storage.Num - num);
		_storage.Num = num;
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::ShrinkToFit() noexcept
{
	if (_storage.Num == _storage.ReservedNum) return;
	_storage.Reallocate(_storage.Num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Reset() noexcept
{
	if (_storage.Num == 0) return;

	SMemoryOps::Destruct(_storage.Data, _storage.Num);
	_storage.Num = 0;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Empty(SizeType num) noexcept
{
	if (_storage.Num == 0) return;

	if (num > 0)
	{
		_storage.ResetAndReallocate(num);
	}
	else if (_storage.ReservedNum > 0)
	{
		_storage.Deallocate();
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Fill(const ElementType& val) noexcept
{
	if (_storage.Num == 0) return;
	SMemoryOps::FillAssign(_storage.Data, val, _storage.Num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Assign(const TArray& other) noexcept
{
	if (this == &other) return;
	_storage.CopyFromOther(other._storage);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Assign(TArray&& other) noexcept
{
	if (this == &other) return;
	_storage.MoveFromOther(Move(other._storage));
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Assign(const ElementType& val, SizeType num) noexcept
{
	if (num > 0)
	{
		_storage.ResetAndReallocate(num);
		SMemoryOps::FillConstruct(_storage.Data, val, num);
		_storage.Num = num;
	}
	else if (_storage.ReservedNum > 0)
	{
		_storage.Deallocate();
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Assign(const ElementType* data, SizeType num) noexcept
{
	if (num > 0)
	{
		KOR_ASSERT(data);

		_storage.ResetAndReallocate(num);
		SMemoryOps::CopyConstruct(_storage.Data, data, num);
		_storage.Num = num;
	}
	else if (_storage.ReservedNum > 0)
	{
		_storage.Deallocate();
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Add(const ElementType& val) noexcept
{
	const SizeType idx = _storage.Num;
	_storage.Add();
	SMemoryOps::CopyConstruct(_storage.Data + idx, &val);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Add(ElementType&& val) noexcept
{
	const SizeType idx = _storage.Num;
	_storage.Add();
	SMemoryOps::MoveConstruct(_storage.Data + (_storage.Num - 1), &val);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::Add_GetRef(const ElementType& val) noexcept
{
	const SizeType idx = Add(val);
	return _storage.Data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::Add_GetRef(ElementType&& val) noexcept
{
	const SizeType idx = Add(Move(val));
	return _storage.Data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::AddUnique(const ElementType& val) noexcept
{
	if (const int32 foundIdx = FindIndex(val); foundIdx != KOR_INDEX_NONE)
	{
		return foundIdx;
	}

	return Add(val);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::AddUnique(ElementType&& val) noexcept
{
	if (const int32 foundIdx = FindIndex(val); foundIdx != KOR_INDEX_NONE)
	{
		return foundIdx;
	}

	return Add(Move(val));
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::AddUnique_GetRef(const ElementType& val) noexcept
{
	const SizeType idx = AddUnique(val);
	return _storage.Data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::AddUnique_GetRef(ElementType&& val) noexcept
{
	const SizeType idx = AddUnique(Move(val));
	return _storage.Data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::AddDefaulted(SizeType num) noexcept
{
	const SizeType idx = _storage.Num;
	_storage.Add(num);
	SMemoryOps::DefaultConstruct(_storage.Data + idx, num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::AddDefaulted_GetRef() noexcept
{
	const SizeType idx = AddDefaulted();
	return _storage.Data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::AddZeroed(SizeType num) noexcept
{
	const SizeType idx = _storage.Num;
	_storage.Add(num);
	SMemoryOps::ZeroConstruct(_storage.Data + idx, num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::AddZeroed_GetRef() noexcept
{
	const SizeType idx = AddZeroed();
	return _storage.Data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::AddUninitialized(SizeType num) noexcept
{
	const SizeType idx = _storage.Num;
	_storage.Add(num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::AddUninitialized_GetRef() noexcept
{
	const SizeType idx = AddUninitialized();
	return _storage.Data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Push(const ElementType& val) noexcept
{
	Add(val);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Push(ElementType&& val) noexcept
{
	Add(Move(val));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename ... ArgTypes>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Emplace(ArgTypes&&... args) noexcept
{
	const SizeType idx = _storage.Num;
	_storage.Add();
	SMemoryOps::Construct(_storage.Data + idx, Forward<ArgTypes>(args)...);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename ... ArgTypes>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::Emplace_GetRef(ArgTypes&&... args) noexcept
{
	const SizeType idx = Emplace(Forward<ArgTypes>(args)...);
	return _storage.Data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Insert(SizeType idx, const ElementType& val) noexcept
{
	_storage.Insert(idx);
	SMemoryOps::CopyConstruct(_storage.Data + idx, &val);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Insert(SizeType idx, ElementType&& val) noexcept
{
	_storage.Insert(idx);
	SMemoryOps::MoveConstruct(_storage.Data + idx, &val);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Insert(SizeType idx, const ElementType* data, SizeType num) noexcept
{
	if (num == 0) return;

	_storage.Insert(idx, num);
	SMemoryOps::CopyConstruct(_storage.Data + idx, data, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Insert(SizeType idx, const ILType& list) noexcept
{
	Insert(idx, list.begin(), list.size());
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(const TArray& other) noexcept
{
	if (other._storage.Num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _storage.Num;
	_storage.AppendFromOther(other._storage);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(TArray&& other) noexcept
{
	if (other._storage.Num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _storage.Num;
	_storage.AppendFromOther(other._storage);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(const ILType& list) noexcept
{
	const SizeType num = (SizeType)list.size();
	if (num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _storage.Num;
	_storage.Add(num);
	SMemoryOps::CopyConstruct(_storage.Data + idx, list.begin(), num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(const ElementType& val, SizeType num) noexcept
{
	if (num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _storage.Num;
	_storage.Add(num);
	SMemoryOps::FillConstruct(_storage.Data + idx, val, num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(const ElementType* data, SizeType num) noexcept
{
	if (num == 0) return KOR_INDEX_NONE;

	KOR_ASSERT(data);

	const SizeType idx = _storage.Num;
	_storage.Add(num);
	SMemoryOps::CopyConstruct(_storage.Data + idx, data, num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Remove(const ElementType& val) noexcept
{
	return _storage.RemoveBy(
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &val);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSingle(const ElementType& val) noexcept
{
	const SizeType idx = _storage.FindIndexBy(
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &val);
		}
	);

	if (idx == KOR_INDEX_NONE) return 0;

	_storage.RemoveAt(idx);
	return 1;
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename PredT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveBy(PredT&& pred)
{
	return _storage.RemoveBy(Forward<PredT>(pred));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename PredT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSingleBy(PredT&& pred)
{
	const SizeType idx = _storage.FindIndexBy(Forward<PredT>(pred));
	if (idx == KOR_INDEX_NONE) return 0;

	_storage.RemoveAt(idx);
	return 1;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSwap(const ElementType& val) noexcept
{
	return _storage.RemoveSwapBy(
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &val);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSwapSingle(const ElementType& val) noexcept
{
	const SizeType idx = _storage.FindIndexBy(
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &val);
		}
	);

	if (idx == KOR_INDEX_NONE) return 0;

	_storage.RemoveAtSwap(idx);
	return 1;
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename PredT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSwapBy(PredT&& pred)
{
	return _storage.RemoveSwapBy(Forward<PredT>(pred));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename PredT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSwapSingleBy(PredT&& pred)
{
	const SizeType idx = _storage.FindIndexBy(Forward<PredT>(pred));
	if (idx == KOR_INDEX_NONE) return 0;

	_storage.RemoveAtSwap(idx);
	return 1;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::RemoveAt(SizeType idx, SizeType num) noexcept
{
	_storage.RemoveAt(idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::RemoveAtSwap(SizeType idx, SizeType num) noexcept
{
	_storage.RemoveAtSwap(idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT TArray<ElementT, AllocatorFamilyT>::RemoveAt_GetCopy(SizeType idx) noexcept
{
	ElementT copy = *GetAt(idx);
	_storage.RemoveAt(idx);
	return copy;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT TArray<ElementT, AllocatorFamilyT>::RemoveAtSwap_GetCopy(SizeType idx) noexcept
{
	ElementT copy = *GetAt(idx);
	_storage.RemoveAtSwap(idx);
	return copy;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT TArray<ElementT, AllocatorFamilyT>::Pop() noexcept
{
	ElementT copy = *GetLast();
	_storage.RemoveFromBack();
	return copy;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Swap(TArray& other) noexcept
{
	if constexpr (!TIsEmpty<AllocatorFamilyType>::Value)
	{
		KOR_NAMESPACE::Swap(_storage.Allocator, other._storage.Allocator);
	}

	KOR_NAMESPACE::Swap(_storage.Data, other._storage.Data);
	KOR_NAMESPACE::Swap(_storage.Num, other._storage.Num);
	KOR_NAMESPACE::Swap(_storage.ReservedNum, other._storage.ReservedNum);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Swap(SizeType firstIdx, SizeType secondIdx) noexcept
{
	if (firstIdx == secondIdx) return;

	KOR_ASSERT(firstIdx < secondIdx);
	KOR_ASSERT(IsValidIndex(firstIdx));
	KOR_ASSERT(IsValidIndex(secondIdx));

	KOR_NAMESPACE::Swap(_storage.Data[firstIdx], _storage.Data[secondIdx]);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SwapRange(SizeType firstIdx, SizeType secondIdx, SizeType num) noexcept
{
	if (firstIdx == secondIdx) return;

	KOR_ASSERT(firstIdx + num <= secondIdx);
	KOR_ASSERT(0 <= firstIdx && firstIdx + (num - 1) <= _storage.Num - 1);
	KOR_ASSERT(0 <= secondIdx && secondIdx + (num - 1) <= _storage.Num - 1);

	KOR_NAMESPACE::Swap(_storage.Data[firstIdx], _storage.Data[secondIdx], num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::begin() noexcept
{
	return _storage.Data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::begin() const noexcept
{
	return _storage.Data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::end() noexcept
{
	return _storage.Data + _storage.Num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::end() const noexcept
{
	return _storage.Data + _storage.Num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>& TArray<ElementT, AllocatorFamilyT>::operator=(const TArray& other) noexcept
{
	if (this != &other)
	{
		_storage.CopyFromOther(other._storage);
	}
	return *this;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>& TArray<ElementT, AllocatorFamilyT>::operator=(TArray&& other) noexcept
{
	if (this != &other)
	{
		_storage.MoveFromOther(Move(other._storage));
	}
	return *this;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>& TArray<ElementT, AllocatorFamilyT>::operator=(const ILType& list) noexcept
{
	Assign(list.begin(), list.size());
	return *this;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE bool TArray<ElementT, AllocatorFamilyT>::operator==(const TArray& other) const noexcept
{
	return
		_storage.Num == other._storage.Num &&
		SMemoryOps::IsEqual(_storage.Data, other._storage.Data, _storage.Num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::operator!=(const TArray& other) const noexcept
{
	return !operator==(other);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE bool TArray<ElementT, AllocatorFamilyT>::operator==(const ILType& other) const noexcept
{
	return
		_storage.Num == (SizeType)other.size() &&
		SMemoryOps::IsEqual(_storage.Data, other.begin(), _storage.Num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::operator!=(const ILType& other) const noexcept
{
	return !operator==(other);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::operator*() noexcept
{
	return _storage.Data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::operator*() const noexcept
{
	return _storage.Data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::operator[](SizeType idx) noexcept
{
	KOR_ASSERT(IsValidIndex(idx));
	return _storage.Data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT& TArray<ElementT, AllocatorFamilyT>::operator[](SizeType idx) const noexcept
{
	KOR_ASSERT(IsValidIndex(idx));
	return _storage.Data[idx];
}

// Container traits
// -------------------------------------------------------------------------

template<typename ElementT, typename AllocatorFamilyT>
struct TContainerTraits<TArray<ElementT, AllocatorFamilyT>>
	: TContainerTraitsBase<TArray<ElementT, AllocatorFamilyT>>
{
	using Type = TArray<ElementT, AllocatorFamilyT>;

	using ElementType = ElementT;
	using AllocatorFamilyType = AllocatorFamilyT;
	using SizeType = typename Type::SizeType;

	enum { InlineMemory = true };
};