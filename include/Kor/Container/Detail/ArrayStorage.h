// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Container/Minimal.h"

#include "Kor/Memory/AllocatorOps.h"

#include "Kor/Utility/Invoke.h"

KOR_NAMESPACE_BEGIN

namespace Detail
{
	// Low-level owning storage for contiguous array-like containers.
	//
	// This type owns:
	// * the allocator instance,
	// * the allocated element buffer,
	// * the number of initialized elements,
	// * the reserved element capacity.
	//
	// Most member functions are intentionally narrow, precondition-checked
	// primitives. They assert when called with invalid ranges, invalid counts,
	// or an incompatible current state. Public containers are expected to do
	// higher-level policy checks first, then call these operations only when
	// their preconditions are satisfied.
	//
	// Example:
	// * public container copy constructor may skip the call entirely when the
	//   source is empty;
	// * CopyFromOther assumes the source contains at least one element because
	//   it eventually allocates exactly other.Num elements.
	//
	// Element lifetime model:
	// * [Data, Data + Num) contains initialized ElementType objects.
	// * [Data + Num, Data + ReservedNum) is allocated but uninitialized storage.
	// * Data is nullptr iff ReservedNum is 0.
	template<typename ElementT, typename AllocatorT>
	class TArrayStorage
	{
		KOR_DISABLE_COPY_AND_MOVE(TArrayStorage)

	public:
		using ElementType = ElementT;
		using AllocatorType = AllocatorT;

		using AllocatorTraits = TAllocatorTraits<AllocatorT>;
		using AllocatorOps = TAllocatorOps<AllocatorType>;
		using SizeType = typename AllocatorTraits::SizeType;

		// Constructors
		// -------------------------------------------------------------------------

		// Creates an empty storage object with a default-constructed allocator.
		KOR_FORCEINLINE constexpr TArrayStorage(
			ElementType* data = nullptr,
			SizeType num = 0,
			SizeType reservedNum = 0) noexcept
			: Allocator()
			, Data(nullptr)
			, Num()
			, ReservedNum(0)
		{}

		// Creates an empty storage object with a supplied allocator.
		KOR_FORCEINLINE constexpr TArrayStorage(
			AllocatorType&& allocator,
			ElementType* data = nullptr,
			SizeType num = 0,
			SizeType reservedNum = 0) noexcept
			: Allocator(Move(allocator))
			, Data(nullptr)
			, Num()
			, ReservedNum(0)
		{}

		// Const-eval construction hook used by the library initialization tags.
		KOR_FORCEINLINE consteval TArrayStorage(Init::SConstEval) noexcept {}

		// Properties
		// -------------------------------------------------------------------------

		// Allocator used for allocating, reallocating, and freeing Data.
		AllocatorType Allocator;

		// Pointer to the first element in the allocated storage.
		ElementType* Data;

		// Number of initialized elements in Data.
		SizeType Num;

		// Number of elements that can fit in the allocated Data buffer.
		SizeType ReservedNum;

		// Memory
		// -------------------------------------------------------------------------

		// Reallocates the storage to exactly `num` reserved elements.
		//
		// Preconditions:
		// * num > 0
		// * ReservedNum != num when HasItems is true
		// * ReservedNum == 0 when HasItems is false
		//
		// When HasItems is true:
		// * existing initialized elements are preserved up to the new capacity;
		// * elements past the new capacity are destructed.
		//
		// When HasItems is false:
		// * the storage must currently be empty;
		// * this only allocates raw storage and does not construct elements.
		template<bool HasItems = true>
		void Reallocate(SizeType num) noexcept
		{
			if constexpr (HasItems)
			{
				KOR_ASSERT(
					num > 0 &&
					ReservedNum != num
				);

				if (ReservedNum == 0)
				{
					Data = AllocatorOps::Allocate(Allocator, num);
					KOR_ASSERT(Data);
				}
				else
				{
					if (Num > num)
					{
						SMemoryOps::Destruct(
							Data + num,
							Num - num
						);

						Num = num;
					}

					Data = AllocatorOps::ReallocateConstructed(
						Allocator,
						Data,
						num,
						ReservedNum,
						Num
					);
				}
			}
			else
			{
				KOR_ASSERT(
					num > 0 &&
					ReservedNum == 0
				);

				Data = AllocatorOps::Allocate(Allocator, num);
				KOR_ASSERT(Data);
			}

			ReservedNum = num;
		}

		// Frees the allocated storage and destroys all initialized elements.
		//
		// Preconditions:
		// * ReservedNum > 0
		//
		// Postconditions:
		// * Data == nullptr
		// * Num == 0
		// * ReservedNum == 0
		void Deallocate() noexcept
		{
			KOR_ASSERT(ReservedNum > 0);

			AllocatorOps::DeallocateConstructed(Allocator, Data, Num);

			Num = 0;
			Data = nullptr;
			ReservedNum = 0;
		}

