// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

namespace Internal::Memory
{
	struct SAlignHeader
	{
		KOR_FORCEINLINE static SAlignHeader* Get(void* alignedPtr) noexcept
		{
			return (SAlignHeader*)alignedPtr - 1;
		}

		void* RawPointer = nullptr;
		uint64 Size = 0;
	};

	KOR_FORCEINLINE uint64 GetAlignedSize(uint64 size, uint64 alignment) noexcept
	{
		return size + alignment - 1 + sizeof(SAlignHeader);
	}

	KOR_FORCEINLINE void* GetAlignedPtr(void* ptr, uint64 alignment) noexcept
	{
		const uintptr afterHeader = (uintptr)ptr + sizeof(SAlignHeader);
		const uintptr alignedAddr = (afterHeader + alignment - 1) & ~((uintptr)alignment - 1);

		return (void*)alignedAddr;
	}

	KOR_FORCEINLINE void InitAlignHeader(void* alignPtr, void* rawPtr, uint64 size) noexcept
	{
		SAlignHeader* header = SAlignHeader::Get(alignPtr);
		header->RawPointer = rawPtr;
		header->Size = size;
	}
}

KOR_FORCEINLINE void SMemoryOps::Free(void* ptr) noexcept
{
	return SPlatformMemoryOps::Free(ptr);
}

KOR_FORCEINLINE void SMemoryOps::Free(void* ptr, uint64 alignment) noexcept
{
	using namespace Internal::Memory;

	KOR_ASSERT_DEBUG(SMath::IsPowerOfTwo(alignment));

	if (alignment <= KOR_DEFAULT_HEAP_ALIGNMENT)
	{
		SPlatformMemoryOps::Free(ptr);
		return;
	}

	KOR_ASSERT_DEBUG(ptr);
	void* rawPtr = SAlignHeader::Get(ptr)->RawPointer;
	SPlatformMemoryOps::Free(rawPtr);
}

KOR_FORCEINLINE void* SMemoryOps::Malloc(uint64 size) noexcept
{
	return SPlatformMemoryOps::Malloc(size);
}

KOR_INLINE void* SMemoryOps::Malloc(uint64 size, uint64 alignment) noexcept
{
	using namespace Internal::Memory;

	KOR_ASSERT_DEBUG(SMath::IsPowerOfTwo(alignment));

	if (alignment <= KOR_DEFAULT_HEAP_ALIGNMENT)
	{
		return SPlatformMemoryOps::Malloc(size);
	}

	const uint64 alignedSize = GetAlignedSize(size, alignment);
	void* rawPtr = SPlatformMemoryOps::Malloc(alignedSize);
	if (!rawPtr) return nullptr;

	void* alignedPtr = GetAlignedPtr(rawPtr, alignment);
	InitAlignHeader(alignedPtr, rawPtr, size);

	return alignedPtr;
}

template<typename T>
KOR_FORCEINLINE T* SMemoryOps::MallocAs(uint64 num) noexcept
{
	return (T*)Malloc(num * sizeof(T), alignof(T));
}

template<typename T>
KOR_FORCEINLINE T* SMemoryOps::MallocAs(uint64 num, uint64 alignment) noexcept
{
	return (T*)Malloc(num * sizeof(T), alignment);
}

KOR_FORCEINLINE void* SMemoryOps::Calloc(uint64 size) noexcept
{
	return SPlatformMemoryOps::Calloc(size);
}

KOR_INLINE void* SMemoryOps::Calloc(uint64 size, uint64 alignment) noexcept
{
	using namespace Internal::Memory;

	KOR_ASSERT_DEBUG(SMath::IsPowerOfTwo(alignment));

	if (alignment <= KOR_DEFAULT_HEAP_ALIGNMENT)
	{
		return SPlatformMemoryOps::Calloc(size);
	}

	const uint64 alignedSize = GetAlignedSize(size, alignment);
	void* rawPtr = SPlatformMemoryOps::Calloc(alignedSize);
	if (!rawPtr) return nullptr;

	void* alignedPtr = GetAlignedPtr(rawPtr, alignment);
	InitAlignHeader(alignedPtr, rawPtr, size);

	return alignedPtr;
}

template<typename T>
KOR_FORCEINLINE T* SMemoryOps::CallocAs(uint64 num) noexcept
{
	return (T*)Calloc(num * sizeof(T), alignof(T));
}

template<typename T>
KOR_FORCEINLINE T* SMemoryOps::CallocAs(uint64 num, uint64 alignment) noexcept
{
	return (T*)Calloc(num * sizeof(T), alignment);
}

KOR_FORCEINLINE void* SMemoryOps::Realloc(void* ptr, uint64 size) noexcept
{
	return SPlatformMemoryOps::Realloc(ptr, size);
}

KOR_INLINE void* SMemoryOps::Realloc(void* ptr, uint64 size, uint64 alignment) noexcept
{
	using namespace Internal::Memory;

	KOR_ASSERT_DEBUG(SMath::IsPowerOfTwo(alignment));

	if (alignment <= KOR_DEFAULT_HEAP_ALIGNMENT)
		return SPlatformMemoryOps::Realloc(ptr, size);

	if (!ptr)
		return Malloc(size, alignment);

	const SAlignHeader oldHeader = *SAlignHeader::Get(ptr);
	const int64 oldOffset = KOR_PTR_DIFF(int64, ptr, oldHeader.RawPointer);

	const uint64 newAlignedSize = GetAlignedSize(size, alignment);
	void* newRawPtr = SPlatformMemoryOps::Realloc(oldHeader.RawPointer, newAlignedSize);
	if (!newRawPtr)
	{
		return nullptr;
	}

	void* newAlignedPtr = GetAlignedPtr(newRawPtr, alignment);
	const int64 newOffset = KOR_PTR_DIFF(int64, newAlignedPtr, newRawPtr);

	// The raw->aligned offset is a function of the raw address itself,
	// so it can shift even when alignment is unchanged. If it did,
	// slide the data back onto the correctly aligned position.
	if (newOffset != oldOffset)
	{
		void* oldDataInNewBlock = (uint8*)newRawPtr + oldOffset;
		const uint64 bytesToMove = oldHeader.Size < size ? oldHeader.Size : size;
		SPlatformMemoryOps::Move(newAlignedPtr, oldDataInNewBlock, bytesToMove);
	}

	// Header must be written LAST — the move above can overlap the
	// header's own memory region until data lands in its final spot.
	InitAlignHeader(newAlignedPtr, newRawPtr, size);

	return newAlignedPtr;
}

