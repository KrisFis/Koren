// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/_internal/SharedReferencer.h"
#include "Kor/_internal/SharedTypeTraits.h"

KOR_NAMESPACE_BEGIN

// Equivalent of std's shared_ptr
template<typename T>
class TSharedPtr
{
	template<typename OtherT> friend class TSharedPtr;
	template<typename OtherT> friend class TWeakPtr;

public:

	// Typedefs
	/////////////////////////////////

	typedef T ObjectType;

	// Constructor
	/////////////////////////////////

	KOR_FORCEINLINE TSharedPtr(_NShared::SNullType* = nullptr) {}
	KOR_FORCEINLINE TSharedPtr(_NShared::CReferencerBase& ref) : _referencerProxy(&ref) { _referencerProxy.AddShared(); }

	// Copy/Move constructors [SharedPtr]
	/////////////////////////////////

	KOR_FORCEINLINE TSharedPtr(const TSharedPtr& other) { ReplaceBy(other); }
	KOR_FORCEINLINE TSharedPtr(TSharedPtr&& other) { ReplaceBy(Forward<TSharedPtr>(other)); }

	// Destructor
	/////////////////////////////////

	KOR_FORCEINLINE ~TSharedPtr() { Reset(); }

	// Conversion operators
	/////////////////////////////////

	KOR_FORCEINLINE operator bool() const { return IsValid(); }

	template<typename OtherT, typename TEnableIf<TIsBaseOf<OtherT, T>::Value>::Type* = nullptr>
	KOR_FORCEINLINE explicit operator TSharedPtr<OtherT>() const { return _referencerProxy.IsValid() ? TSharedPtr<OtherT>(*_referencerProxy) : nullptr; }

	// Comparison operators
	/////////////////////////////////

	KOR_FORCEINLINE bool operator==(const TSharedPtr& other) const { return _referencerProxy == other._referencerProxy; }
	KOR_FORCEINLINE bool operator!=(const TSharedPtr& other) const { return !operator==(other); }

	// Assignment operators
	/////////////////////////////////

	KOR_FORCEINLINE TSharedPtr& operator=(_NShared::SNullType*) { Reset(); return *this; }

	KOR_FORCEINLINE TSharedPtr& operator=(const TSharedPtr& Other) { if(&Other != this) ReplaceBy(Other); return *this; }
	KOR_FORCEINLINE TSharedPtr& operator=(TSharedPtr&& Other) { if(&Other != this) ReplaceBy(Forward<TSharedPtr>(Other)); return *this; }

	// Pointer operators
	/////////////////////////////////

	KOR_FORCEINLINE ObjectType* operator->() const { return Get(); }
	KOR_FORCEINLINE ObjectType& operator*() const { return *Get(); }

	// Validation
	/////////////////////////////////

	KOR_FORCEINLINE bool IsValid() const { return _referencerProxy.IsSafeToDereference(); }
	KOR_FORCEINLINE bool IsUnique() const { return _referencerProxy.IsUnique(); }

	// Getters
	/////////////////////////////////

	KOR_FORCEINLINE ObjectType* Get() const { return _referencerProxy.IsValid() ? _referencerProxy->GetObject<ObjectType>() : nullptr; }
	KOR_FORCEINLINE ObjectType& GetRef() { return *Get(); }

	// Other
	/////////////////////////////////

	KOR_FORCEINLINE void Reset() { _referencerProxy.RemoveShared(); _referencerProxy.Set(nullptr); }

private:

	// const PtrType&
	// * Copy
	KOR_FORCEINLINE_DEBUGGABLE void ReplaceBy(const TSharedPtr& other)
	{
		// How it should work ? (Copy implementation)

		// * SharedPtr as argument:
		// ** 1) Remove shared reference from current
		// ** 2) Add shared reference to other
		// ** 3) Replace referencer

		_referencerProxy.RemoveShared(); // 1
		other._referencerProxy.AddShared(); // 2
		_referencerProxy = other._referencerProxy; // 3
	}

	// PtrType&&
	// * Move
	KOR_FORCEINLINE_DEBUGGABLE void ReplaceBy(TSharedPtr&& other)
	{
		// How it should work ? (move implementation)

		// * SharedPtr as argument:
		// ** 1) Remove shared reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer

		_referencerProxy.RemoveShared(); // 1
		_referencerProxy = other._referencerProxy; // 2

		other._referencerProxy.Set(nullptr); // 3
	}

	mutable _NShared::SReferencerProxy _referencerProxy = nullptr;
};

// Archive operator<< && operator>>
////////////////////////////////////////////

template<typename T>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const TSharedPtr<T>& sharedPtr)
{
	if (sharedPtr.IsValid())
	{
		ar << sharedPtr.GetRef();
	}

	return ar;
}