		// Grows capacity to fit at least `num` elements, using allocator growth policy.
		//
		// Preconditions:
		// * num > ReservedNum
		//
		// Note:
		// * the final capacity may be greater than `num`;
		// * initialized elements are preserved.
		void Grow(SizeType num) noexcept
		{
			KOR_ASSERT(num > ReservedNum);

			num = AllocatorOps::CalculateGrow(
				Allocator,
				num,
				ReservedNum
			);

			if (num == ReservedNum) return; // no-op hit

			Reallocate(num);
		}

		// Shrinks capacity according to allocator shrink policy.
		//
		// Preconditions:
		// * num >= Num
		// * num < ReservedNum
		//
		// Note:
		// * this never truncates initialized elements;
		// * the final capacity may be different from `num` depending on policy;
		// * policy may decide that shrinking is a no-op.
		void Shrink(SizeType num) noexcept
		{
			KOR_ASSERT(
				num >= Num &&
				num < ReservedNum
			);

			num = AllocatorOps::CalculateShrink(
				Allocator,
				num,
				ReservedNum
			);

			if (num == ReservedNum) return; // no-op hit

			if (num > 0)
			{
				Reallocate(num);
			}
			else if (num == 0 && ReservedNum > 0)
			{
				Deallocate();
			}
		}

		// Reallocates storage using grow/shrink policy.
		//
		// If `num` is greater than current capacity, Grow is used.
		// If `num` is smaller than current capacity, Shrink is used.
		// If `num` equals current capacity, this is a no-op.
		//
		// Preconditions:
		// * when shrinking, num >= Num must hold because Shrink cannot destroy
		//   initialized elements beyond the requested capacity.
		void ReallocateByPolicy(SizeType num) noexcept
		{
			if (num > ReservedNum)
			{
				Grow(num);
			}
			else if (num < ReservedNum)
			{
				Shrink(num);
			}
		}

		// Destroys all initialized elements and reallocates storage to exactly `num`.
		//
		// This is a faster alternative to:
		// * destructing/resetting all elements first;
		// * then reallocating the now-empty storage.
		//
		// Preconditions:
		// * num > 0
		//
		// Postconditions:
		// * Num == 0
		// * ReservedNum == num
		// * Data points to storage for `num` elements
		void ResetAndReallocate(SizeType num) noexcept
		{
			KOR_ASSERT(num > 0);

			if (ReservedNum > 0)
			{
				if (Num > 0)
				{
					SMemoryOps::Destruct(Data, Num);
					Num = 0;
				}

				if (ReservedNum == num) return;

				Data = AllocatorOps::ReallocateWithFallback(
					Allocator,
					Data,
					num,
					ReservedNum
				);

				ReservedNum = num;
			}
			else
			{
				Data = AllocatorOps::Allocate(Allocator, num);
				ReservedNum = num;
			}
		}

		// Copies allocator/storage/items from another non-empty storage object.
		//
		// Preconditions:
		// * other.Num > 0
		// * when HasItems is false, this storage must currently be empty
		//
		// When HasItems is true:
		// * current initialized elements are destroyed or replaced as needed.
		//
		// When HasItems is false:
		// * the caller guarantees that this object has no owned items/storage
		//   that need to be released first.
		template<bool HasItems = true>
		void CopyFromOther(const TArrayStorage& other) noexcept
		{
			if constexpr (!TIsEmpty<AllocatorType>::Value) // EBO optimization
			{
				if constexpr (HasItems)
				{
					if (Num > 0)
					{
						SMemoryOps::Destruct(Data, Num);
						Num = 0;
					}
				}

				Allocator = other.Allocator;
				Reallocate<false>(other.Num);
			}
			else if constexpr (HasItems)
			{
				ResetAndReallocate(other.Num);
			}
			else
			{
				Reallocate<false>(other.Num);
			}

			SMemoryOps::CopyConstruct(Data, other.Data, other.Num);
			Num = other.Num;
		}

		// Moves allocator/storage/items from another storage object.
		//
		// Preconditions:
		// * other owns the storage to transfer.
		// * when HasItems is false, this storage must currently be empty.
		//
		// Postconditions:
		// * this owns the previous contents of other;
		// * other is reset to an empty state.
		template<bool HasItems = true>
		void MoveFromOther(TArrayStorage&& other) noexcept
		{
			if constexpr (HasItems)
			{
				if (ReservedNum > 0)
				{
					Deallocate();
				}
			}

			if constexpr (!TIsEmpty<AllocatorType>::Value) // EBO optimization
			{
				Allocator = Move(other.Allocator);
			}

			Data = other.Data;
			Num = other.Num;
			ReservedNum = other.ReservedNum;

			other.Data = nullptr;
			other.ReservedNum = 0;
			other.Num = 0;
		}

