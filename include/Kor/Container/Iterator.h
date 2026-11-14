// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Container/Minimal.h"
#include "Kor/Container/ContainerTraits.h"

// Iterator over indexed container
// ContainerT must:
// * Expose types: ElementType and SizeType
// * Expose operators: ElementType& operator[](SizeType)
// * Expose methods: bool IsValidIndex(SizeType), void RemoveAtSwap(SizeType)
//
template<typename ContainerT, bool Reversed = false>
class TIndexedContainerIterator
{
	using ContainerTraits = TContainerTraits<TRemoveConst<ContainerT>::Type>;

	static_assert(!TIsVoid<ContainerTraits::ElementType>::Value, "ContainerT must define valid ElementType");
	static_assert(!TIsVoid<ContainerTraits::SizeType>::Value, "ContainerT must define valid SizeType")
	// Check ElementType& operator[](SizeType)
	// Check IsValidIndex(SizeType)
	// Check RemoveAt(SizeType)
	// Check RemoveAtSwap(SizeType)

public:
	using ElementType = typename ContainerTraits::ElementType;
	using SizeType = typename ContainerTraits::SizeType;

	KOR_FORCEINLINE explicit TIndexedContainerIterator(ContainerT& container, SizeType startIdx = 0)
		: _container(container)
		, _idx(startIdx)
	{}

	KOR_FORCEINLINE KOR_NODISCARD bool operator==(const TIndexedContainerIterator& other) const	{ return (&_container == &other._container) && (_idx == other._idx); }
	KOR_FORCEINLINE KOR_NODISCARD bool operator!=(const TIndexedContainerIterator& other) const { return !operator==(other); }

	KOR_FORCEINLINE TIndexedContainerIterator& operator++() { Increment(); return *this;	}
	KOR_FORCEINLINE KOR_NODISCARD TIndexedContainerIterator operator++(int) { TIndexedContainerIterator tmp(*this); Increment(); return tmp; }
	KOR_FORCEINLINE TIndexedContainerIterator& operator+=(SizeType offset) { Increment(offset); return *this; }
	KOR_FORCEINLINE KOR_NODISCARD TIndexedContainerIterator operator+(SizeType offset) const { TIndexedContainerIterator tmp(*this); return (tmp += offset): }	

	KOR_FORCEINLINE TIndexedContainerIterator& operator--() { Decrement(); return *this; }
	KOR_FORCEINLINE KOR_NODISCARD TIndexedContainerIterator operator--(int) { TIndexedContainerIterator tmp(*this); Decrement(); return *this;	}
	KOR_FORCEINLINE TIndexedContainerIterator& operator-=(SizeType offset) { Decrement(offset); return *this; }
	KOR_FORCEINLINE KOR_NODISCARD TIndexedContainerIterator operator-(SizeType offset) const { TIndexedContainerIterator tmp(*this); return (tmp -= offset): }	

	KOR_FORCEINLINE KOR_NODISCARD ElementType& operator*() const { return _container[_idx]; }
	KOR_FORCEINLINE KOR_NODISCARD ElementType* operator->() const { return &_container[_idx]; }

	KOR_FORCEINLINE KOR_NODISCARD explicit operator bool() const { return _container.IsValidIndex(_idx); }

	KOR_FORCEINLINE KOR_NODISCARD SizeType GetIndex() const { return _idx; }
	KOR_FORCEINLINE void SetIndex(SizeType idx) { _idx = idx; }

	KOR_FORCEINLINE void Remove() { _container.RemoveAt(_idx); Decrement(); }
	KOR_FORCEINLINE void RemoveSwap() { _container.RemoveAtSwap(_idx); Decrement(); }

	KOR_FORCEINLINE void Reset() { _idx = 0; }

private:
	KOR_FORCEINLINE void Increment(SizeType offset = 1) { if constexpr (Reversed) { _idx -= offset; } else { _idx += offset; } }
	KOR_FORCEINLINE void Decrement(SizeType offset = 1) { if constexpr (Reversed) { _idx += offset; } else { _idx -= offset; } }

	ContainerT& _container;
	SizeType _idx;
};