// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once // silence tooling

KOR_FORCEINLINE void SMemoryOps::Free(void* ptr) noexcept
{
	return SPlatformMemoryOps::Free(ptr);
}

template<typename T>
KOR_FORCEINLINE T* SMemoryOps::MallocAs(int64 num) noexcept
{
	return (T*)SPlatformMemoryOps::Malloc(num * sizeof(T));
}

KOR_FORCEINLINE void* SMemoryOps::Malloc(int64 size) noexcept
{
	return SPlatformMemoryOps::Malloc(size);
}

template<typename T>
KOR_FORCEINLINE T* SMemoryOps::CallocAs(int64 num) noexcept
{
	return (T*)SPlatformMemoryOps::Calloc(num * sizeof(T));
}

KOR_FORCEINLINE void* SMemoryOps::Calloc(int64 size) noexcept
{
	return SPlatformMemoryOps::Calloc(size);
}

template<typename T>
KOR_FORCEINLINE T* SMemoryOps::ReallocAs(T* ptr, int64 num) noexcept
{
	return (T*)SPlatformMemoryOps::Realloc(ptr, num * sizeof(T));
}

KOR_FORCEINLINE void* SMemoryOps::Realloc(void* ptr, int64 size) noexcept
{
	return SPlatformMemoryOps::Realloc(ptr, size);
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::CopyAs(T* to, const T* from, int64 num) noexcept
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

KOR_FORCEINLINE void* SMemoryOps::Copy(void* dest, const void* src, int64 size) noexcept
{
	return SPlatformMemoryOps::Copy(dest, src, size);
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

KOR_FORCEINLINE void* SMemoryOps::Move(void* dest, const void* src, int64 size) noexcept
{
	return SPlatformMemoryOps::Move(dest, src, size);
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::FillAs(const T* dst, T val, int64 num) noexcept
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

KOR_FORCEINLINE void* SMemoryOps::Fill(void* dest, int32 val, int64 size) noexcept
{
	return SPlatformMemoryOps::Fill(dest, val, size);
}

template<typename T>
KOR_FORCEINLINE void SMemoryOps::ZeroAs(const T* dst, int64 num) noexcept
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

KOR_FORCEINLINE void* SMemoryOps::Zero(void* dest, int64 size) noexcept
{
	return SPlatformMemoryOps::Zero(dest, size);
}

template<typename T>
KOR_FORCEINLINE int32 SMemoryOps::CompareAs(const T* lhs, const T* rhs, int64 num) noexcept
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

KOR_FORCEINLINE int32 SMemoryOps::Compare(const void* lhs, const void* rhs, int64 size) noexcept
{
	return SPlatformMemoryOps::Compare(lhs, rhs, size);
}

template<typename T>
KOR_FORCEINLINE bool SMemoryOps::IsEqualAs(const T* lhs, const T* rhs, int64 num) noexcept
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

KOR_FORCEINLINE bool SMemoryOps::IsEqual(const void* lhs, const void* rhs, int64 size) noexcept
{
	return SPlatformMemoryOps::Compare(lhs, rhs, size) == 0;
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