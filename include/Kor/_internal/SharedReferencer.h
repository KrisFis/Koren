// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Build.h"
#include "Kor/Assert.h"

// TODO(jkfisera): REIMPLEMENT Invoke
#include <functional>

KOR_NAMESPACE_BEGIN

namespace _NShared
{
	struct SNullType {};

	class CReferencerBase
	{
	public:
		virtual ~CReferencerBase() = default;

		// Getters
		/////////////////////////////////

		KOR_FORCEINLINE bool HasAnyReference() const { return _sharedNum + _weakCount > 0; }
		KOR_FORCEINLINE uint16 GetSharedNum() const { return _sharedNum; }
		KOR_FORCEINLINE uint16 GetWeakCount() const { return _weakCount; }

		template<typename T>
		KOR_FORCEINLINE T* GetObject() const { return reinterpret_cast<T*>(GetObjectImpl()); }

		KOR_FORCEINLINE bool HasObject() const { return GetObjectImpl() != nullptr; }

		// Setters [Add]
		/////////////////////////////////

		KOR_FORCEINLINE void AddShared()
		{
			KOR_ASSERT(_sharedNum < UINT16_MAX);
			++_sharedNum;
		}

		KOR_FORCEINLINE void AddWeak()
		{
			KOR_ASSERT(_weakCount < UINT16_MAX); // overflow
			++_weakCount;
		}

		// Setters [REMOVE]
		/////////////////////////////////

		KOR_FORCEINLINE void RemoveShared()
		{
			KOR_ASSERT(_sharedNum > 0); // underflow
			if(_sharedNum == 1) DeconstructObjectImpl();
			--_sharedNum;
		}

		KOR_FORCEINLINE void RemoveWeak()
		{
			KOR_ASSERT(_weakCount > 0); // underflow
			--_weakCount;
		}

	protected:

		virtual void* GetObjectImpl() const = 0;
		virtual void DeconstructObjectImpl() = 0;

		uint16 _sharedNum = 0;
		uint16 _weakCount = 0;
	};

	template<typename T, typename DeleterT>
	class TCustomReferencer : public CReferencerBase
	{
	public:
		// Typedefs
		/////////////////////////////////

		typedef T ObjectType;
		typedef DeleterT DeleterType;

		// Constructors
		/////////////////////////////////

		TCustomReferencer() = delete;

		template<typename OtherDeleterT>
		KOR_FORCEINLINE TCustomReferencer(ObjectType* object, OtherDeleterT&& deleter)
			: CReferencerBase()
			, _object(object)
			, _deleter(MoveIfPossible(deleter))
		{}

		virtual ~TCustomReferencer() override {	DestructObjectImpl(); }

	protected:

		// ~BEGIN CReferencerBase interface
		KOR_FORCEINLINE virtual void* GetObjectImpl() const override { return _object; }
		KOR_FORCEINLINE virtual void DeconstructObjectImpl() override { DestructObjectImpl(); }
		// ~END CReferencerBase interface

	private: // Fields
		void DestructObjectImpl()
		{
			if(_object)
			{
				std::invoke(_deleter, _object);
				_object = nullptr;
			}
		}

		ObjectType* _object;
		DeleterType _deleter;
	};

	template<typename ObjectT>
	void DefaultDeleteObjectFunc(ObjectT* obj)
	{
		delete obj;
	}

	template<typename ObjectType, typename DeleterType>
	KOR_FORCEINLINE static CReferencerBase* NewCustomReferencerWithDeleter(ObjectType* obj, DeleterType&& deleter)
	{
		return new TCustomReferencer<ObjectType, DeleterType>(obj, MoveIfPossible(deleter));
	}

	template<typename ObjectType>
	KOR_FORCEINLINE_DEBUGGABLE static CReferencerBase* NewCustomReferencer(ObjectType* obj)
	{
		return NewCustomReferencerWithDeleter(obj, &DefaultDeleteObjectFunc<ObjectType>);
	}

	KOR_FORCEINLINE static void DeleteReferencer(CReferencerBase* referencer)
	{
		delete referencer;
	}

	// Contains helper methods for referencer
	// * Should be used internally
	// * Handles even deconstruction of referencer
	struct SReferencerProxy
	{
		// Constructors
		/////////////////////////////////

		KOR_FORCEINLINE SReferencerProxy(CReferencerBase* InReferencer)
			: _inner(InReferencer)
		{}

		// Compare operators
		/////////////////////////////////

		KOR_FORCEINLINE bool operator==(const SReferencerProxy& other) const { return _inner == other._inner; }
		KOR_FORCEINLINE bool operator!=(const SReferencerProxy& other) const { return !operator==(other); }

		// Pointer operators
		/////////////////////////////////

		KOR_FORCEINLINE CReferencerBase* operator->() { return Get(); }
		KOR_FORCEINLINE const CReferencerBase* operator->() const { return Get(); }

		KOR_FORCEINLINE CReferencerBase& operator*() { return *Get(); }
		KOR_FORCEINLINE const CReferencerBase& operator*() const { return *Get(); }

		// Checkers
		/////////////////////////////////

		KOR_FORCEINLINE bool IsValid() const { return _inner != nullptr; }
		KOR_FORCEINLINE bool IsUnique() const { return _inner != nullptr && _inner->GetSharedNum() == 1; }
		KOR_FORCEINLINE bool IsSafeToDereference() const { return _inner != nullptr && _inner->GetSharedNum() > 0; }

		// Getters
		/////////////////////////////////

		KOR_FORCEINLINE CReferencerBase* Get() const { return _inner; }

		// Setters
		/////////////////////////////////

		KOR_FORCEINLINE void Set(CReferencerBase* referencer) { _inner = referencer; }

		// Helper methods [Add]
		/////////////////////////////////

		KOR_FORCEINLINE void AddShared()
		{
			if(!IsValid()) return;

			_inner->AddShared();
		}

		KOR_FORCEINLINE void AddWeak()
		{
			if(!IsValid()) return;

			_inner->AddWeak();
		}

		// Helper methods [Remove]
		/////////////////////////////////

		KOR_FORCEINLINE void RemoveShared()
		{
			if(!IsValid()) return;

			_inner->RemoveShared();
			if(!_inner->HasAnyReference())
			{
				DeleteReferencer(_inner);
				_inner = nullptr;
			}
		}

		KOR_FORCEINLINE void RemoveWeak()
		{
			if(!IsValid()) return;

			_inner->RemoveWeak();
			if (!_inner->HasAnyReference()) 
			{
				DeleteReferencer(_inner);
				_inner = nullptr;
			}
		}

	private:

		CReferencerBase* _inner = nullptr;
	};
}

KOR_NAMESPACE_END