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

	// TArray<...> friend
	struct SFriend
	{
		// Range check
		template<typename ArrayT>
		static void CheckRange(
			ArrayT& arr, 
			typename ArrayT::SizeType idx) noexcept
		{
			KOR_ASSERT(SMath::IsWithin(idx, 0, arr._num));
		}

		// Resizes allocation to exactly `num` elements (num * sizeof(element) bytes)
		// * Does NOT use allocation strategy
		// * Expects `num` to be already sanitized
		template<typename ArrayT>
		static void ReallocateExact(
			ArrayT& arr, 
			typename ArrayT::SizeType num) noexcept
		{
			if constexpr (TAllocatorTraits<ArrayT::AllocatorType>::HasReallocate)
			{
				arr._data = arr._allocator.Reallocate(arr._data, num);
				KOR_ASSERT(arr._data);
			}
			else
			{
				ArrayT::ElementType* newData = arr._allocator.Allocate(num);
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

		// * Expects `num` to be already sanitized
		template<typename ArrayT>
		static void Grow(
			ArrayT& arr, 
			typename ArrayT::SizeType num) noexcept
		{
			const auto newNum = TDefaultAllocationPolicy<ArrayT::ElementType>::CalculateGrow(
				num, 
				arr._reservedNum
			);

			if (newNum < num) newNum = num;
			if (newNum <= arr._reservedNum) return;

			ReallocateExact(arr, newNum);
		}

		template<typename ArrayT>
		static void Shrink(
			ArrayT& arr,
			typename ArrayT::SizeType num) noexcept
		{
			const typename ArrayT::SizeType newNum = SDefaultAllocationPolicy::CalculateShrink(
				num, 
				arr._reservedNum
			);

			if (newNum == arr._reservedNum ||
				!SMath::IsWithin(newNum, arr._num, num)) return;

			ReallocateExact(arr, newNum);
		}

		// Expects `num` and `slack` to be already sanitized
		template<typename InitType, typename ArrayT>
		static void InitFromEmpty(
			ArrayT& arr, 
			typename ArrayT::SizeType num, 
			typename ArrayT::SizeType slack) noexcept
		{
			static_assert(
				TIsSame<InitType, Init::SNoInit>::Value ||
				TIsSame<InitType, Init::SDefault>::Value ||
				TIsSame<InitType, Init::SZero>::Value,
				"InitFromEmpty: unsupported InitType"
			);

			const auto allocationSize = num + slack;

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

		template<typename InitType, typename ArrayT>
		static void Resize(
			ArrayT& arr,
			typename ArrayT::SizeType num)
		{
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
					ReallocateExact(arr, num);
				}

				const auto dataStart = arr._data + arr._num;
				const auto numDiff = num - arr._num;

				if constexpr (TIsSame<InitType, Init::SDefault>::Value)
				{
					SMemoryOps::DefaultConstruct(dataStart, numDiff);
				}
				else if constexpr (TIsSame<InitType, Init::SZero>::Value)
				{
					SMemoryOps::ZeroConstruct(dataStart, numDiff);
				}

				arr._num = num;
			}
			else if (num < arr._num)
			{
				const auto dataStart = arr._data + num;
				const auto numDiff = arr._num - num;

				SMemoryOps::Destruct(dataStart, numDiff);
				arr._num = num;
			}
		}

		// Expects arr to not be empty
		template<typename ArrayT>
		static void Empty(ArrayT& arr) noexcept
		{
			SMemoryOps::Destruct(arr._data, arr._num);
			_allocator.Deallocate(arr._data);

			arr._data = nullptr;
			arr._num = 0;
			arr._reservedNum = 0;
		}

		// Frees the underlying allocation, if any and resets `_data`/`_reservedNum`
		// * Does NOT destruct elements, caller must ensure `_num == 0` first
		template<typename ArrayT>
		static void Release(ArrayT& arr) noexcept
		{
			if (!arr._data) return;

			arr._allocator.Deallocate(arr._data);
			arr._data = nullptr;
			arr._reservedNum = 0;
		}

		// Expects:
		// - `num` and `slack` to be already sanitized
		// - arr is empty
		template<typename ArrayT>
		static void CopyToEmpty(
			ArrayT& arr, 
			const ArrayT::ElementType* data, 
			typename ArrayT::SizeType num, 
			typename ArrayT::SizeType slack = 0) noexcept
		{
			const auto allocationSize = num + slack;

			arr._data = arr._allocator.Allocate(allocationSize);
			KOR_ASSERT(arr._data);

			SMemoryOps::CopyConstruct(arr._data, data, num);

			arr._num = num;
			arr._reservedNum = allocationSize;
		}

		// Expects:
		// - dest is empty
		// - source is not empty
		template<typename ArrayT>
		static void CopyToEmpty(ArrayT& dest, const ArrayT& source) noexcept
		{
			if constexpr (!TIsEmpty<ElementAllocatorType>::Value)
			{
				dest._allocator = source._allocator;
			}

			CopyToEmpty(dest, source._data, source._num);
		}

		template<typename ArrayT>
		static void MoveToEmpty(ArrayT& dest, ArrayT&& source) noexcept
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

		// Bumps `_num` by `count`, growing capacity first if needed
		// * Expects `count > 0`
		// * Returns the index of the first newly-added (uninitialized) slot
		template<typename ArrayT>
		static typename ArrayT::SizeType AddUninitialized(
			ArrayT& arr,
			typename ArrayT::SizeType count) noexcept
		{
			const typename ArrayT::SizeType idx = arr._num;
			const typename ArrayT::SizeType newNum = idx + num;

			if (newNum > arr._reservedNum)
			{
				Grow(arr, newNum);
			}

			arr._num = newNum;
			return idx;
		}

		// Opens an unitialized gap of `count` elements at `idx`, growing capacity first if needed
		// * Expects `count > 0` and `idx` in [0, arr._num]
		// * Existing elements at and after `idx` are relocated past the gap
		// ** the gap itself is left uninitialized for the caller to construct into
		template<typename ArrayT>
		static void InsertUninitialized(
			ArrayT& arr,
			typename ArrayT::SizeType idx,
			typename ArrayT::SizeType count) noexcept
		{
			using SizeType = typename ArrayT::SizeType;

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
		template<typename ArrayT>
		static void RemoveAtShift(
			ArrayT& arr,
			typename ArrayT::SizeType idx,
			typename ArrayT::SizeType count) noexcept
		{
			const auto numAfterHole = arr._num - (idx + count);

			SMemoryOps::MoveAssign(
				arr._data + idx,
				arr._data + idx + count,
				numAfterHole
			);

			SMemoryOps::Destruct(
				arr._data + (arr._num - count),
				count
			);

			arr._num -= count;
		}

		// Removes `vount` elements at `idx` by swapping in elemnets from the tail (order not preserved)
		// * Expects `count > 0` and [idx, idx + count) to be a valid range
		template<typename ArrayT>
		static void RemoveAtSwapShift(
			ArrayT& arr,
			typename ArrayT::SizeType idx,
			typename ArrayT::SizeType count) noexcept
		{
			const auto numAfterHole = arr._num - (idx + count);
			const auto numToMove = SMath::Min(count, numAfterHole);

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