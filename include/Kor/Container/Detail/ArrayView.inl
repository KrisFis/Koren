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

		template<typename FunctorT>
		static constexpr SizeType FindIndexByFunc(const ViewType& view, FunctorT&& func) KOR_NOEXCEPT_EXPR(func(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<FunctorT, const ElementType&>::Value, "FunctorT format must be bool(const ElementT&)");

			ElementType* const end = view._data + view._num;
			for (ElementType* curr = view._data; curr != end; ++curr)
			{
				if (Invoke(func, *curr))
				{
					return KOR_PTR_TYPED_DIFF(SizeType, curr, view._data);
				}
			}

			return KOR_INDEX_NONE;
		}

		template<typename FunctorT>
		static constexpr ElementType* FindByFunc(const ViewType& view, FunctorT&& func) KOR_NOEXCEPT_EXPR(func(DeclVal<const ElementType&>()))
		{
			static_assert(TIsInvocable<FunctorT, const ElementType&>::Value, "FunctorT format must be bool(const ElementT&)");

			ElementType* const end = view._data + view._num;
			for (ElementType* curr = view._data; curr != end; ++curr)
			{
				if (Invoke(func, *curr))
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
KOR_FORCEINLINE constexpr TArrayView<ElementT, SizeT>::TArrayView(ElementType* data, const SizeType num) noexcept
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
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::SizeType TArrayView<ElementT, SizeT>::FindIndex(const ElementType& val) const noexcept
{
	return SFriend::FindIndexByFunc(*this, [&val](const ElementType& element) -> bool { return SMemoryOps::IsEqual(&element, &val); });
}

template<typename ElementT, typename SizeT>
template<typename FunctorT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::SizeType TArrayView<ElementT, SizeT>::FindIndexByFunc(FunctorT&& func) const
{
	return SFriend::FindIndexByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename SizeT>
template<typename KeyType>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::SizeType TArrayView<ElementT, SizeT>::FindIndexByKey(const KeyType& key) const noexcept
{
	return SFriend::FindIndexByFunc(*this, [&key](const ElementType& element) -> bool { return SMemoryOps::IsEqual(&element, &key); });
}

template<typename ElementT, typename SizeT>
template<typename FunctorT>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType* TArrayView<ElementT, SizeT>::FindByFunc(FunctorT&& func) const
{
	return SFriend::FindByFunc(*this, Forward<FunctorT>(func));
}

template<typename ElementT, typename SizeT>
template<typename KeyType>
KOR_FORCEINLINE constexpr typename TArrayView<ElementT, SizeT>::ElementType* TArrayView<ElementT, SizeT>::FindByKey(const KeyType& key) const noexcept
{
	return SFriend::FindByFunc(*this, [&key](const ElementType& element) -> bool { return SMemoryOps::IsEqual(&element, &key); });
}

template<typename ElementT, typename SizeT>
KOR_FORCEINLINE constexpr bool TArrayView<ElementT, SizeT>::Contains(const ElementType& val) const noexcept
{
	return !!FindByKey(val);
}

template<typename ElementT, typename SizeT>
template<typename FunctorT>
KOR_FORCEINLINE constexpr bool TArrayView<ElementT, SizeT>::ContainsByFunc(FunctorT&& func) const
{
	return !!FindByFunc(Forward<FunctorT>(func));
}

template<typename ElementT, typename SizeT>
template<typename KeyType>
KOR_FORCEINLINE constexpr bool TArrayView<ElementT, SizeT>::ContainsByKey(const KeyType& key) const noexcept
{
	return !!FindByKey(key);
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
