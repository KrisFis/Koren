// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Detail::Array
{
	template<typename ArrayT>
	struct TFriend
	{
		using ArrayType = ArrayT;
		using ILType = typename ArrayType::ILType;
		using SizeType = typename ArrayType::SizeType;
		using ElementType = typename ArrayType::ElementType;
		using AllocatorType = typename ArrayType::AllocatorType;
		using AllocatorOps = TAllocatorOps<AllocatorType>;

		// Memory
		// -------------------------------------------------------------------------

		template<bool HasItems = true>
		static void Reallocate(ArrayType& arr, SizeType num) noexcept
		{
			if constexpr (HasItems)
			{
				KOR_ASSERT(
					num > 0 &&
					arr._reservedNum != num
				);

				if (arr._reservedNum == 0)
				{
					arr._data = AllocatorOps::Allocate(arr._allocator, num);
					KOR_ASSERT(arr._data);
				}
				else
				{
					if (arr._num > num)
					{
						SMemoryOps::Destruct(
							arr._data + num,
							arr._num - num
						);

						arr._num = num;
					}

					arr._data = AllocatorOps::ReallocateConstructed(
						arr._allocator,
						arr._data,
						num,
						arr._reservedNum,
						arr._num
					);
				}
			}
			else
			{
				KOR_ASSERT(
					num > 0 &&
					arr._reservedNum == 0
				);

				arr._data = AllocatorOps::Allocate(arr._allocator, num);
				KOR_ASSERT(arr._data);
			}

			arr._reservedNum = num;
		}

		static void Deallocate(ArrayType& arr) noexcept
		{
			KOR_ASSERT(arr._reservedNum > 0);

			AllocatorOps::DeallocateConstructed(arr._allocator, arr._data, arr._num);

			arr._num = 0;
			arr._data = nullptr;
			arr._reservedNum = 0;
		}

		static void Grow(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(num > arr._reservedNum);

			num = AllocatorOps::CalculateGrow(
				arr._allocator,
				num,
				arr._reservedNum
			);

			if (num == arr._reservedNum) return; // no-op hit

			Reallocate(arr, num);
		}

		static void Shrink(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(
				num >= arr._num &&
				num < arr._reservedNum
			);

			num = AllocatorOps::CalculateShrink(
				arr._allocator,
				num,
				arr._reservedNum
			);

			if (num == arr._reservedNum) return; // no-op hit

			if (num > 0)
			{
				Reallocate(arr, num);
			}
			else if (num == 0 && arr._reservedNum > 0)
			{
				Deallocate(arr);
			}
		}

		static void ReallocateByPolicy(ArrayType& arr, SizeType num) noexcept
		{
			if (num > arr._reservedNum)
			{
				Grow(arr, num);
			}
			else if (num < arr._reservedNum)
			{
				Shrink(arr, num);
			}
		}

		// Version of reallocate that will reset elements beforehand
		// * Is faster alternative to Reset(arr); Reallocate(arr, num);
		static void ResetAndReallocate(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(num > 0);

			if (arr._reservedNum > 0)
			{
				if (arr._num > 0)
				{
					SMemoryOps::Destruct(arr._data, arr._num);
					arr._num = 0;
				}

				if (arr._reservedNum == num) return;

				arr._data = AllocatorOps::ReallocateWithFallback(
					arr._allocator,
					arr._data,
					num,
					arr._reservedNum
				);

				arr._reservedNum = num;
			}
			else
			{
				arr._data = AllocatorOps::Allocate(arr._allocator, num);
				arr._reservedNum = num;
			}
		}

		// Copies array memory and items from source array to dest array
		template<bool HasItems = true>
		static void CopyFromOther(ArrayType& dest, const ArrayType& source) noexcept
		{
			if constexpr (!TIsEmpty<AllocatorType>::Value) // EBO optimization
			{
				if constexpr (HasItems)
				{
					if (dest._reservedNum > 0)
					{
						SMemoryOps::Destruct(dest._data, dest._num);
						dest._num = 0;
					}
				}

				dest._allocator = source._allocator;
				Reallocate<false>(dest, source._num);
			}
			else if constexpr (HasItems)
			{
				ResetAndReallocate(dest, source._num);
			}
			else
			{
				Reallocate<false>(dest, source._num);
			}

			SMemoryOps::CopyConstruct(dest._data, source._data, source._num);
			dest._num = source._num;
		}

		// Moves array memory and items from source array to dest array
		template<bool HasItems = true>
		static void MoveFromOther(ArrayType& dest, ArrayType&& source) noexcept
		{
			if constexpr (HasItems)
			{
				if (dest._reservedNum > 0)
				{
					Deallocate(dest);
				}
			}

			if constexpr (!TIsEmpty<AllocatorType>::Value) // EBO optimization
			{
				dest._allocator = Move(source._allocator);
			}

			dest._data = source._data;
			dest._num = source._num;
			dest._reservedNum = source._reservedNum;

			source._data = nullptr;
			source._reservedNum = 0;
			source._num = 0;
		}

		// Query
		// -------------------------------------------------------------------------

		template<typename FunctorT>
		static SizeType FindIndexByFunc(const ArrayType& arr, FunctorT&& func)
			KOR_NOEXCEPT_EXPR(func(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<FunctorT, const ElementType&>::Value, "FunctorT format must be bool(const ElementT&)");

			const ElementType* const end = arr._data + arr._num;
			for (const ElementType* curr = arr._data; curr != end; ++curr)
			{
				if (Invoke(func, *curr))
				{
					return KOR_PTR_TYPED_DIFF(SizeType, curr, arr._data);
				}
			}

			return KOR_INDEX_NONE;
		}

		template<typename FunctorT>
		static const ElementType* FindByFunc(const ArrayType& arr, FunctorT&& func)
			KOR_NOEXCEPT_EXPR(func(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<FunctorT, const ElementType&>::Value, "FunctorT format must be bool(const ElementT&)");

			const ElementType* const end = arr._data + arr._num;
			for (const ElementType* curr = arr._data; curr != end; ++curr)
			{
				if (Invoke(func, *curr))
				{
					return curr;
				}
			}

			return nullptr;
		}

		template<typename FunctorT>
		KOR_FORCEINLINE static ElementType* FindByFunc(ArrayType& arr, FunctorT&& func)
			KOR_NOEXCEPT_EXPR(func(DeclVal<const ElementType&>()))
		{
			return const_cast<ElementType*>(FindByFunc((const ArrayType&)arr, Forward<FunctorT>(func)));
		}

		// Mutations
		// -------------------------------------------------------------------------

		static void Add(ArrayType& arr, SizeType num = 1) noexcept
		{
			KOR_ASSERT(num > 0);

			const SizeType newNum = arr._num + num;

			if (newNum > arr._reservedNum)
			{
				Grow(arr, newNum);
			}

			arr._num = newNum;
		}

		static void AppendFromOther(ArrayType& dest, const ArrayType& source) noexcept
		{
			KOR_ASSERT(&dest != &source);

			Add(dest, source._num);
			SMemoryOps::CopyConstruct(
				dest._data + (dest._num - source._num),
				source._data,
				source._num
			);
		}

		static void AppendFromOther(ArrayType& dest, ArrayType&& source) noexcept
		{
			KOR_ASSERT(&dest != &source);

			Add(dest, source._num);
			SMemoryOps::MoveConstruct(
				dest._data + (dest._num - source._num),
				source._data,
				source._num
			);

			AllocatorOps::DeallocateConstructed(source._data, source._num);

			source._data = nullptr;
			source._reservedNum = 0;
			source._num = 0;
		}

		// Opens an un-initialized gap of `num` elements at `idx`, growing capacity first if needed
		// * Existing elements at and after `idx` are relocated past the gap
		// ** the gap itself is left uninitialized for the caller to construct into
		static void Insert(ArrayType& arr, SizeType idx, SizeType num = 1) noexcept
		{
			KOR_ASSERT(
				num > 0 &&
				SMathOps::IsWithin(idx, 0, arr._num)
			);

			const SizeType oldNum = arr._num;
			const SizeType newNum = oldNum + num;
			if (newNum > arr._reservedNum)
			{
				Grow(arr, newNum);
			}

			ElementType* const data = arr._data;
			const SizeType numToMove = oldNum - idx;

			// Elements landing past oldNum hit raw memory; the rest land on still-live slots
			const SizeType numIntoRaw = (num < numToMove) ? num : numToMove;
			const SizeType numIntoLive = numToMove - numIntoRaw;

			// Relocate tail into raw memory
			if (numIntoRaw > 0)
			{
				SMemoryOps::MoveConstruct(data + oldNum + num - numIntoRaw, data + oldNum - numIntoRaw, numIntoRaw);
			}

			// Relocate overlap onto live slots
			if (numIntoLive > 0)
			{
				SMemoryOps::MoveAssign(data + idx + num, data + idx, numIntoLive);
			}

			// Retire vacated source
			// * [idx, idx + numIntoRaw) held the elements that were moved-from and never
			//   overwritten by a destination write — those are the only ones needing destruction
			SMemoryOps::Destruct(data + idx, numIntoRaw);

			arr._num = newNum;
		}

		// Removes `num` elements at `idx`, shifting the tail left to close the gap (stable order)
		// * Expects `num > 0` and [idx, idx + num) to be a valid range
		static void RemoveAt(ArrayType& arr, SizeType idx,	SizeType num = 1) noexcept
		{
			KOR_ASSERT(
				num > 0 &&
				SMathOps::IsWithin(idx, 0, arr._num - 1) &&
				(idx + num) <= arr._num
			);

			const SizeType numAfterHole = arr._num - (idx + num);

			SMemoryOps::MoveAssign(
				arr._data + idx,
				arr._data + idx + num,
				numAfterHole
			);

			SMemoryOps::Destruct(
				arr._data + (arr._num - num),
				num
			);

			arr._num -= num;
		}

		// Removes `num` elements at `idx` by swapping in elements from the tail (order not preserved)
		static void RemoveAtSwap(ArrayType& arr, SizeType idx, SizeType num = 1) noexcept
		{
			KOR_ASSERT(
				num > 0 &&
				SMathOps::IsWithin(idx, 0, arr._num - 1) &&
				(idx + num) <= arr._num
			);

			const SizeType numAfterHole = arr._num - (idx + num);
			const SizeType numToMove = SMathOps::Min(num, numAfterHole);

			SMemoryOps::MoveAssign(
				arr._data + idx,
				arr._data + arr._num - numToMove,
				numToMove
			);

			SMemoryOps::Destruct(
				arr._data + (arr._num - num),
				num
			);

			arr._num -= num;
		}

		static void RemoveFromBack(ArrayType& arr, SizeType num = 1) noexcept
		{
			KOR_ASSERT(arr._num > 0);

			arr._num -= num;
			SMemoryOps::Destruct(arr._data + arr._num, num);
		}

		// [RemoveByFunc]
		// Removes every element for which func(*ptr) is true, preserving order via a
		// single forward compaction pass. Each surviving element moves at most once;
		// contiguous survivor runs are batched into one MoveAssign call.
		// * Func: bool(const ElementType*)
		template<typename FunctorT>
		static SizeType RemoveByFunc(ArrayType& arr, FunctorT&& func)
			KOR_NOEXCEPT_EXPR(func(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<FunctorT, const ElementType&>::Value, "FunctorT format must be bool(const ElementT&)");

			ElementType* const oldEnd = arr._data + arr._num;
			ElementType* dst = arr._data;
			ElementType* src = arr._data;

			while (src != oldEnd)
			{
				if (Invoke(func, *(const ElementType*)src))
				{
					++src;
				}
				else
				{
					ElementType* const runStart = src;
					do
					{
						++src;
					} while (src != oldEnd && !Invoke(func, *(const ElementType*)src));

					const SizeType runLen = KOR_PTR_TYPED_DIFF(SizeType, src, runStart);
					if (dst != runStart)
					{
						SMemoryOps::MoveAssign(dst, runStart, runLen);
					}
					dst += runLen;
				}
			}

			const SizeType newNum = KOR_PTR_TYPED_DIFF(SizeType, dst, arr._data);
			const SizeType totalRemoved = arr._num - newNum;
			if (totalRemoved > 0)
			{
				SMemoryOps::Destruct(dst, totalRemoved);
				arr._num = newNum;
			}
			return totalRemoved;
		}

		// [RemoveSwapByFunc]
		// Removes every element for which func(ptr) is true via swap-from-tail;
		// order not preserved. Reverse scan batches contiguous match-runs into one
		// RemoveAtSwap call each. Safe because RemoveAtSwap only mutates addresses
		// >= idx, which the reverse scan has already visited and compared.
		// * Func: bool(const ElementType*)
		template<typename FunctorT>
		static SizeType RemoveSwapByFunc(ArrayType& arr, FunctorT&& func)
			KOR_NOEXCEPT_EXPR(func(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<FunctorT, const ElementType&>::Value, "FunctorT format must be bool(const ElementT&)");

			SizeType totalRemoved = 0;
			SizeType num = 0;
			const ElementType* const begin = arr._data;
			const ElementType* curr = arr._data + arr._num;

			while (curr != begin)
			{
				--curr;
				if (Invoke(func, *curr))
				{
					++num;
				}
				else if (num > 0)
				{
					RemoveAtSwap(arr, KOR_PTR_TYPED_DIFF(SizeType, curr, begin) + 1, num);
					totalRemoved += num;
					num = 0;
				}
			}
			if (num > 0)
			{
				RemoveAtSwap(arr, 0, num);
				totalRemoved += num;
			}
			return totalRemoved;
		}
	};
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::AllocatorType& TArray<ElementT, AllocatorFamilyT>::GetAllocator() noexcept
{
	return _allocator;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const TArray<ElementT, AllocatorFamilyT>::AllocatorType& TArray<ElementT, AllocatorFamilyT>::GetAllocator() const noexcept
{
	return _allocator;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::GetData() noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::GetData() const noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::GetNum() const noexcept
{
	return _num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::GetReservedNum() const noexcept
{
	return _reservedNum;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::IsEmpty() const noexcept
{
	return _num == 0;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::IsValidIndex(SizeType idx) const noexcept
{
	return SMathOps::IsWithin(idx, (SizeType)0, _num - 1);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::View() noexcept
{
	return TArrayView<ElementType, SizeType>(_data, _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::View() const noexcept
{
	return TConstArrayView<ElementType, SizeType>(_data, _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::SubView(SizeType idx, SizeType num) noexcept
{
	KOR_ASSERT(idx + num <= _num);
	return TArrayView<ElementType, SizeType>(_data + idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::SubView(SizeType idx, SizeType num) const noexcept
{
	KOR_ASSERT(idx + num <= _num);
	return TConstArrayView<ElementType, SizeType>(_data + idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::LeftView(SizeType num) noexcept
{
	KOR_ASSERT(num <= _num);
	return TArrayView<ElementType, SizeType>(_data, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::LeftView(SizeType num) const noexcept
{
	KOR_ASSERT(num <= _num);
	return TConstArrayView<ElementType, SizeType>(_data, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::RightView(SizeType num) noexcept
{
	KOR_ASSERT(num <= _num);
	return TArrayView<ElementType, SizeType>(_data + (_num - num), num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE TConstArrayView<ElementT, typename TArray<ElementT, AllocatorFamilyT>::SizeType> TArray<ElementT, AllocatorFamilyT>::RightView(SizeType num) const noexcept
{
	KOR_ASSERT(num <= _num);
	return TConstArrayView<ElementType, SizeType>(_data + (_num - num), num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorFamilyT>::TArray() noexcept
	: _data(nullptr)
	, _num(0)
	, _reservedNum(0)
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator) noexcept
	: _allocator(Move(allocator))
	, _data(nullptr)
	, _num(0)
	, _reservedNum(0)
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE consteval TArray<ElementT, AllocatorFamilyT>::TArray(Init::SConstEval) noexcept
{}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const TArray& other) noexcept
	: TArray()
{
	if (other._num == 0) return;
	SFriend::template CopyFromOther<false>(*this, other);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TArray<ElementT, AllocatorFamilyT>::TArray(TArray&& other) noexcept
	: TArray()
{
	if (other._num == 0) return;
	SFriend::template MoveFromOther<false>(*this, Move(other));
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const ILType& list) noexcept
	: TArray()
{
	const SizeType num = (SizeType)list.size();
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::CopyConstruct(_data, list.begin(), num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ILType& list) noexcept
	: TArray(Move(allocator))
{
	const SizeType num = (SizeType)list.size();
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::CopyConstruct(_data, list.begin(), num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(SizeType num, Init::SNoInit) noexcept
	: TArray()
{
	if (num <= 0) return;

	SFriend::template Reallocate<false>(*this, num);
	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, SizeType num, Init::SNoInit) noexcept
	: TArray(Move(allocator))
{
	if (num <= 0) return;

	SFriend::template Reallocate<false>(*this, num);
	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(SizeType num, Init::SDefault) noexcept
	: TArray()
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::DefaultConstruct(_data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, SizeType num, Init::SDefault) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::DefaultConstruct(_data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(SizeType num, Init::SZero) noexcept
	: TArray()
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::ZeroConstruct(_data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, SizeType num, Init::SZero) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::ZeroConstruct(_data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(const ElementType* data, SizeType num) noexcept
	: TArray()
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::CopyConstruct(_data, data, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ElementType* data, SizeType num) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::CopyConstruct(_data, data, num);

	_num = num;
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

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::FillConstruct(_data, val, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::TArray(AllocatorType&& allocator, const ElementType& val, SizeType num) noexcept
	: TArray(Move(allocator))
{
	if (num == 0) return;

	SFriend::template Reallocate<false>(*this, num);
	SMemoryOps::FillConstruct(_data, val, num);

	_num = num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>::~TArray() noexcept
{
	if (_reservedNum == 0) return;
	SFriend::Deallocate(*this);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE_DEBUG ElementT* TArray<ElementT, AllocatorFamilyT>::GetAt(SizeType idx) noexcept
{
	return IsValidIndex(idx) ? _data + idx : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE_DEBUG const ElementT* TArray<ElementT, AllocatorFamilyT>::GetAt(SizeType idx) const noexcept
{
	return IsValidIndex(idx) ? _data + idx : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::GetFirst() noexcept
{
	return _num > 0 ? _data : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::GetFirst() const noexcept
{
	return _num > 0 ? _data : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::GetLast() noexcept
{
	return _num > 0 ? _data + (_num - 1) : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::GetLast() const noexcept
{
	return _num > 0 ? _data + (_num - 1) : nullptr;
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::FindIndex(const KeyType& key) const noexcept
{
	return SFriend::FindIndexByFunc(*this,
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::FindIndexByFunc(FunctorT&& func) const
{
	return SFriend::FindIndexByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::Find(const KeyType& key) noexcept
{
	return SFriend::FindByFunc(*this,
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
	return SFriend::FindByFunc(*this,
		[&key](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &key);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::FindByFunc(FunctorT&& func)
{
	return SFriend::FindByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::FindByFunc(FunctorT&& func) const
{
	return SFriend::FindByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename KeyType>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::Contains(const KeyType& key) const noexcept
{
	return !!Find(key);
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::ContainsByFunc(FunctorT&& func) const
{
	return !!FindByFunc(Forward<FunctorT>(func));
}
template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Reserve(SizeType num) noexcept
{
	if (num <= _reservedNum) return;
	SFriend::Grow(*this, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::ReserveExact(SizeType num) noexcept
{
	if (num <= _reservedNum) return;
	SFriend::Reallocate(*this, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SetNum(SizeType num) noexcept
{
	if (num > _num)
	{
		if (num > _reservedNum) SFriend::Grow(*this, num);
		SMemoryOps::DefaultConstruct(_data + _num, num - _num);
		_num = num;
	}
	else if (num < _num)
	{
		SMemoryOps::Destruct(_data + num, _num - num);
		_num = num;
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SetNumZeroed(SizeType num) noexcept
{
	if (num > _num)
	{
		if (num > _reservedNum) SFriend::Grow(*this, num);
		SMemoryOps::ZeroConstruct(_data + _num, num - _num);
		_num = num;
	}
	else if (num < _num)
	{
		SMemoryOps::Destruct(_data + num, _num - num);
		_num = num;
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SetNumUninitialized(SizeType num) noexcept
{
	if (num > _num)
	{
		if (num > _reservedNum) SFriend::Grow(*this, num);
		_num = num;
	}
	else if (num < _num)
	{
		SMemoryOps::Destruct(_data + num, _num - num);
		_num = num;
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::ShrinkToFit() noexcept
{
	if (_num == _reservedNum) return;
	SFriend::Reallocate(*this, _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Reset() noexcept
{
	if (_num == 0) return;

	SMemoryOps::Destruct(_data, _num);
	_num = 0;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Empty(SizeType num) noexcept
{
	if (_num == 0) return;

	if (num > 0)
	{
		SFriend::ResetAndReallocate(*this, num);
	}
	else if (_reservedNum > 0)
	{
		SFriend::Deallocate(*this);
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Fill(const ElementType& val) noexcept
{
	if (_num == 0) return;
	SMemoryOps::FillAssign(_data, val, _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Assign(const TArray& other) noexcept
{
	if (this == &other) return;
	SFriend::CopyFromOther(*this, other);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Assign(TArray&& other) noexcept
{
	if (this == &other) return;
	SFriend::MoveFromOther(*this, Move(other));
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Assign(const ElementType& val, SizeType num) noexcept
{
	if (num > 0)
	{
		SFriend::ResetAndReallocate(*this, num);
		SMemoryOps::FillConstruct(_data, val, num);
		_num = num;
	}
	else if (_reservedNum > 0)
	{
		SFriend::Deallocate(*this);
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Assign(const ElementType* data, SizeType num) noexcept
{
	if (num > 0)
	{
		KOR_ASSERT(data);

		SFriend::ResetAndReallocate(*this, num);
		SMemoryOps::CopyConstruct(_data, data, num);
		_num = num;
	}
	else if (_reservedNum > 0)
	{
		SFriend::Deallocate(*this);
	}
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Add(const ElementType& val) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this);
	SMemoryOps::CopyConstruct(_data + idx, &val);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Add(ElementType&& val) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this);
	SMemoryOps::MoveConstruct(_data + (_num - 1), &val);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::Add_GetRef(const ElementType& val) noexcept
{
	const SizeType idx = Add(val);
	return _data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::Add_GetRef(ElementType&& val) noexcept
{
	const SizeType idx = Add(Move(val));
	return _data[idx];
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
	return _data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::AddUnique_GetRef(ElementType&& val) noexcept
{
	const SizeType idx = AddUnique(Move(val));
	return _data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::AddDefaulted(SizeType num) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this, num);
	SMemoryOps::DefaultConstruct(_data + idx, num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::AddDefaulted_GetRef() noexcept
{
	const SizeType idx = AddDefaulted();
	return _data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::AddZeroed(SizeType num) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this, num);
	SMemoryOps::ZeroConstruct(_data + idx, num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::AddZeroed_GetRef() noexcept
{
	const SizeType idx = AddZeroed();
	return _data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::AddUninitialized(SizeType num) noexcept
{
	const SizeType idx = _num;
	SFriend::Add(*this, num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::AddUninitialized_GetRef() noexcept
{
	const SizeType idx = AddUninitialized();
	return _data[idx];
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
	const SizeType idx = _num;
	SFriend::Add(*this);
	SMemoryOps::Construct(_data + idx, Forward<ArgTypes>(args)...);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename ... ArgTypes>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::Emplace_GetRef(ArgTypes&&... args) noexcept
{
	const SizeType idx = Emplace(Forward<ArgTypes>(args)...);
	return _data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Insert(SizeType idx, const ElementType& val) noexcept
{
	SFriend::Insert(*this, idx);
	SMemoryOps::CopyConstruct(_data + idx, &val);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Insert(SizeType idx, ElementType&& val) noexcept
{
	SFriend::Insert(*this, idx);
	SMemoryOps::MoveConstruct(_data + idx, &val);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Insert(SizeType idx, const ElementType* data, SizeType num) noexcept
{
	if (num == 0) return;

	SFriend::Insert(*this, idx, num);
	SMemoryOps::CopyConstruct(_data + idx, data, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::Insert(SizeType idx, const ILType& list) noexcept
{
	Insert(idx, list.begin(), list.size());
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(const TArray& other) noexcept
{
	if (other._num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _num;
	SFriend::AppendFromOther(*this, other);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(TArray&& other) noexcept
{
	if (other._num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _num;
	SFriend::AppendFromOther(*this, other);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(const ILType& list) noexcept
{
	const SizeType num = (SizeType)list.size();
	if (num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _num;
	SFriend::Add(*this, num);
	SMemoryOps::CopyConstruct(_data + idx, list.begin(), num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(const ElementType& val, SizeType num) noexcept
{
	if (num == 0) return KOR_INDEX_NONE;

	const SizeType idx = _num;
	SFriend::Add(*this, num);
	SMemoryOps::FillConstruct(_data + idx, val, num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Append(const ElementType* data, SizeType num) noexcept
{
	if (num == 0) return KOR_INDEX_NONE;

	KOR_ASSERT(data);

	const SizeType idx = _num;
	SFriend::Add(*this, num);
	SMemoryOps::CopyConstruct(_data + idx, data, num);
	return idx;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::Remove(const ElementType& val) noexcept
{
	return SFriend::RemoveByFunc(*this,
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &val);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSingle(const ElementType& val) noexcept
{
	const SizeType idx = SFriend::FindIndexByFunc(*this,
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &val);
		}
	);

	if (idx == KOR_INDEX_NONE) return 0;

	SFriend::RemoveAt(*this, idx);
	return 1;
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveByFunc(FunctorT&& func)
{
	return SFriend::RemoveByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSingleByFunc(FunctorT&& func)
{
	const SizeType idx = SFriend::FindIndexByFunc(*this, Forward<FunctorT>(func));
	if (idx == KOR_INDEX_NONE) return 0;

	SFriend::RemoveAt(*this, idx);
	return 1;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSwap(const ElementType& val) noexcept
{
	return SFriend::RemoveSwapByFunc(*this,
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &val);
		}
	);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSwapSingle(const ElementType& val) noexcept
{
	const SizeType idx = SFriend::FindIndexByFunc(*this,
		[&val](const ElementType& el) noexcept -> bool
		{
			return SMemoryOps::IsEqual(&el, &val);
		}
	);

	if (idx == KOR_INDEX_NONE) return 0;

	SFriend::RemoveAtSwap(*this, idx);
	return 1;
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSwapByFunc(FunctorT&& func)
{
	return SFriend::RemoveSwapByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename AllocatorFamilyT>
template<typename FunctorT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::SizeType TArray<ElementT, AllocatorFamilyT>::RemoveSwapSingleByFunc(FunctorT&& func)
{
	const SizeType idx = SFriend::FindIndexByFunc(*this, Forward<FunctorT>(func));
	if (idx == KOR_INDEX_NONE) return 0;

	SFriend::RemoveAtSwap(*this, idx);
	return 1;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::RemoveAt(SizeType idx, SizeType num) noexcept
{
	SFriend::RemoveAt(*this, idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE void TArray<ElementT, AllocatorFamilyT>::RemoveAtSwap(SizeType idx, SizeType num) noexcept
{
	SFriend::RemoveAtSwap(*this, idx, num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT TArray<ElementT, AllocatorFamilyT>::RemoveAt_GetCopy(SizeType idx) noexcept
{
	ElementT copy = *GetAt(idx);
	SFriend::RemoveAt(*this, idx);
	return copy;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT TArray<ElementT, AllocatorFamilyT>::RemoveAtSwap_GetCopy(SizeType idx) noexcept
{
	ElementT copy = *GetAt(idx);
	SFriend::RemoveAtSwap(*this, idx);
	return copy;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT TArray<ElementT, AllocatorFamilyT>::Pop() noexcept
{
	ElementT copy = *GetLast();
	SFriend::RemoveFromBack(*this);
	return copy;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Swap(TArray& other) noexcept
{
	if constexpr (!TIsEmpty<AllocatorFamilyType>::Value)
	{
		KOR_NAMESPACE::Swap(_allocator, other._allocator);
	}

	KOR_NAMESPACE::Swap(_data, other._data);
	KOR_NAMESPACE::Swap(_num, other._num);
	KOR_NAMESPACE::Swap(_reservedNum, other._reservedNum);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::Swap(SizeType firstIdx, SizeType secondIdx) noexcept
{
	if (firstIdx == secondIdx) return;

	KOR_ASSERT(firstIdx < secondIdx);
	KOR_ASSERT(IsValidIndex(firstIdx));
	KOR_ASSERT(IsValidIndex(secondIdx));

	KOR_NAMESPACE::Swap(_data[firstIdx], _data[secondIdx]);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_INLINE void TArray<ElementT, AllocatorFamilyT>::SwapRange(SizeType firstIdx, SizeType secondIdx, SizeType num) noexcept
{
	if (firstIdx == secondIdx) return;

	KOR_ASSERT(firstIdx + num <= secondIdx);
	KOR_ASSERT(0 <= firstIdx && firstIdx + (num - 1) <= _num - 1);
	KOR_ASSERT(0 <= secondIdx && secondIdx + (num - 1) <= _num - 1);

	KOR_NAMESPACE::Swap(_data[firstIdx], _data[secondIdx], num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::IteratorType TArray<ElementT, AllocatorFamilyT>::begin() noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::ConstIteratorType TArray<ElementT, AllocatorFamilyT>::begin() const noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::IteratorType TArray<ElementT, AllocatorFamilyT>::end() noexcept
{
	return _data + _num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE typename TArray<ElementT, AllocatorFamilyT>::ConstIteratorType TArray<ElementT, AllocatorFamilyT>::end() const noexcept
{
	return _data + _num;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>& TArray<ElementT, AllocatorFamilyT>::operator=(const TArray& other) noexcept
{
	if (this != &other)
	{
		SFriend::CopyFromOther(*this, other);
	}
	return *this;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE TArray<ElementT, AllocatorFamilyT>& TArray<ElementT, AllocatorFamilyT>::operator=(TArray&& other) noexcept
{
	if (this != &other)
	{
		SFriend::MoveFromOther(*this, Move(other));
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
		_num == other._num &&
		SMemoryOps::IsEqual(_data, other._data, _num);
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
		_num == (SizeType)other.size() &&
		SMemoryOps::IsEqual(_data, other.begin(), _num);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE bool TArray<ElementT, AllocatorFamilyT>::operator!=(const ILType& other) const noexcept
{
	return !operator==(other);
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT* TArray<ElementT, AllocatorFamilyT>::operator*() noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT* TArray<ElementT, AllocatorFamilyT>::operator*() const noexcept
{
	return _data;
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE ElementT& TArray<ElementT, AllocatorFamilyT>::operator[](SizeType idx) noexcept
{
	KOR_ASSERT(IsValidIndex(idx));
	return _data[idx];
}

template<typename ElementT, typename AllocatorFamilyT>
KOR_FORCEINLINE const ElementT& TArray<ElementT, AllocatorFamilyT>::operator[](SizeType idx) const noexcept
{
	KOR_ASSERT(IsValidIndex(idx));
	return _data[idx];
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