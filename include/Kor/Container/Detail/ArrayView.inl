// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

// Container traits
// -------------------------------------------------------------------------

namespace Detail::ArrayView
{
	template<typename ViewT>
	struct TFriend
	{
		using ViewType = ViewT;
		using ElementType = typename ViewType::ElementType;
		using SizeType = typename ViewType::SizeType;

		template<typename PredT>
		static constexpr SizeType FindIndexBy(const ViewType& view, PredT&& pred) KOR_NOEXCEPT_EXPR(pred(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<PredT, const ElementType&>::Value, "PredT format must be bool(const ElementT&)");

			ElementType* const end = view._data + view._num;
			for (ElementType* curr = view._data; curr != end; ++curr)
			{
				if (Invoke(pred, *curr))
				{
					return KOR_PTR_TYPED_DIFF(SizeType, curr, view._data);
				}
			}

			return KOR_INDEX_NONE;
		}

		template<typename PredT>
		static constexpr ElementType* FindBy(const ViewType& view, PredT&& pred) KOR_NOEXCEPT_EXPR(pred(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<PredT, const ElementType&>::Value, "PredT format must be bool(const ElementT&)");

			ElementType* const end = view._data + view._num;
			for (ElementType* curr = view._data; curr != end; ++curr)
			{
				if (Invoke(pred, *curr))
				{
					return curr;
				}
			}

			return nullptr;
		}
	};
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr TArrayView<ElementT, SizeT>::TArrayView(TNullptr) noexcept
	: _data(nullptr)
	, _num(0)
{}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr TArrayView<ElementT, SizeT>::TArrayView(const ILType& list) noexcept
	: _data(list.begin())
	, _num(list.size())
{}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr TArrayView<ElementT, SizeT>::TArrayView(ElementType* data, SizeType num) noexcept
	: _data(data)
	, _num(num)
{}

template<typename ElementT, typename SizeT>
template<TSize N>
KOR_FORCEINLINE TArrayView<ElementT, SizeT>::TArrayView(ElementType(& data)[N]) noexcept
	: _data(static_cast<ElementType*>(data))
	, _num(N)
{}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE TArrayView<ElementT, SizeT>& TArrayView<ElementT, SizeT>::operator=(const ILType& list) noexcept
{
	_data = list.begin();
	_num = list.size();
	return *this;
}

template<typename ElementT, typename SizeT>
KOR_INLINE bool TArrayView<ElementT, SizeT>::operator==(const TArrayView& other) const noexcept
{
	return
		_num == other._num &&
		SMemoryOps::IsEqual(_data, other._data, _num);
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE bool TArrayView<ElementT, SizeT>::operator!=(const TArrayView& other) const noexcept
{
	return !operator==(other);
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType* TArrayView<ElementT, SizeT>::operator*() const noexcept
{
	return _data;
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType& TArrayView<ElementT, SizeT>::operator[](SizeType idx) const noexcept
{
	return *GetAt(idx);
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType* TArrayView<ElementT, SizeT>::GetData() const noexcept
{
	return _data;
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::SizeType TArrayView<ElementT, SizeT>::GetNum() const noexcept
{
	return _num;
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr bool TArrayView<ElementT, SizeT>::IsEmpty() const noexcept
{
	return _num == 0;
}

template<typename ElementT, typename SizeT>
KOR_INLINE constexpr bool TArrayView<ElementT, SizeT>::IsValidIndex(SizeType idx) const noexcept
{
	return SMathOps::IsWithin(idx, (SizeType)0, _num - 1);
}

template<typename ElementT, typename SizeT>
KOR_INLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType* TArrayView<ElementT, SizeT>::GetAt(SizeType idx) const noexcept
{
	KOR_ASSERT(IsValidIndex(idx));
	return _data + idx;
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType* TArrayView<ElementT, SizeT>::GetFirst() const noexcept
{
	return _num > 0 ? _data : nullptr;
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType* TArrayView<ElementT, SizeT>::GetLast() const noexcept
{
	return _num > 0 ? _data + (_num - 1) : nullptr;
}

template<typename ElementT, typename SizeT>
template<typename KeyType>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::SizeType TArrayView<ElementT, SizeT>::FindIndex(const KeyType& key) const noexcept
{
	return SFriend::FindIndexBy(*this, [&key](const ElementType& element) -> bool { return SMemoryOps::IsEqual(&element, &key); });
}

template<typename ElementT, typename SizeT>
template<typename PredT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::SizeType TArrayView<ElementT, SizeT>::FindIndexBy(PredT&& pred) const
{
	return SFriend::FindIndexBy(*this, Forward<PredT>(pred));
}

template<typename ElementT, typename SizeT>
template<typename KeyType>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType* TArrayView<ElementT, SizeT>::Find(const KeyType& key) const noexcept
{
	return SFriend::FindBy(*this, [&key](const ElementType& element) -> bool { return SMemoryOps::IsEqual(&element, &key); });
}

template<typename ElementT, typename SizeT>
template<typename PredT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType* TArrayView<ElementT, SizeT>::FindBy(PredT&& pred) const
{
	return SFriend::FindBy(*this, Forward<PredT>(pred));
}

template<typename ElementT, typename SizeT>
template<typename KeyType>
KOR_FORCEINLINE constexpr bool TArrayView<ElementT, SizeT>::Contains(const KeyType& key) const noexcept
{
	return !!Find(key);
}

template<typename ElementT, typename SizeT>
template<typename PredT>
KOR_FORCEINLINE constexpr bool TArrayView<ElementT, SizeT>::ContainsBy(PredT&& pred) const
{
	return !!FindBy(Forward<PredT>(pred));
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::IteratorType TArrayView<ElementT, SizeT>::begin() noexcept
{
	return _data;
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::IteratorType TArrayView<ElementT, SizeT>::end() noexcept
{
	return _data + _num;
}

template<typename T>
TArrayView(std::initializer_list<T>) -> TArrayView<const T, int32>;

template<typename ElementT, typename SizeT>
struct TContainerTraits<TArrayView<ElementT, SizeT>>
	: TContainerTraitsBase<TArrayView<ElementT, SizeT>>
{
	using Type = TArrayView<ElementT, SizeT>;

	using ElementType = ElementT;
	using SizeType = SizeT;

	enum { InlineMemory = true };
};
