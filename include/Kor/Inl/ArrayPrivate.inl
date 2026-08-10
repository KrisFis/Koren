// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Internal::Array
{
	struct SFriend
	{
		template<typename ArrayT>
		static void CheckRange(
			ArrayT& arr, 
			typename ArrayT::SizeType idx) noexcept
		{
			KOR_ASSERT(SMath::IsWithin(idx, 0, arr._num));
		}

		template<typename InitType, typename ArrayT>
		static void InitFromEmpty(
			ArrayT& arr, 
			typename ArrayT::SizeType num, 
			typename ArrayT::SizeType slack) noexcept
		{
			const auto allocationSize = num + slack;

			arr._data = arr._allocator.Allocate(allocationSize);
			KOR_ASSERT(_data);

			if constexpr (TIsSame<InitType, Init::SDefault>::Value)
			{
				SMemoryOps::DefaultConstruct(arr._data, num);
			}
			else if constexpr (TIsSame<InitType, Init::SZero>::Value)
			{
				SMemoryOps::ZeroAs(arr._data, num);
			}

			arr._num = num;
			arr._reservedNum = allocationSize;
		}

		template<typename ArrayT>
		static void Empty(ArrayT& arr) noexcept
		{
			SMemoryOps::Destruct(arr._data, arr._num);
			_allocator.Deallocate(arr._data);

			arr._data = nullptr;
			arr._num = 0;
			arr._reservedNum = 0;
		}

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

			SMemoryOps::CopyAs(arr._data, data, num);

			arr._num = num;
			arr._reservedNum = allocationSize;
		}

		template<typename ArrayT>
		static void CopyToEmpty(ArrayT& dest, const ArrayT& source) noexcept
		{
			if constexpr (!TIsEmpty<ElementAllocatorType>::Value)
			{
				dest._allocator = source._allocator;
			}

			CopyToEmpty(dest, source._data, source._num);
		}

		// TODO: Implement it as 'swap' overload
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
	};
}