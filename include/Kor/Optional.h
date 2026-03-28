// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/Memory.h"
#include "Kor/TypeTraits.h"

KOR_NAMESPACE_BEGIN

template<typename ElementT>
class TOptional
{
public:

	// Typedefs
	/////////////////////////////////

	typedef ElementT ElementType;

	// Asserts
	/////////////////////////////////

	static_assert(!TIsSame<ElementType, void>::Value && !TIsReference<ElementType>::Value, "Element type is not valid");

	// Constructors
	/////////////////////////////////

	KOR_FORCEINLINE TOptional() = default;
	KOR_FORCEINLINE TOptional(const TOptional& other) { FillToEmpty(other); }
	KOR_FORCEINLINE TOptional(TOptional&& other) { FillToEmpty(Move(other)); }
	KOR_FORCEINLINE TOptional(const ElementType& InValue) { FillToEmpty(InValue); }
	KOR_FORCEINLINE TOptional(ElementType&& InValue) { FillToEmpty(Move(InValue)); }

	// Destructor
	/////////////////////////////////

	KOR_FORCEINLINE ~TOptional() { Reset(); }

	// Comparison operators
	/////////////////////////////////

	KOR_FORCEINLINE bool operator==(const TOptional& other) const { return ComparePrivate(*this, other); }
	KOR_FORCEINLINE bool operator!=(const TOptional& other) const { return !ComparePrivate(*this, other); }

	// Assign operators
	/////////////////////////////////

	KOR_FORCEINLINE TOptional& operator=(const TOptional& other) { Reset(); FillToEmpty(other); return *this; }
	KOR_FORCEINLINE TOptional& operator=(TOptional&& other) noexcept { Reset(); FillToEmpty(Move(other)); return *this; }

	KOR_FORCEINLINE TOptional& operator=(const ElementType& InValue) { Reset(); FillToEmpty(InValue); return *this; }
	KOR_FORCEINLINE TOptional& operator=(ElementType&& InValue) { Reset(); FillToEmpty(Move(InValue)); return *this; }

	// Dereference operators
	/////////////////////////////////

	KOR_FORCEINLINE const ElementType* operator->() const { return _data; }
	KOR_FORCEINLINE ElementType* operator->() { return _data; }

	// Checks
	/////////////////////////////////

	KOR_FORCEINLINE bool IsValid() const { return !!_data; }
	KOR_FORCEINLINE bool IsSet() const { return IsValid(); }

	// Getters
	/////////////////////////////////

	// Gets copy
	KOR_FORCEINLINE ElementType Get(const ElementType& defaultValue) const { return IsSet() ? *_data : defaultValue; }
	KOR_FORCEINLINE ElementType Get() const { return GetDefaultedImpl(); }

	// Gets reference, but can crash
	KOR_FORCEINLINE const ElementType& GetRef() const { return *_data; }
	KOR_FORCEINLINE ElementType& GetRef() { return *_data; }

	// Manipulation
	/////////////////////////////////

	KOR_FORCEINLINE void Set(const ElementType& InValue) { Reset(); FillToEmpty(InValue); }
	KOR_FORCEINLINE void Set(ElementType&& InValue) { Reset(); FillToEmpty(Move(InValue)); }

	void Reset()
	{
		if(_data)
		{
			SMemory::Destruct(_data);
			SMemory::Free(_data);
			_data = nullptr;
		}
	}

private:

	void FillToEmpty(const ElementType& InValue)
	{
		_data = SMemory::MallocTyped<ElementType>();
		SMemory::CopyTyped(_data, &InValue);
	}

	void FillToEmpty(ElementType&& InValue)
	{
		_data = SMemory::MallocTyped<ElementType>();
		SMemory::MoveTyped(_data, &InValue);
	}

	KOR_FORCEINLINE void FillToEmpty(const TOptional& other)
	{
		if(other.IsSet())
		{
			FillToEmpty(*other._data);
		}
	}

	KOR_FORCEINLINE void FillToEmpty(TOptional&& other)
	{
		if(other.IsSet())
		{
			_data = other._data;
			other._data = nullptr;
		}
	}

	ElementType GetDefaultedImpl()
	{
		if constexpr(TIsConstructible<ElementType>::Value)
		{
			return IsSet() ? *_data : ElementType();
		}
		else
		{
			static_assert(sizeof(ElementType) < 0, "Unsuppported type for default construction");
			return DeclVal<ElementType>();
		}
	}

	static bool ComparePrivate(const TOptional& Lhs, const TOptional& Rhs)
	{
		if(Lhs.IsSet() == Rhs.IsSet())
		{
			return Lhs.IsSet() && SMemory::IsEqual(Lhs._data, Rhs._data) == 0;
		}

		return false;
	}

	ElementType* _data = nullptr;
};

// Archive operator<< && operator>>
////////////////////////////////////////////

template<typename T>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const TOptional<T>& optional)
{
	if (optional.IsSet())
	{
		ar << optional.GetRef();
	}

	return ar;
}

template<typename T>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, TOptional<T>& optional)
{
	if (optional.IsSet())
	{
		ar >> optional.GetRef();
	}

	return ar;
}

KOR_NAMESPACE_END