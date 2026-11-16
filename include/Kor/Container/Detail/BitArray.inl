// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

template<typename WordT, typename AllocatorFamilyT>
struct TContainerTraits<TBitArray<WordT, AllocatorFamilyT>>
	: TContainerTraitsBase<TBitArray<WordT, AllocatorFamilyT>>
{
	using Type = TBitArray<AllocatorFamilyT>;

	using ElementType = bool;
	using AllocatorFamilyType = AllocatorFamilyT;
	using SizeType = typename Type::SizeType;

	enum { InlineMemory = false };
};

template<typename WordT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TBitArray<WordT, AllocatorFamilyT>::TBitArray() noexcept
	: _storage()
{}

template<typename WordT, typename AllocatorFamilyT>
KOR_FORCEINLINE constexpr TBitArray<WordT, AllocatorFamilyT>::TBitArray(AllocatorType&& allocator) noexcept
	: _storage(Move(allocator))
{}

template<typename WordT, typename AllocatorFamilyT>
KOR_FORCEINLINE consteval TBitArray<WordT, AllocatorFamilyT>::TBitArray(Init::SConstEval) noexcept
	: _storage(Init::ConstEval)
{}

template<typename WordT, typename AllocatorFamilyT>
TBitArray<WordT, AllocatorFamilyT>::TBitArray(const TBitArray& other) noexcept
	: TBitArray()
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
constexpr TBitArray<WordT, AllocatorFamilyT>::TBitArray(TBitArray&& other) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
TBitArray<WordT, AllocatorFamilyT>::TBitArray(SizeType num, bool value) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
TBitArray<WordT, AllocatorFamilyT>::TBitArray(AllocatorType&& allocator, SizeType num, bool value) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
TBitArray<WordT, AllocatorFamilyT>::~TBitArray() noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
TBitArray<WordT, AllocatorFamilyT>& TBitArray<WordT, AllocatorFamilyT>::operator=(const TBitArray& other) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
TBitArray<WordT, AllocatorFamilyT>& TBitArray<WordT, AllocatorFamilyT>::operator=(TBitArray&& other) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
bool TBitArray<WordT, AllocatorFamilyT>::operator==(const TBitArray& other) const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
bool TBitArray<WordT, AllocatorFamilyT>::operator!=(const TBitArray& other) const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::ReferenceType TBitArray<WordT, AllocatorFamilyT>::operator[](SizeType idx) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::ConstReferenceType TBitArray<WordT, AllocatorFamilyT>::operator[](SizeType idx) const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::AllocatorType& TBitArray<WordT, AllocatorFamilyT>::GetAllocator() noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
const typename TBitArray<WordT, AllocatorFamilyT>::AllocatorType& TBitArray<WordT, AllocatorFamilyT>::GetAllocator() const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::WordType* TBitArray<WordT, AllocatorFamilyT>::GetData() noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
const typename TBitArray<WordT, AllocatorFamilyT>::WordType* TBitArray<WordT, AllocatorFamilyT>::GetData() const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::SizeType TBitArray<WordT, AllocatorFamilyT>::GetNum() const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::SizeType TBitArray<WordT, AllocatorFamilyT>::GetWordNum() const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::SizeType TBitArray<WordT, AllocatorFamilyT>::GetReservedNum() const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::SizeType TBitArray<WordT, AllocatorFamilyT>::GetReservedWordNum() const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
bool TBitArray<WordT, AllocatorFamilyT>::IsEmpty() const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
bool TBitArray<WordT, AllocatorFamilyT>::IsValidIndex(SizeType idx) const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::ReferenceType TBitArray<WordT, AllocatorFamilyT>::GetAt(SizeType idx) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::ConstReferenceType TBitArray<WordT, AllocatorFamilyT>::GetAt(SizeType idx) const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::Reserve(SizeType num) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::ReserveExact(SizeType num) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::SetNum(SizeType num, bool value) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::ShrinkToFit() noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::Reset() noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::Empty(SizeType num) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::SizeType TBitArray<WordT, AllocatorFamilyT>::Add(bool value) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::SizeType TBitArray<WordT, AllocatorFamilyT>::Add(bool value, SizeType num) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::RemoveAt(SizeType idx, SizeType num) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::RemoveAtSwap(SizeType idx, SizeType num) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
bool TBitArray<WordT, AllocatorFamilyT>::Pop() noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::Fill(bool value) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::Assign(const TBitArray& other) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
void TBitArray<WordT, AllocatorFamilyT>::Assign(TBitArray&& other) noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
typename TBitArray<WordT, AllocatorFamilyT>::SizeType TBitArray<WordT, AllocatorFamilyT>::FindIndex(bool value) const noexcept
{
	TODO_IMPLEMENT()
}

template<typename WordT, typename AllocatorFamilyT>
bool TBitArray<WordT, AllocatorFamilyT>::Contains(bool value) const noexcept
{
	TODO_IMPLEMENT()
}
