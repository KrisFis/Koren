// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/ArrayAllocator.h"

KOR_NAMESPACE_BEGIN

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

	KOR_FORCEINLINE ElementType* GetData() const { return _allocator.GetData(); }
	KOR_FORCEINLINE void SetData(ElementType* data) { _allocator.SetData(data); }
	KOR_FORCEINLINE SizeType GetCount() const { return _allocator.GetSize(); }
	KOR_FORCEINLINE void GetCount(SizeType count) { _allocator.SetCount(count); }

	// Manipulation
	/////////////////////////////////

	KOR_FORCEINLINE ElementType* Allocate(SizeType num)
	{
		KOR_CHECK_RET(GetCount() + num <= InNumLimit, nullptr);
		return _allocator.Allocate(num);
	}

	KOR_FORCEINLINE void Release() { _allocator.Release(); }

private:

	TArrayAllocator<ElementType> _allocator = {};
};

KOR_NAMESPACE_END