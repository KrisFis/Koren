// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Internal::Array
{
	template<typename ElementT>
	struct TDefaultAllocationPolicy
	{
		using SizeType = int64;

		static constexpr int64 INIT = 4;
		static constexpr int64 CONSTANT = KOR_DEFAULT_HEAP_ALIGNMENT;
		static constexpr float FACTOR = 3 / 8;

		static int64 CalculateGrow(
			int64 num,
			int64 oldNum) noexcept
		{
			int64 result = INIT;
			if (oldNum != 0 || num > INIT)
			{
				result = num + (num * FACTOR) + CONSTANT;
			}

			return result;
		}

		static int64 CalculateShrink(
			int64 num,
			int64 oldNum) noexcept
		{
			const int64 slack = num - oldNum;
			const int64 slackBytes = slack * sizeof(ElementT);

			int64 result = oldNum;
			if (slackBytes >= KOR_BUFFER_SIZE_LARGE)
			{
				result = num;
			}

			return result;
		}
	};

	template<typename ArrayT>
	struct TFriend
	{
		using ArrayType = ArrayT;
		using SizeType = typename ArrayType::SizeType;
		using ElementType = typename ArrayType::ElementType;
		using AllocatorType = typename ArrayType::AllocatorType;
		using ElementAllocatorType = typename ArrayType::ElementAllocatorType;

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
					arr._data = arr._allocator.Allocate(num);
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

					if constexpr (TAllocatorTraits<AllocatorType>::HasReallocate)
					{
						arr._data = arr._allocator.Reallocate(arr._data, num);
						KOR_ASSERT(arr._data);
					}
					else
					{
						ElementType* newData = arr._allocator.Allocate(num);
						KOR_ASSERT(newData);

						if (arr._num > 0)
						{
							SMemoryOps::MoveConstruct(newData, arr._data, arr._num);
							arr._allocator.Deallocate(arr._data);
						}

						arr._data = newData;
					}
				}
			}
			else
			{
				KOR_ASSERT(
					num > 0 &&
					arr._reservedNum == 0
				);

				arr._data = arr._allocator.Allocate(num);
				KOR_ASSERT(arr._data);
			}

			arr._reservedNum = num;
		}

		static void Deallocate(ArrayType& arr) noexcept
		{
			KOR_ASSERT(arr._reservedNum > 0);

			if (arr._num > 0)
			{
				SMemoryOps::Destruct(arr._data, arr._num);
				arr._num = 0;
			}

			arr._allocator.Deallocate(arr._data);
			arr._data = nullptr;
			arr._reservedNum = 0;
		}

		template<bool HasItems = true>
		static void Resize(ArrayType& arr, SizeType num) noexcept
		{
			if constexpr (HasItems)
			{
				Reallocate<true>(arr, num);
			}
			else if (arr._reservedNum != num)
			{
				Reallocate<false>(arr, num);
			}

			arr._num = num;
		}

		static void EmptyAndReallocate(ArrayType& arr, SizeType num) noexcept
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

				if constexpr (TAllocatorTraits<AllocatorType>::HasReallocate)
				{
					arr._data = arr._allocator.Reallocate(arr._data, num);
					KOR_ASSERT(arr._data);
				}
				else
				{
					arr._allocator.Deallocate(arr._data);
					arr._data = arr._allocator.Allocate(num);
					KOR_ASSERT(arr._data);
				}

				arr._reservedNum = num;
			}
			else
			{
				arr._data = arr._allocator.Allocate(num);
				arr._reservedNum = num;
			}
		}

		static void EmptyAndResize(ArrayType& arr, SizeType num) noexcept
		{
			EmptyAndReallocate(arr, num);
			arr._num = num;
		}

		// Destructs all items, but keeps array allocation
		static void Destruct(ArrayType& arr) noexcept
		{
			KOR_ASSERT(arr._num > 0);

			SMemoryOps::Destruct(arr._data, arr._num);
			arr._num = 0;
		}

		// Calculates `num` for `Allocate`, using allocation policy for growth
		static SizeType CalculateGrow(ArrayType& arr, SizeType num) noexcept
		{
			const SizeType newNum = TDefaultAllocationPolicy<ElementType>::CalculateGrow(
				num, 
				arr._reservedNum
			);

			// Return value that is at least as big as provided num
			return SMath::Max(newNum, num);
		}

		static void Grow(ArrayType& arr, SizeType num) noexcept
		{
			num = CalculateGrow(arr, num);
			if (num > arr._reservedNum)
			{
				Reallocate(arr, num);
			}
		}

		// Calculates `num` for `Allocate`, using allocation policy for shrink
		static SizeType CalculateShrink(ArrayType& arr, SizeType num) noexcept
		{
			const SizeType newNum = TDefaultAllocationPolicy<ElementType>::CalculateShrink(
				num, 
				arr._reservedNum
			);

			// Return value that is at most as provided num
			return SMath::Min(newNum, num);
		}

		template<bool LimitToInitialized = true>
		static void Shrink(ArrayType& arr, SizeType num) noexcept
		{
			num = CalculateShrink(arr, num);

			if constexpr (LimitToInitialized)
			{
				if (num < arr._num) return;
			}

			if (num > 0)
			{
				Reallocate(arr, num);
			}
			else if (num == 0 && arr._reservedNum > 0)
			{
				Deallocate(arr);
			}
		}

		// Copies array memory and items from source array to dest array
		template<bool HasItems = true>
		static void CopyFromOther(ArrayType& dest, const ArrayType& source) noexcept
		{
			// TODO: Support different array type copy
			// For example:
			// * TArray<uint8> <-> TArray<uint16> is possible fairly safely if both are binary buffers,
			// * Although consumer can just read two bytes at the time, its convenience to be able to cast if allocators allow that

			if constexpr (!TIsEmpty<ElementAllocatorType>::Value)
			{
				if constexpr (HasItems)
				{
					if (dest._reservedNum > 0)
					{
						Empty(dest);
					}
				}

				dest._allocator = source._allocator;
				Resize<false>(dest, source._num);
			}
			else if constexpr (HasItems)
			{
				EmptyAndResize(dest, source._num);
			}
			else
			{
				Resize<false>(dest, source._num);
			}

			SMemoryOps::CopyConstruct(dest._data, source._data, source._num);
		}

		// Moves array memory and items from source array to dest array
		template<bool HasItems = true>
		static void MoveFromOther(ArrayType& dest, ArrayType&& source) noexcept
		{
			// TODO: Support different array type move
			// For example:
			// * TArray<uint8> <-> TArray<uint16> is possible fairly safely swap if both are binary buffers,
			// * Although consumer can just read two bytes at the time, its convenience to be able to cast if allocators allow that

			if constexpr (HasItems)
			{
				if (dest._reservedNum > 0)
				{
					Deallocate(dest);
				}
			}

			if constexpr (!TIsEmpty<ElementAllocatorType>::Value)
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

			source._allocator.Deallocate(source._data);
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
				SMath::IsWithin(idx, 0, arr._num)
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
				SMath::IsWithin(idx, 0, arr._num - 1) &&
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
				SMath::IsWithin(idx, 0, arr._num - 1) &&
				(idx + num) <= arr._num
			);

			const SizeType numAfterHole = arr._num - (idx + num);
			const SizeType numToMove = SMath::Min(num, numAfterHole);

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