template<typename T>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, TSharedPtr<T>& sharedPtr)
{
	if (sharedPtr.IsValid())
	{
		ar >> sharedPtr.GetRef();
	}

	return ar;
}

// Equivalent of std's weak_ptr
template<typename T>
class TWeakPtr
{
	template<typename OtherT> friend class TSharedPtr;
	template<typename OtherT> friend class TWeakPtr;

public:

	// Typedefs
	/////////////////////////////////

	typedef T ObjectType;

	// Constructors
	/////////////////////////////////

	KOR_FORCEINLINE TWeakPtr(_NShared::SNullType* = nullptr) {}
	KOR_FORCEINLINE TWeakPtr(_NShared::CReferencerBase& ref) : _referencerProxy(&ref) { _referencerProxy.AddWeak();}

	// Copy/Move constructors
	/////////////////////////////////

	KOR_FORCEINLINE TWeakPtr(const TWeakPtr& other) { ReplaceBy(other); }
	KOR_FORCEINLINE TWeakPtr(TWeakPtr&& other) noexcept { ReplaceBy(Forward<TWeakPtr>(other)); }

	KOR_FORCEINLINE explicit TWeakPtr(const TSharedPtr<T>& other) { ReplaceBy(other); }
	KOR_FORCEINLINE explicit TWeakPtr(TSharedPtr<T>&& other) noexcept { ReplaceBy(Forward<TSharedPtr<T>>(other)); }

	// Destructor
	/////////////////////////////////

	KOR_FORCEINLINE ~TWeakPtr() { Reset(); }

	// Conversion operators
	/////////////////////////////////

	KOR_FORCEINLINE operator bool() const { return IsValid(); }

	template<typename OtherT, typename TEnableIf<TIsDerivedFrom<OtherT, T>::Value>::Type* = nullptr>
	KOR_FORCEINLINE operator TWeakPtr<OtherT>() const { return _referencerProxy.IsValid() ? TWeakPtr<OtherT>(*_referencerProxy) : nullptr; }

	template<typename OtherT, typename TEnableIf<TIsBaseOf<OtherT, T>::Value>::Type* = nullptr>
	KOR_FORCEINLINE explicit operator TWeakPtr<OtherT>() const { return _referencerProxy.IsValid() ? TWeakPtr<OtherT>(*_referencerProxy) : nullptr; }

	// Comparison operators [WeakPtr]
	/////////////////////////////////

	KOR_FORCEINLINE bool operator==(const TWeakPtr& other) const	{ return _referencerProxy == other._referencerProxy; }
	KOR_FORCEINLINE bool operator!=(const TWeakPtr& other) const	{ return !operator==(other); }

	// Comparison operators [SharedPtr]
	/////////////////////////////////

	KOR_FORCEINLINE bool operator==(const TSharedPtr<T>& other) const	{ return _referencerProxy == other._referencerProxy; }
	KOR_FORCEINLINE bool operator!=(const TSharedPtr<T>& other) const	{ return !operator==(other); }

	// Assignment operators
	/////////////////////////////////

	KOR_FORCEINLINE TWeakPtr& operator=(const _NShared::SNullType*) { Reset(); return *this; }

	// Assignment operators [WeakPtr]
	/////////////////////////////////

	KOR_FORCEINLINE TWeakPtr& operator=(const TWeakPtr& other) { if (&other != this) ReplaceBy(other); return *this; }
	KOR_FORCEINLINE TWeakPtr& operator=(TWeakPtr&& other) { if (&other != this) ReplaceBy(Forward<TWeakPtr>(other)); return *this; }

	// Assignment operators [SharedPtr]
	/////////////////////////////////

	KOR_FORCEINLINE TWeakPtr& operator=(const TSharedPtr<T>& other) { ReplaceBy(other); return *this; }
	KOR_FORCEINLINE TWeakPtr& operator=(TSharedPtr<T>&& other) { ReplaceBy(Forward<TSharedPtr<T>>(other)); return *this; }

	// Pointer operators
	/////////////////////////////////
	// * Our weak pointer supports dereferencing without shared_ptr

	KOR_FORCEINLINE ObjectType* operator->() const { return Get(); }
	KOR_FORCEINLINE ObjectType& operator*() const { return *Get(); }

	// Validity
	/////////////////////////////////

	KOR_FORCEINLINE bool IsValid() const { return _referencerProxy.IsSafeToDereference(); }

	// Getters
	/////////////////////////////////

	KOR_FORCEINLINE ObjectType* Get() const { return _referencerProxy.IsValid() ? _referencerProxy->GetObject<ObjectType>() : nullptr; }
	KOR_FORCEINLINE ObjectType& GetRef() const { return *Get(); }

	// Other
	/////////////////////////////////

	KOR_FORCEINLINE void Reset() { _referencerProxy.RemoveWeak(); _referencerProxy.Set(nullptr); }