template<typename T>
KOR_FORCEINLINE T* SMemoryOps::ReallocAs(T* ptr, uint64 num) noexcept
{
	return (T*)SPlatformMemoryOps::Realloc(ptr, num * sizeof(T), alignof(T));
}

template<typename T>
KOR_FORCEINLINE T* SMemoryOps::ReallocAs(T* ptr, uint64 num, uint64 alignment) noexcept
{
	return (T*)SPlatformMemoryOps::Realloc(ptr, num * sizeof(T), alignment);
}

KOR_FORCEINLINE void* SMemoryOps::Copy(void* dest, const void* src, uint64 size) noexcept
{
	return SPlatformMemoryOps::Copy(dest, src, size);
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::CopyAs(T* to, const T* from, uint64 num) noexcept
{
	if constexpr (!TTypeTraits<T>::IsBitwiseCopyable)
	{
		while (num-- > 0)
		{
			::new((void*) to) T(*from);

			++to;
			++from;
		}
	} else
	{
		SPlatformMemoryOps::Copy(
			to,
			from,
			sizeof(T) * num
		);
	}
}

KOR_FORCEINLINE void* SMemoryOps::Move(void* dest, const void* src, uint64 size) noexcept
{
	return SPlatformMemoryOps::Move(dest, src, size);
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::MoveAs(T* to, T* from) noexcept
{
	if constexpr (!TTypeTraits<T>::IsBitwiseMovable)
	{
		::new((void*) to) T(*from);
	} else
	{
		SPlatformMemoryOps::Move(
			to,
			from,
			sizeof(T)
		);
	}
}

KOR_FORCEINLINE void* SMemoryOps::Fill(void* dest, int32 val, uint64 size) noexcept
{
	return SPlatformMemoryOps::Fill(dest, val, size);
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::FillAs(const T* dst, T val, uint64 num) noexcept
{
	if constexpr (!TTypeTraits<T>::IsBitwiseCopyable)
	{
		while (num-- > 0)
		{
			::new((void*) dst) T(val);
			++dst;
		}
	} else
	{
		SPlatformMemoryOps::Fill(
			dst,
			val,
			sizeof(T) * num
		);
	}
}

KOR_FORCEINLINE void* SMemoryOps::Zero(void* dest, uint64 size) noexcept
{
	return SPlatformMemoryOps::Zero(dest, size);
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::ZeroAs(const T* dst, uint64 num) noexcept
{
	if constexpr (!TTypeTraits<T>::IsBitwiseCopyable)
	{
		while (num-- > 0)
		{
			::new((void*) dst) T();
			++dst;
		}
	} else
	{
		SPlatformMemoryOps::Zero(
			dst,
			sizeof(T) * num
		);
	}
}

KOR_FORCEINLINE int32 SMemoryOps::Compare(const void* lhs, const void* rhs, uint64 size) noexcept
{
	return SPlatformMemoryOps::Compare(lhs, rhs, size);
}

template<typename T>
KOR_FORCEINLINE int32 SMemoryOps::CompareAs(const T* lhs, const T* rhs, uint64 num) noexcept
{
	if constexpr (!TTypeTraits<T>::IsBitwiseComparable)
	{
		while (num-- > 0)
		{
			if (*lhs < *rhs) return -1;
			if (*rhs < *lhs) return 1;
			++lhs;
			++rhs;
		}
		return 0;
	} else
	{
		return SPlatformMemoryOps::Compare(lhs, rhs, sizeof(T) * num);
	}
}

KOR_FORCEINLINE bool SMemoryOps::IsEqual(const void* lhs, const void* rhs, uint64 size) noexcept
{
	return SPlatformMemoryOps::Compare(lhs, rhs, size) == 0;
}

template<typename T>
KOR_FORCEINLINE bool SMemoryOps::IsEqualAs(const T* lhs, const T* rhs, uint64 num) noexcept
{
	if constexpr (!TTypeTraits<T>::IsBitwiseComparable)
	{
		while (num-- > 0)
		{
			if (!(*lhs == *rhs)) return false;

			++lhs;
			++rhs;
		}

		return true;
	} else
	{
		return SPlatformMemoryOps::Compare(
			lhs,
			rhs,
			sizeof(T) * num
		) == 0;
	}
}

template<typename T, typename... ArgTypes>
KOR_FORCEINLINE void SMemoryOps::Construct(T* ptr, ArgTypes&&... Args) noexcept
{
	if constexpr (!TIsTriviallyConstructible<T, ArgTypes...>::Value)
	{
		::new((void*) ptr) T(Forward<ArgTypes>(Args)...);
	}
	else
	{
		SPlatformMemoryOps::Zero(ptr, sizeof(T));
	}
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::Destruct(T* ptr) noexcept
{
	if constexpr (!TIsTriviallyDestructible<T>::Value)
	{
		ptr->~T();
	}
}
