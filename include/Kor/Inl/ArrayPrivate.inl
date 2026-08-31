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

		// Checks
		// -------------------------------------------------------------------------

		static void CheckRange(ArrayType& arr, SizeType idx) noexcept
		{
			KOR_ASSERT(SMath::IsWithin(idx, 0, arr._num));
		}

		static void CheckPointer(const void* ptr) noexcept
		{
			KOR_ASSERT(ptr);
		}

		// Memory
		// -------------------------------------------------------------------------

		static void ReallocateRaw(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(
				num > 0 &&
				arr._reservedNum != num
			);

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

			arr._reservedNum = num;
		}

		static void Reallocate(ArrayType& arr, SizeType num) noexcept
		{
			if (arr._num > num)
			{
				SMemoryOps::Destruct(arr._data + num, arr._num - num);
				arr._num = num;
			}

			ReallocateRaw(arr, num);
		}

		template<typename InitType = Init::SNoInit>
		static void Resize(ArrayType& arr, SizeType num)
		{
			KOR_ASSERT(
				num > 0 && 
				arr._num != num
			);

			static_assert(
				TIsSame<InitType, Init::SNoInit>::Value ||
				TIsSame<InitType, Init::SDefault>::Value ||
				TIsSame<InitType, Init::SZero>::Value,
				"Resize: unsupported InitType"
			);

			if (num > arr._num)
			{
				if (num > arr._reservedNum)
				{
					ReallocateRaw(arr, num);
				}

				const SizeType dataStart = arr._data + arr._num;
				const SizeType numDiff = num - arr._num;

				if constexpr (TIsSame<InitType, Init::SDefault>::Value)
				{
					SMemoryOps::DefaultConstruct(dataStart, numDiff);
				}
				else if constexpr (TIsSame<InitType, Init::SZero>::Value)
				{
					SMemoryOps::ZeroConstruct(dataStart, numDiff);
				}
			}
			else if (num < arr._num)
			{
				const SizeType dataStart = arr._data + num;
				const SizeType numDiff = arr._num - num;

				SMemoryOps::Destruct(dataStart, numDiff);
			}

			arr._num = num;
		}

		static void Grow(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(num > 0);

			const SizeType newNum = TDefaultAllocationPolicy<ArrayType::ElementType>::CalculateGrow(
				num, 
				arr._reservedNum
			);

			if (newNum < num) newNum = num;
			if (newNum <= arr._reservedNum) return;

			Reallocate(arr, newNum);
		}

		static void Shrink(ArrayType& arr, SizeType num) noexcept
		{
			KOR_ASSERT(num > 0);

			const SizeType newNum = SDefaultAllocationPolicy::CalculateShrink(
				num, 
				arr._reservedNum
			);

			if (newNum == arr._reservedNum) return;
			else if (!SMath::IsWithin(newNum, arr._num, num)) return;

			Reallocate(arr, newNum);
		}

		// Construct From Empty
		// -------------------------------------------------------------------------

		template<typename InitType = Init::SNoInit>
		static void InitFromEmpty(
			ArrayType& arr, 
			SizeType num, 
			SizeType slack) noexcept
		{
			KOR_ASSERT(
				num > 0 && 
				arr._num == 0
			);

			static_assert(
				TIsSame<InitType, Init::SNoInit>::Value ||
				TIsSame<InitType, Init::SDefault>::Value ||
				TIsSame<InitType, Init::SZero>::Value,
				"InitFromEmpty: unsupported InitType"
			);

			const SizeType allocationSize = num + slack;

			arr._data = arr._allocator.Allocate(allocationSize);
			KOR_ASSERT(arr._data);

			if constexpr (TIsSame<InitType, Init::SDefault>::Value)
			{
				SMemoryOps::DefaultConstruct(arr._data, num);
			}
			else if constexpr (TIsSame<InitType, Init::SZero>::Value)
			{
				SMemoryOps::ZeroConstruct(arr._data, num);
			}

			arr._num = num;
			arr._reservedNum = allocationSize;
		}

		static void Empty(ArrayType& arr) noexcept
		{
			if (!arr._data) return;

			if (arr._num > 0)
			{
				SMemoryOps::Destruct(arr._data, arr._num);
			}

			_allocator.Deallocate(arr._data);
			arr._data = nullptr;
			arr._num = 0;
			arr._reservedNum = 0;
		}

		static void Release(ArrayType& arr) noexcept
		{
			if (!arr._data) return;

			arr._allocator.Deallocate(arr._data);

			arr._data = nullptr;
			arr._reservedNum = 0;
		}

		static void CopyToEmpty(
			ArrayType& arr, 
			const ElementType* data, 
			SizeType num, 
			SizeType slack = 0) noexcept
		{
			KOR_ASSERT(
				!!arr._data &&
				!!data &&
				num >= 0 && 
				slack >= 0
			);

			const SizeType allocationSize = num + slack;

			arr._data = arr._allocator.Allocate(allocationSize);
			KOR_ASSERT(arr._data);

			SMemoryOps::CopyConstruct(arr._data, data, num);

			arr._num = num;
			arr._reservedNum = allocationSize;
		}

		static void CopyToEmpty(ArrayType& dest, const ArrayType& source) noexcept
		{
			if constexpr (!TIsEmpty<ElementAllocatorType>::Value)
			{
				dest._allocator = source._allocator;
			}

			CopyToEmpty(dest, source._data, source._num);
		}

		static void MoveToEmpty(ArrayType& dest, ArrayType&& source) noexcept
		{
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

		// Mutation Helpers
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

		// Removes `count` elements at `idx`, shifting the tail left to close the gap (stable order)
		// * Expects `count > 0` and [idx, idx + count) to be a valid range
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

		// Removes `vount` elements at `idx` by swapping in elemnets from the tail (order not preserved)
		static void RemoveAtSwapShift(ArrayType& arr, SizeType idx, SizeType count) noexcept
		{
			KOR_ASSERT(
				num > 0 && 
				SMath::IsWithin(idx, 0, arr._num) &&
				(idx + num) < arr._num
			);

			const SizeType numAfterHole = arr._num - (idx + count);
			const SizeType numToMove = SMath::Min(count, numAfterHole);

			SMemoryOps::MoveAssign(
				arr._data + idx,
				arr._data + arr._num - numToMove,
				numToMove
			);

			SMemoryOps::Destruct(
				arr._data + (arr._num - count),
				count
			);

			arr._num -= count;
		}
	};
}