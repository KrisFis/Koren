// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "ASTD/ArrayAllocator.h"

// Default array allocator but with fixed num of elements
template<typename ElementT, uint32 InNumLimit>
class TFixedArrayAllocator
{
public:
	// Typedef
	/////////////////////////////////

	typedef ElementT ElementType;
	typedef typename TArrayAllocator<ElementType>::SizeType SizeType;

	// Getters
	/////////////////////////////////

	FORCEINLINE ElementType* GetData() const { return _allocator.GetData(); }
	FORCEINLINE void SetData(ElementType* data) { _allocator.SetData(data); }
	FORCEINLINE SizeType GetCount() const { return _allocator.GetSize(); }
	FORCEINLINE void GetCount(SizeType count) { _allocator.SetCount(count); }

	// Manipulation
	/////////////////////////////////

	FORCEINLINE ElementType* Allocate(SizeType num)
	{
		CHECK_RET(GetCount() + num <= InNumLimit, nullptr);
		return _allocator.Allocate(num);
	}

	FORCEINLINE void Release() { _allocator.Release(); }

private:

	TArrayAllocator<ElementType> _allocator = {};
};
