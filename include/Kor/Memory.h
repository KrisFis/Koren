// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include <new>
#include PLATFORM_HEADER(Memory)

typedef PLATFORM_PREFIXED_TYPE(S, PlatformMemory) SPlatformMemory;
struct SMemory : public SPlatformMemory
{
	static constexpr long double BITS_PER_BYTE = 8; // bits
	static constexpr long double KiB_PER_BYTE = 1.e-3; // kibibytes
	static constexpr long double Kb_PER_BYTE = 7.8e-3; // kilobits
	static constexpr long double KB_PER_BYTE = 1.e-3; // kilobytes
	static constexpr long double MiB_PER_BYTE = 9.53674e-7; // mebibytes
	static constexpr long double Mb_PER_BYTE = 7.629395e-6; // megabits
	static constexpr long double MB_PER_BYTE = 1.e-6; // megabytes
	static constexpr long double GiB_PER_BYTE = 9.31e-10; // gibibytes
	static constexpr long double Gb_PER_BYTE = 7.451e-9; // gigabits
	static constexpr long double GB_PER_BYTE = 1.e-9; // gigabytes
	static constexpr long double TiB_PER_BYTE = 1.e-12; // tebibytes
	static constexpr long double Tb_PER_BYTE = 7.e-12; // terabits
	static constexpr long double TB_PER_BYTE = 1.e-12; // terabytes

	template<typename T>
	FORCEINLINE static T* MallocTyped(int64 num = 1)
	{
		return (T*)SPlatformMemory::Malloc(num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE static T* ReallocTyped(T* ptr, int64 num = 1)
	{
		return (T*)SPlatformMemory::Realloc(ptr, num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE static T* CallocTyped(int64 num = 1)
	{
		return (T*)SPlatformMemory::Calloc(num * sizeof(T));
	}

	template<typename T>
	static void CopyTyped(T* to, const T* from, int64 num = 1)
	{
		if constexpr (!TTypeTraits<T>::IsBitwiseCopyable)
		{
			while(num-- > 0)
			{
				::new((void*)to) T(*from);

				++to;
				++from;
			}
		}
		else
		{
			SPlatformMemory::Copy(
				to,
				from,
				sizeof(T) * num
			);
		}
	}

	template<typename T>
	FORCEINLINE static void MoveTyped(T* to, T* from)
	{
		if constexpr (!TTypeTraits<T>::IsBitwiseMovable)
		{
			::new((void*)to) T(*from);
		}
		else
		{
			SPlatformMemory::Move(
				to,
				from,
				sizeof(T)
			);
		}
	}

	template<typename T>
	FORCEINLINE static void FillTyped(const T* dst, T val, int64 num = 1)
	{
		if constexpr (!TTypeTraits<T>::IsBitwiseCopyable)
		{
			while(num-- > 0)
			{
				::new((void*)dst) T(val);
				++dst;
			}
		}
		else
		{
			SPlatformMemory::Fill(
				dst,
				val,
				sizeof(T) * num
			);
		}
	}

	template<typename T>
	FORCEINLINE static void ZeroTyped(const T* dst, int64 num = 1)
	{
		if constexpr (!TTypeTraits<T>::IsBitwiseCopyable)
		{
			while(num-- > 0)
			{
				::new((void*)dst) T();
				++dst;
			}
		}
		else
		{
			SPlatformMemory::Zero(
				dst,
				sizeof(T) * num
			);
		}
	}

	template<typename T>
	FORCEINLINE static bool IsEqual(const T* lhs, const T* rhs, int64 num = 1)
	{
		if constexpr (!TTypeTraits<T>::IsBitwiseComparable)
		{
			while(num-- > 0)
			{
				if (!(*lhs == *rhs)) return false;

				++lhs;
				++rhs;
			}

			return true;
		}
		else
		{
			return SPlatformMemory::Compare(
				lhs,
				rhs,
				sizeof(T) * num
			);
		}
	}

	template<typename T, typename... ArgTypes>
	FORCEINLINE static void Construct(T* ptr, ArgTypes&&... Args)
	{
		if constexpr (!TIsTriviallyConstructible<T, ArgTypes...>::Value)
		{
			::new((void*)ptr) T(Forward<ArgTypes>(Args)...);
		}
		else
		{
			SPlatformMemory::Zero(ptr, sizeof(T));
		}
	}

	// Destruct element
	template<typename T>
	FORCEINLINE static void Destruct(T* ptr)
	{
		if constexpr(!TIsTriviallyDestructible<T>::Value)
		{
			ptr->~T();
		}
	}
};

#if ASTD_NEW_DELETE
void* operator new(TSize size)
{
	return SMemory::MallocTyped<uint8>((uint32)size);
}

void operator delete(void* ptr, TSize) noexcept
{
	return SMemory::Free(ptr);
}
#endif
