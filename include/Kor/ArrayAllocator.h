// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "ASTD/Memory.h"

// Main allocator used by TArray
// * Has data inlined
template<typename ElementT>
class TArrayAllocator
{
public:

	// Types
	/////////////////////////////////

	typedef ElementT ElementType;
	typedef int64 SizeType;

	// Constructor
	/////////////////////////////////

	FORCEINLINE TArrayAllocator() = default;

	// Destructor
	/////////////////////////////////

	FORCEINLINE ~TArrayAllocator() { Release(); }

	// Getters
	/////////////////////////////////

	// Gets/Sets allocated data
	FORCEINLINE ElementType* GetData() const { return _data; }
	FORCEINLINE void SetData(ElementType* data) { _data = data; }

	// Gets/Sets allocated count
	FORCEINLINE SizeType GetSize() const { return _size; }
	FORCEINLINE void SetSize(SizeType count) { _size = count; }

	// Manipulation
	/////////////////////////////////

	// Allocates new elements
	// @param - element size
	// @param - how many of elements should be allocated
	// @return - array of new elements
	ElementType* Allocate(SizeType num)
	{
		if (num <= 0) return nullptr;

		ElementType* newData = _data
			? SMemory::ReallocTyped<ElementType>(_data, _size + num)
			: SMemory::MallocTyped<ElementType>(_size + num);

		ElementType* elementPtr = newData + _size;

		_data = newData;
		_size += num;

		return elementPtr;
	}

	// Releases resources
	void Release()
	{
		if(_data)
		{
			SMemory::Free(_data);

			_data = nullptr;
			_size = 0;
		}
	}

private:

	ElementType* _data = nullptr;
	SizeType _size = 0;
};