		// Query
		// -------------------------------------------------------------------------

		// Finds the first initialized element matching `pred` and returns its index.
		//
		// Predicate format:
		// * bool(const ElementType&)
		//
		// Returns:
		// * index of the first matching element;
		// * KOR_INDEX_NONE when no element matches.
		template<typename PredT>
		SizeType FindIndexBy(PredT&& pred) const
			KOR_NOEXCEPT_EXPR(pred(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<PredT, const ElementType&>::Value, "PredT format must be bool(const ElementT&)");

			const ElementType* const end = Data + Num;
			for (const ElementType* curr = Data; curr != end; ++curr)
			{
				if (Invoke(pred, *curr))
				{
					return KOR_PTR_TYPED_DIFF(SizeType, curr, Data);
				}
			}

			return KOR_INDEX_NONE;
		}

		// Finds the first initialized element matching `pred`.
		//
		// Predicate format:
		// * bool(const ElementType&)
		//
		// Returns:
		// * pointer to the first matching element;
		// * nullptr when no element matches.
		template<typename PredT>
		const ElementType* FindBy(PredT&& pred) const
			KOR_NOEXCEPT_EXPR(pred(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<PredT, const ElementType&>::Value, "PredT format must be bool(const ElementT&)");

			const ElementType* const end = Data + Num;
			for (const ElementType* curr = Data; curr != end; ++curr)
			{
				if (Invoke(pred, *curr))
				{
					return curr;
				}
			}

			return nullptr;
		}

		// Mutable overload of FindBy.
		//
		// The search itself is shared with the const overload.
		template<typename PredT>
		KOR_FORCEINLINE ElementType* FindBy(PredT&& pred)
			KOR_NOEXCEPT_EXPR(pred(DeclVal<const ElementType&>()))
		{
			const TArrayStorage* constThis = this;
			return const_cast<ElementType*>(constThis->FindBy(Forward<PredT>(pred)));
		}

		// Mutations
		// -------------------------------------------------------------------------

		// Increases Num by `num`, growing capacity first if needed.
		//
		// Preconditions:
		// * num > 0
		//
		// Important:
		// * this only reserves logical slots;
		// * newly added elements are left uninitialized;
		// * caller must construct elements in the newly exposed range.
		void Add(SizeType num = 1) noexcept
		{
			KOR_ASSERT(num > 0);

			const SizeType newNum = Num + num;

			if (newNum > ReservedNum)
			{
				Grow(newNum);
			}

			Num = newNum;
		}

		// Appends all initialized elements from another non-empty storage object.
		//
		// Preconditions:
		// * this != &other
		// * other.Num > 0
		//
		// The destination grows if needed. Appended elements are copy-constructed
		// into previously uninitialized storage.
		void AppendFromOther(const TArrayStorage& other) noexcept
		{
			KOR_ASSERT(this != &other);

			Add(other.Num);
			SMemoryOps::CopyConstruct(
				Data + (Num - other.Num),
				other.Data,
				other.Num
			);
		}

		// Appends all initialized elements from another storage object by moving them.
		//
		// Preconditions:
		// * this != &other
		// * other.Num > 0
		//
		// Postconditions:
		// * moved elements are appended to this storage;
		// * other is reset to an empty state.
		void AppendFromOther(TArrayStorage&& other) noexcept
		{
			KOR_ASSERT(this != &other);

			Add(other.Num);
			SMemoryOps::MoveConstruct(
				Data + (Num - other.Num),
				other.Data,
				other.Num
			);

			AllocatorOps::DeallocateConstructed(other.Data, other.Num);

			other.Data = nullptr;
			other.ReservedNum = 0;
			other.Num = 0;
		}

		// Opens an uninitialized gap of `num` elements at `idx`.
		//
		// Existing elements at and after `idx` are relocated past the gap.
		// The gap itself is left uninitialized for the caller to construct into.
		//
		// Preconditions:
		// * num > 0
		// * idx is in [0, Num]
		//
		// Postconditions:
		// * Num is increased by `num`
		// * [Data + idx, Data + idx + num) is uninitialized storage
		void Insert(SizeType idx, SizeType num = 1) noexcept
		{
			KOR_ASSERT(
				num > 0 &&
				SMathOps::IsWithin(idx, 0, Num)
			);

			const SizeType oldNum = Num;
			const SizeType newNum = oldNum + num;
			if (newNum > ReservedNum)
			{
				Grow(newNum);
			}

			ElementType* const data = Data;
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

			Num = newNum;
		}

