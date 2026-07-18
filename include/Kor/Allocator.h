// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/MemoryOps.h"

KOR_NAMESPACE_BEGIN

// Main allocator used by TArray
// * Has data inlined
class CAllocator
{
public:
	using SizeType = int32;

	// Manipulation
	/////////////////////////////////

	void* Allocate(SizeType bytes, SizeType alignment)
	{
		ElementType* newData = _data
			? SMemoryOps::ReallocAs<ElementType>(_data, _size + num)
			: SMemoryOps::MallocAs<ElementType>(_size + num);

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
			SMemoryOps::Free(_data);

			_data = nullptr;
			_size = 0;
		}
	}

private:

	ElementType* _data = nullptr;
	SizeType _size = 0;
};

template<>
struct TAllocatorTraits<CAllocator> : TAllocatorTraitsBase<CAllocator>
{
	using SizeType = typename CAllocator::SizeType;
};

KOR_NAMESPACE_END