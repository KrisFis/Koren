// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Container/Minimal.h"
#include "Kor/Container/ContainerTraits.h"

KOR_NAMESPACE_BEGIN

namespace Iterators::Container
{
	// Iterator over indexed container
	// ContainerT must:
	// * Expose types: ElementType and SizeType
	// * Expose operators: ElementType& operator[](SizeType)
	// * Expose methods: bool IsValidIndex(SizeType)
	//
	// Examples:
	//
	// for (auto it = Iterators::Container::MakeIndexed(container); it; ++it) { *it = ...; }
	//
	template<typename ContainerT>
	class TIndexed
	{
		static constexpr bool Const = TIsConst<ContainerT>::Value;
		using ContainerTraits = TContainerTraits<typename TRemoveConst<ContainerT>::Type>;

		static_assert(!TIsVoid<typename ContainerTraits::ElementType>::Value, "ContainerT must define valid ElementType");
		static_assert(!TIsVoid<typename ContainerTraits::SizeType>::Value, "ContainerT must define valid SizeType");
		// Check ElementType& operator[](SizeType)
		// Check IsValidIndex(SizeType)

	public:
		using ContainerType = typename TRemoveConst<ContainerT>::Type;
		using ElementType = typename ContainerTraits::ElementType;
		using SizeType = typename ContainerTraits::SizeType;
		using AccessType = TChoose<Const, const ElementType, ElementType>;

		KOR_FORCEINLINE explicit TIndexed(ContainerT& container)
			: _container(container)
			, _idx(0)
		{}

		KOR_FORCEINLINE explicit TIndexed(ContainerT& container, SizeType startIdx)
			: _container(container)
			, _idx(startIdx)
		{}

		KOR_NODISCARD KOR_FORCEINLINE bool operator==(const TIndexed& other) const	{ return (&_container == &other._container) && (_idx == other._idx); }
		KOR_NODISCARD KOR_FORCEINLINE bool operator!=(const TIndexed& other) const { return !operator==(other); }

		KOR_FORCEINLINE TIndexed& operator++() { ++_idx; return *this;	}
		KOR_NODISCARD KOR_FORCEINLINE TIndexed operator++(int) { TIndexed tmp(*this); ++_idx; return tmp; }
		KOR_FORCEINLINE TIndexed& operator+=(SizeType offset) { _idx += offset; return *this; }
		KOR_NODISCARD KOR_FORCEINLINE TIndexed operator+(SizeType offset) const { TIndexed tmp(*this); return (tmp += offset); }

		KOR_FORCEINLINE TIndexed& operator--() { --_idx; return *this; }
		KOR_NODISCARD KOR_FORCEINLINE TIndexed operator--(int) { TIndexed tmp(*this); --_idx; return tmp; }
		KOR_FORCEINLINE TIndexed& operator-=(SizeType offset) { _idx -= offset; return *this; }
		KOR_NODISCARD KOR_FORCEINLINE TIndexed operator-(SizeType offset) const { TIndexed tmp(*this); return (tmp -= offset); }

		KOR_NODISCARD KOR_FORCEINLINE AccessType& operator*() const { return _container[_idx]; }
		KOR_NODISCARD KOR_FORCEINLINE AccessType* operator->() const { return &_container[_idx]; }

		KOR_NODISCARD KOR_FORCEINLINE explicit operator bool() const { return _container.IsValidIndex(_idx); }

		KOR_NODISCARD KOR_FORCEINLINE SizeType GetIndex() const { return _idx; }
		KOR_FORCEINLINE void SetIndex(SizeType idx) { _idx = idx; }

		KOR_FORCEINLINE ContainerT& GetContainer() const { return _container; }

	private:
		ContainerT& _container;
		SizeType _idx;
	};

	template<typename ContainerT>
	KOR_NODISCARD KOR_FORCEINLINE static TIndexed<ContainerT> MakeIndexed(ContainerT& container)
	{
		return TIndexed<ContainerT>(container);
	}

	template<typename ContainerT>
	KOR_NODISCARD KOR_FORCEINLINE static TIndexed<ContainerT> MakeIndexed(ContainerT& container, TSize startIdx)
	{
		using ItT = TIndexed<ContainerT>;
		return TIndexed<ContainerT>(container, (typename ItT::SizeType)startIdx);
	}
}

KOR_NAMESPACE_END