		// Removes `num` elements at `idx`, preserving element order.
		//
		// Elements after the removed range are shifted left to close the gap.
		//
		// Preconditions:
		// * num > 0
		// * idx is a valid initialized element index
		// * [idx, idx + num) is inside [0, Num)
		void RemoveAt(SizeType idx,	SizeType num = 1) noexcept
		{
			KOR_ASSERT(
				num > 0 &&
				SMathOps::IsWithin(idx, 0, Num - 1) &&
				(idx + num) <= Num
			);

			const SizeType numAfterHole = Num - (idx + num);

			SMemoryOps::MoveAssign(
				Data + idx,
				Data + idx + num,
				numAfterHole
			);

			SMemoryOps::Destruct(
				Data + (Num - num),
				num
			);

			Num -= num;
		}

		// Removes `num` elements at `idx` without preserving element order.
		//
		// Elements from the tail are moved into the removed range.
		//
		// Preconditions:
		// * num > 0
		// * idx is a valid initialized element index
		// * [idx, idx + num) is inside [0, Num)
		void RemoveAtSwap(SizeType idx, SizeType num = 1) noexcept
		{
			KOR_ASSERT(
				num > 0 &&
				SMathOps::IsWithin(idx, 0, Num - 1) &&
				(idx + num) <= Num
			);

			const SizeType numAfterHole = Num - (idx + num);
			const SizeType numToMove = SMathOps::Min(num, numAfterHole);

			SMemoryOps::MoveAssign(
				Data + idx,
				Data + Num - numToMove,
				numToMove
			);

			SMemoryOps::Destruct(
				Data + (Num - num),
				num
			);

			Num -= num;
		}

		// Removes initialized elements from the end of the storage.
		//
		// Preconditions:
		// * Num > 0
		// * num <= Num
		//
		// Note:
		// * the current assertion only checks Num > 0; callers are still expected
		//   to satisfy num <= Num.
		void RemoveFromBack(SizeType num = 1) noexcept
		{
			KOR_ASSERT(Num > 0 && num <= Num);

			Num -= num;
			SMemoryOps::Destruct(Data + Num, num);
		}

		// Removes every element for which `pred(element)` returns true.
		//
		// Order is preserved using a single forward compaction pass.
		// Each surviving element moves at most once, and contiguous survivor
		// runs are batched into one MoveAssign call.
		//
		// Predicate format:
		// * bool(const ElementType&)
		//
		// Returns:
		// * number of removed elements.
		template<typename PredT>
		SizeType RemoveBy(PredT&& pred) KOR_NOEXCEPT_EXPR(pred(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<PredT, const ElementType&>::Value, "PredT format must be bool(const ElementT&)");

			ElementType* const oldEnd = Data + Num;
			ElementType* dst = Data;
			ElementType* src = Data;

			while (src != oldEnd)
			{
				if (Invoke(pred, *(const ElementType*)src))
				{
					++src;
				}
				else
				{
					ElementType* const runStart = src;
					do
					{
						++src;
					} while (src != oldEnd && !Invoke(pred, *(const ElementType*)src));

					const SizeType runLen = KOR_PTR_TYPED_DIFF(SizeType, src, runStart);
					if (dst != runStart)
					{
						SMemoryOps::MoveAssign(dst, runStart, runLen);
					}
					dst += runLen;
				}
			}

			const SizeType newNum = KOR_PTR_TYPED_DIFF(SizeType, dst, Data);
			const SizeType totalRemoved = Num - newNum;
			if (totalRemoved > 0)
			{
				SMemoryOps::Destruct(dst, totalRemoved);
				Num = newNum;
			}
			return totalRemoved;
		}

		// Removes every element for which `pred(element)` returns true.
		//
		// Order is not preserved. Matching elements are removed by swapping/moving
		// tail elements into their place. Reverse scanning batches contiguous
		// match-runs into one RemoveAtSwap call each.
		//
		// Predicate format:
		// * bool(const ElementType&)
		//
		// Returns:
		// * number of removed elements.
		template<typename PredT>
		SizeType RemoveSwapBy(PredT&& pred) KOR_NOEXCEPT_EXPR(pred(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<PredT, const ElementType&>::Value, "PredT format must be bool(const ElementT&)");

			SizeType totalRemoved = 0;
			SizeType num = 0;
			const ElementType* const begin = Data;
			const ElementType* curr = Data + Num;

			while (curr != begin)
			{
				--curr;
				if (Invoke(pred, *curr))
				{
					++num;
				}
				else if (num > 0)
				{
					RemoveAtSwap(KOR_PTR_TYPED_DIFF(SizeType, curr, begin) + 1, num);
					totalRemoved += num;
					num = 0;
				}
			}
			if (num > 0)
			{
				RemoveAtSwap(0, num);
				totalRemoved += num;
			}
			return totalRemoved;
		}
	};
}

KOR_NAMESPACE_END