	KOR_FORCEINLINE TSharedPtr<T> Pin() const { return IsValid() ? TSharedPtr<T>(_referencerProxy) : TSharedPtr<T>(); }

private:

	// const PtrType&
	// * Copy
	template<typename PtrType>
	KOR_FORCEINLINE_DEBUGGABLE void ReplaceBy(const PtrType& other)
	{
		// How it should work ? (Copy implementation)

		// * WeakPtr as argument:
		// ** 1) Remove weak reference from current
		// ** 2) Add weak reference to other
		// ** 3) Replace referencer

		// * SharedPtr as argument:
		// ** 1) Remove weak reference from current
		// ** 2) Add weak reference to other
		// ** 3) Replace referencer

		_referencerProxy.RemoveWeak(); // 1
		other._referencerProxy.AddWeak(); // 2
		_referencerProxy = other._referencerProxy; // 3
	}

	// PtrType&&
	// * Move
	template<typename PtrType>
	KOR_FORCEINLINE_DEBUGGABLE void ReplaceBy(PtrType&& other)
	{
		// How it should work ? (move implementation)

		// * WeakPtr as argument:
		// ** 1) Remove weak reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer

		// * SharedPtr as argument:
		// ** We MUST CHECK that reference of other does not get destroyed (if valid before passing)
		// ** -2) Add weak reference to other
		// ** -1) Remove shared reference from other
		// ** 1) Remove weak reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer

		// * Only SharedPtr
		if(TIsSame<typename TRemoveReference<PtrType>::Type, TSharedPtr<T>>::Value)
		{
			other._referencerProxy.AddWeak(); // -2
			other._referencerProxy.RemoveShared(); // -1
		}

		_referencerProxy.RemoveWeak(); // 1
		_referencerProxy = other._referencerProxy; // 2

		other._referencerProxy.Set(nullptr); // 3
	}

	mutable _NShared::SReferencerProxy _referencerProxy = nullptr;
};

// Archive operator<< && operator>>
////////////////////////////////////////////

template<typename T>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const TWeakPtr<T>& weakPtr)
{
	if (weakPtr.IsValid())
	{
		ar << weakPtr.Pin().GetRef();
	}

	return ar;
}

template<typename T>
KOR_FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, TWeakPtr<T>& weakPtr)
{
	if (weakPtr.IsValid())
	{
		ar >> weakPtr.Pin().GetRef();
	}

	return ar;
}

template<typename T>
class TSharedClass
{
public:
	// Typedefs
	/////////////////////////////////

	typedef T ClassType;

	// Constructors
	/////////////////////////////////

	KOR_FORCEINLINE TSharedClass() : _isSharedInitialized(false) {}

	// Getters
	/////////////////////////////////

	KOR_FORCEINLINE bool IsSharedInitialized() const { return _isSharedInitialized; }

	// External method
	/////////////////////////////////

	// Gets pointer as shared_ptr
	KOR_FORCEINLINE TSharedPtr<ClassType> AsShared()
	{
		KOR_CHECK_RET(_weakThis.IsValid(), nullptr);
		return _weakThis.Pin();
	}

	// Gets pointer as shared_ptr with provided type
	template<typename ChildType>
	KOR_FORCEINLINE TSharedPtr<ChildType> AsShared()
	{
		KOR_CHECK_RET(_weakThis.IsValid(), nullptr);
		return _weakThis.Pin();
	}

private:
	// Do not call this method DIRECTLY!
	KOR_FORCEINLINE void Init_Private(const TSharedPtr<ClassType>& ptr)
	{
		KOR_CHECK_RET(!_isSharedInitialized);

		_weakThis = ptr;
		_isSharedInitialized = true;
	}

	mutable TWeakPtr<ClassType> _weakThis;
	bool _isSharedInitialized;
};

template<typename T, typename InstanceT = T>
KOR_FORCEINLINE_DEBUGGABLE static TSharedPtr<T> MakeShareable(InstanceT* instance)
{
	static_assert(TIsDerivedFrom<InstanceT,T>::Value, "Instance type must be derived from return type");

	_NShared::CReferencerBase* referencer = _NShared::NewCustomReferencer(instance);
	TSharedPtr<T> newShared = TSharedPtr<T>(*referencer);
	if constexpr (_NShared::TIsSharedClassType<T>::Value)
	{
		newShared->Init_Private(TSharedPtr<typename T::ClassType>(referencer));
	}

	return newShared;
}

template<typename T, typename... ArgTypes>
KOR_FORCEINLINE_DEBUGGABLE static TSharedPtr<T> MakeShared(ArgTypes&&... args)
{
	if constexpr (sizeof...(args) > 0)
	{
		return MakeShareable<T>(new T(Forward<ArgTypes>(args)...));
	}
	else
	{
		return MakeShareable<T>(new T());
	}
}

KOR_NAMESPACE_END