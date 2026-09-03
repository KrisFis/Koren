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

	static void CheckValidIndex(SizeType idx, SizeType max) noexcept
	{
		KOR_ASSERT(SMath::IsWithin(idx, 0, max));
	}

	static void CheckValidPointer(const void* ptr) noexcept
	{
		KOR_ASSERT(ptr);
	}

	template<typename ArrayT>
	struct TFriend
	{
		using ArrayType = ArrayT;
		using SizeType = typename ArrayType::SizeType;
		using ElementType = typename ArrayType::ElementType;

		// Memory
		// -------------------------------------------------------------------------

		// Destructs all items, but keeps array allocation
		static void Reset(ArrayType& arr) noexcept
		{
			KOR_ASSERT(arr._num > 0);

			SMemoryOps::Destruct(arr._data, arr._num);
			arr._num = 0;
		}

		// Destruct all items, releases array memory
		static void Empty(ArrayType& arr) noexcept
		{
			KOR_ASSERT(arr._reservedNum > 0);

			if (arr._num > 0)
			{
				SMemoryOps::Destruct(arr._data, arr._num);
				arr._num = 0;
			}

			_allocator.Deallocate(arr._data);
			arr._data = nullptr;
			arr._reservedNum = 0;
		}

		// Reallocates array memory
		// * i.e. sets "arr._reservedNum" to specific value of `num`
		// * optionally calls destructor on items
		//
		// @param HasItems - Handles array with already existing items (Num > 0)
		// @param arr - Array/this pointer
		// @param num - exact number of elements that final arr._data be able to hold
		//
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

					if constexpr (TAllocatorTraits<ArrayType::AllocatorType>::HasReallocate)
					{
						arr._data = arr._allocator.Reallocate(arr._data, num);
						KOR_ASSERT(arr._data);
					}
					else
					{
						ArrayType::ElementType* newData = arr._allocator.Allocate(num);
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

		// Resizes array memory
		// * i.e. sets "arr._num" to specific value of `num`
		//
		// @param HasItems - Handles array with already existing items (Num > 0)
		// @param arr - Array/this pointer
		// @param num - exact number of elements that final arr._data be able to hold
		//
		template<bool HasItems = true>
		static void Resize(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(
				num > 0 && 
				arr._num != num
			);

			if (arr._reservedNum != num)
			{
				Reallocate<HasItems>(arr, num);
			}

			arr._num = num;
		}

		// Helper to combine Resize + DefaultConstruct if needed
		template<bool HasItems = true>
		static void ResizeDefault(ArrayType& arr, SizeType num) noexcept
		{
			const SizeType oldNum = arr._num;
			Resize<HasItems>(arr, num);

			if (oldNum < num)
			{
				SMemoryOps::DefaultConstruct(arr._data + oldNum, num - oldNum);
			}
		}

		// Helper to combine Resize + ZeroConstruct if needed
		template<bool HasItems = true>
		static void ResizeZero(ArrayType& arr, SizeType num) noexcept
		{
			const SizeType oldNum = arr._num;
			Resize<HasItems>(arr, num);

			if (oldNum < num)
			{
				SMemoryOps::ZeroConstruct(arr._data + oldNum, num - oldNum);
			}
		}

		// Helper to combine Resize + ZeroConstruct if needed
		template<bool HasItems = true>
		static void ResizeFill(ArrayType& arr, const ElementType& val, SizeType num) noexcept
		{
			const SizeType oldNum = arr._num;
			Resize<HasItems>(arr, num);

			if (oldNum < num)
			{
				SMemoryOps::FillConstruct(arr._data + oldNum, val, num - oldNum);
			}
		}

		// Helper to combine Resize + ZeroConstruct if needed
		template<bool HasItems = true>
		static void ResizeCopy(ArrayType& arr, const ElementType* buffer, SizeType num) noexcept
		{
			KOR_ASSERT(buffer);

			const SizeType oldNum = arr._num;
			Resize<HasItems>(arr, num);

			if (oldNum < num)
			{
				SMemoryOps::CopyConstruct(arr._data + oldNum, buffer, num - oldNum);
			}
		}

		// Helper to combine Resize + ZeroConstruct if needed
		template<bool HasItems = true>
		static void ResizeMove(ArrayType& arr, ElementType* buffer, SizeType num) noexcept
		{
			KOR_ASSERT(buffer);

			const SizeType oldNum = arr._num;
			Resize<HasItems>(arr, num);

			if (oldNum < num)
			{
				SMemoryOps::MoveConstruct(arr._data + oldNum, buffer, num - oldNum);
			}
		}

		// Grows array memory using allocation policy
		template<bool HasItems = true>
		static void Grow(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(num > 0);

			const SizeType newNum = TDefaultAllocationPolicy<ElementType>::CalculateGrow(
				num, 
				arr._reservedNum
			);

			if (newNum < num) newNum = num;
			if (newNum <= arr._reservedNum) return;

			Reallocate<HasItems>(arr, newNum);
		}

		// Shrinks array memory using allocation policy
		template<bool HasItems = true>
		static void Shrink(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(num > 0);

			const SizeType newNum = TDefaultAllocationPolicy<ElementType>::CalculateShrink(
				num, 
				arr._reservedNum
			);

			if (newNum == arr._reservedNum) return;
			else if (!SMath::IsWithin(newNum, arr._num, num)) return;

			Reallocate<HasItems>(arr, newNum);
		}

		// Copies array memory and items from source array to dest array
		template<bool HasItems = true>
		static void CopyFromOther(ArrayType& dest, const ArrayType& source) noexcept
		{
			// TODO: Support different array type copy
			// For example:
			// * TArray<uint8> <-> TArray<uint16> is possible fairly safely if both are binary buffers,
			// * Although consumer can just read two bytes at the time, its convenience to be able to cast if allocators allow that

			// TODO_2: Maybe we can reuse allocation ?

			if constexpr (HasItems)
			{
				if (arr._reservedNum > 0) 
				{
					Empty(arr);
				}
			}

			if constexpr (!TIsEmpty<ElementAllocatorType>::Value)
			{
				dest._allocator = source._allocator;
			}

			ResizeCopy<false>(arr, source._data, source._num);
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
				if (arr._reservedNum > 0) 
				{
					Empty(arr);
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
			source._num = 0;
			source._reservedNum = 0;
		}

		// Mutations
		// -------------------------------------------------------------------------

		static SizeType AddUninitialized(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(num > 0);

			const SizeType idx = arr._num;
			const SizeType newNum = idx + num;

			if (newNum > arr._reservedNum)
			{
				Grow(arr, newNum);
			}

			arr._num = newNum;
			return idx;
		}

		// Opens an unitialized gap of `count` elements at `idx`, growing capacity first if needed
		// * Existing elements at and after `idx` are relocated past the gap
		// ** the gap itself is left uninitialized for the caller to construct into
		static void InsertUninitialized(ArrayType& arr, SizeType idx, SizeType num) noexcept
		{
			KOR_ASSERT(
				num > 0 && 
				SMath::IsWithin(idx, 0, arr._num)
			);

			const SizeType oldNum = arr._num;
			const SizeType newNum = oldNum + count;

			if (newNum > arr._reservedNum)
			{
				Grow(arr, newNum);
			}

			// TODO: Move buffer
			// We might move over already live objects
			// ex. HELO\0 -> HE[unitialized]LO
		}

		// Removes `num` elements at `idx`, shifting the tail left to close the gap (stable order)
		// * Expects `num > 0` and [idx, idx + num) to be a valid range
		static void RemoveAtShift(ArrayType& arr, SizeType idx,	SizeType num) noexcept
		{
			KOR_ASSERT(
				num > 0 && 
				SMath::IsWithin(idx, 0, arr._num) &&
				(idx + num) < arr._num
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

		// Removes `num` elements at `idx` by swapping in elemnets from the tail (order not preserved)
		static void RemoveAtSwapShift(ArrayType& arr, SizeType idx, SizeType num) noexcept
		{
			KOR_ASSERT(
				num > 0 && 
				SMath::IsWithin(idx, 0, arr._num) &&
				(idx + num) < arr._num
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
	};
}