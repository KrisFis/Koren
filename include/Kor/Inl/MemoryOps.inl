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

	KOR_FORCEINLINE uint64 GetAlignedSize(uint64 size, uint32 alignment) noexcept
	{
		return size + alignment - 1 + sizeof(SAlignHeader);
	}

	KOR_FORCEINLINE void* GetAlignedPtr(void* ptr, uint32 alignment) noexcept
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

KOR_INLINE void SMemoryOps::Free(void* ptr, uint32 alignment) noexcept
{
	using namespace Internal::Memory;

	KOR_ASSERT_DEBUG(SMath::IsPowerOfTwo(alignment));

	if (alignment <= KOR_DEFAULT_HEAP_ALIGNMENT)
	{
		SPlatformMemoryOps::Free(ptr);
		return;
	}

	KOR_ASSERT(ptr);
	void* rawPtr = SAlignHeader::Get(ptr)->RawPointer;
	SPlatformMemoryOps::Free(rawPtr);
}

KOR_FORCEINLINE void* SMemoryOps::Malloc(uint64 size) noexcept
{
	return SPlatformMemoryOps::Malloc(size);
}

KOR_INLINE void* SMemoryOps::Malloc(uint64 size, uint32 alignment) noexcept
{
	using namespace Internal::Memory;

	KOR_ASSERT(SMath::IsPowerOfTwo(alignment));

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
KOR_FORCEINLINE T* SMemoryOps::MallocAs(uint64 num, uint32 alignment) noexcept
{
	return (T*)Malloc(num * sizeof(T), alignment);
}

KOR_FORCEINLINE void* SMemoryOps::Calloc(uint64 size) noexcept
{
	return SPlatformMemoryOps::Calloc(size);
}

KOR_INLINE void* SMemoryOps::Calloc(uint64 size, uint32 alignment) noexcept
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
KOR_FORCEINLINE T* SMemoryOps::CallocAs(uint64 num, uint32 alignment) noexcept
{
	return (T*)Calloc(num * sizeof(T), alignment);
}

KOR_FORCEINLINE void* SMemoryOps::Realloc(void* ptr, uint64 size) noexcept
{
	return SPlatformMemoryOps::Realloc(ptr, size);
}

KOR_INLINE void* SMemoryOps::Realloc(void* ptr, uint64 size, uint32 alignment) noexcept
{
	using namespace Internal::Memory;

	KOR_ASSERT_DEBUG(SMath::IsPowerOfTwo(alignment));

	if (alignment <= KOR_DEFAULT_HEAP_ALIGNMENT)
	{
		return SPlatformMemoryOps::Realloc(ptr, size);
	}

	if (!ptr)
	{
		return Malloc(size, alignment);
	}

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
KOR_FORCEINLINE T* SMemoryOps::ReallocAs(T* ptr, uint64 num, uint32 alignment) noexcept
{
	return (T*)SPlatformMemoryOps::Realloc(ptr, num * sizeof(T), alignment);
}

KOR_FORCEINLINE void* SMemoryOps::Copy(void* dest, const void* src, uint64 size) noexcept
{
	return SPlatformMemoryOps::Copy(dest, src, size);
}

template<typename T> 
KOR_FORCEINLINE void SMemoryOps::CopyConstruct(T* dest, const T* src, uint64 num) noexcept
{
	if constexpr (!TIsTriviallyCopyable<T>::Value)
	{
		while (num-- > 0)
		{
			::new((void*) dest) T(*src);

			++dest;
			++src;
		}
	} 
	else
	{
		SPlatformMemoryOps::Copy(dest, src, sizeof(T) * num);
	}
}

template<typename T> 
KOR_FORCEINLINE void SMemoryOps::CopyAssign(T* dest, const T* src, uint64 num) noexcept
{
	if constexpr (!TIsTriviallyCopyable<T>::Value)
	{
		while (num-- > 0)
		{
			*dest = ::Move(*src);
			++dest;
			++src;
		}
	} 
	else
	{
		SPlatformMemoryOps::Copy(dest, src, sizeof(T) * num);
	}
}

KOR_FORCEINLINE void* SMemoryOps::Move(void* dest, const void* src, uint64 size) noexcept
{
	return SPlatformMemoryOps::Move(dest, src, size);
}

template<typename T> 
KOR_FORCEINLINE void SMemoryOps::MoveConstruct(T* dest, T* src, uint64 num) noexcept
{
	if constexpr (!TIsTriviallyMovable<T>::Value)
	{
		if (dest < src)
		{
			while (num-- > 0)
			{
				::new ((void*)dest) T(::Move(*src));
				++dest;
				++src;
			}
		}
		else if (dest > src)
		{
			dest += num;
			src += num;

			while (num-- > 0)
			{
				--dest;
				--src;
				::new ((void*)dest) T(::Move(*src));
			}
		}
	} 
	else
	{
		SPlatformMemoryOps::Move(dest, src, sizeof(T) * num);
	}
}

template<typename T> 
KOR_FORCEINLINE void SMemoryOps::MoveAssign(T* dest, T* src, uint64 num) noexcept
{
if constexpr (!TIsTriviallyMovable<T>::Value)
	{
		if (dest < src)
		{
			while (num-- > 0)
			{
				*dest = ::Move(*src);
				++dest;
				++src;
			}
		}
		else if (dest > src)
		{
			dest += num;
			src += num;

			while (num-- > 0)
			{
				--dest;
				--src;
				*dest = ::Move(*src);
			}
		}
	} 
	else
	{
		SPlatformMemoryOps::Move(dest, src, sizeof(T) * num);
	}
}

KOR_FORCEINLINE void* SMemoryOps::Fill(void* ptr, uint8 val, uint64 size) noexcept
{
	return SPlatformMemoryOps::Fill(ptr, val, size);
}

KOR_FORCEINLINE void* SMemoryOps::Fill(void* ptr, int8 val, uint64 size) noexcept
{
	return SPlatformMemoryOps::Fill(ptr, *(uint8*)&val, size);
}

template<typename T> 
KOR_FORCEINLINE void SMemoryOps::FillConstruct(T* ptr, const T& val, uint64 num) noexcept
{
	// Bitwise-fill can only take 1 byte (although Win and POSIX api takes 4 bytes)
	if constexpr (!(TIsTriviallyCopyable<T>::Value && sizeof(T) == 1))
	{
		while (num-- > 0)
		{
			*ptr = val;
			++ptr;
		}
	} 
	else
	{
		SPlatformMemoryOps::Fill(ptr, *(uint8*)&val, sizeof(T) * num);
	}
}

template<typename T> 
KOR_FORCEINLINE void SMemoryOps::FillAssign(T* ptr, const T& val, uint64 num) noexcept
{
	// Bitwise-fill can only take 1 byte (although Win and POSIX api takes 4 bytes)
	if constexpr (!(TIsTriviallyCopyable<T>::Value && sizeof(T) == 1))
	{
		while (num-- > 0)
		{
			*ptr = val;
			++ptr;
		}
	} 
	else
	{
		SPlatformMemoryOps::Fill(ptr, (int32)*(uint8*)&val, sizeof(T) * num);
	}
}

KOR_FORCEINLINE void* SMemoryOps::Zero(void* ptr, uint64 size) noexcept
{
	return SPlatformMemoryOps::Zero(dest, size);
}

template<typename T> 
KOR_FORCEINLINE void SMemoryOps::ZeroConstruct(T* ptr, uint64 num) noexcept
{
	if constexpr (!TIsTriviallyDefaultConstructible<T>::Value)
	{
		while (num-- > 0)
		{
			::new((void*) ptr) T();
			++ptr;
		}
	} 
	else
	{
		SPlatformMemoryOps::Zero(ptr, sizeof(T) * num);
	}
}

template<typename T> 
KOR_FORCEINLINE void SMemoryOps::ZeroAssign(T* ptr, uint64 num) noexcept
{
	if constexpr (!TIsTriviallyDefaultConstructible<T>::Value)
	{
		while (num-- > 0)
		{
			*ptr = T();
			++ptr;
		}
	} 
	else
	{
		SPlatformMemoryOps::Zero(ptr, sizeof(T) * num);
	}
}

KOR_FORCEINLINE void SMemoryOps::Swap(void* lhs, void* rhs, uint64 size) noexcept
{
	uint8 temp[KOR_BUFFER_SIZE_SMALL];

	while (size > 0)
	{
		const uint64 chunk = size > KOR_BUFFER_SIZE_SMALL
			? KOR_BUFFER_SIZE_SMALL
			: size;

		SPlatformMemoryOps::Copy(temp, lhs, chunk);
		SPlatformMemoryOps::Copy(lhs, rhs, chunk);
		SPlatformMemoryOps::Copy(rhs, temp, chunk);

		lhs = (uint8*)lhs + chunk;
		rhs = (uint8*)rhs + chunk;
		size -= chunk;
	}
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::SwapAs(T* lhs, T* rhs, uint64 num) noexcept
{	
	if constexpr (!TIsTriviallyMovable<T>::Value)
	{
		while (num-- > 0)
		{
			T tmp(::Move(*lhs));
			*lhs = ::Move(*rhs);
			*rhs = ::Move(tmp);

			++lhs;
			++rhs;
		}
	}
	else
	{
		Swap(lhs, rhs, num * sizeof(T));
	}
}

KOR_FORCEINLINE int32 SMemoryOps::Compare(const void* lhs, const void* rhs, uint64 size) noexcept
{
	return SPlatformMemoryOps::Compare(lhs, rhs, size);
}

template<typename T>
KOR_FORCEINLINE int32 SMemoryOps::CompareAs(const T* lhs, const T* rhs, uint64 num) noexcept
{
	if constexpr (!TIsFundamental<T>::Value && !TIsEnum<T>::Value)
	{
		while (num-- > 0)
		{
			if (*lhs < *rhs) return -1;
			if (*rhs < *lhs) return 1;
			++lhs;
			++rhs;
		}
		return 0;
	} 
	else
	{
		return Compare(lhs, rhs, sizeof(T) * num);
	}
}

KOR_FORCEINLINE bool SMemoryOps::IsEqual(const void* lhs, const void* rhs, uint64 size) noexcept
{
	return SPlatformMemoryOps::Compare(lhs, rhs, size) == 0;
}

template<typename T>
KOR_FORCEINLINE bool SMemoryOps::IsEqualAs(const T* lhs, const T* rhs, uint64 num) noexcept
{
	if constexpr (!TIsFundamental<T>::Value && !TIsEnum<T>::Value)
	{
		while (num-- > 0)
		{
			if (!(*lhs == *rhs)) return false;

			++lhs;
			++rhs;
		}

		return true;
	} 
	else
	{
		return IsEqual(lhs, rhs, sizeof(T) * num);
	}
}

template<typename T, typename... ArgsT>
KOR_FORCEINLINE void SMemoryOps::Construct(T* ptr, ArgsT&&... Args) noexcept
{
	if constexpr (sizeof...(ArgsT) == 0)
	{
		if constexpr (!TIsTriviallyDefaultConstructible<T>::Value)
		{
			::new((void*) ptr) T();
		}
		else
		{
			SPlatformMemoryOps::Zero(ptr, sizeof(T));
		}
	}
	else
	{
		using FirstArgType = typename TFirstArg<ArgsT...>::Type;
		using CleanFirstArgType = typename TClean<FirstArgType>::Type;
		constexpr bool IsSameAsT = TIsSame<CleanFirstArgType, T>::Value;
		constexpr bool IsRValueArg = !TIsLValue<FirstArgType>::Value;

		if constexpr (sizeof...(ArgsT) == 1 &&
			IsSameAsT &&
			IsRValueArg &&
			TIsTriviallyMovable<T>::Value)
		{
			SPlatformMemoryOps::Move(ptr, (const void*)&Args..., sizeof(T));
		}
		else if constexpr (sizeof...(ArgsT) == 1 &&
			IsSameAsT &&
			TIsTriviallyCopyable<T>::Value)
		{
			SPlatformMemoryOps::Copy(ptr, (const void*)&Args..., sizeof(T));
		}
		else
		{
			::new((void*) ptr) T(Forward<ArgsT>(Args)...);
		}
	}
}


template<typename T>
KOR_FORCEINLINE void SMemoryOps::DefaultConstruct(T* ptr, uint64 num) noexcept
{
	if constexpr (!TIsTriviallyDefaultConstructible<T>::Value)
	{
		while(num-- > 0)
		{
			::new((void*) ptr) T();
			++ptr;
		}
	}
	else
	{
		SPlatformMemoryOps::Zero(ptr, num * sizeof(T));
	}
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::Destruct(T* ptr, uint64 num) noexcept
{
	if constexpr (!TIsTriviallyDestructible<T>::Value)
	{
		while (num-- > 0)
		{
			ptr->~T();
			++ptr;
		}
	}
}
