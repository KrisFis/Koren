// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/Memory/Minimal.h"

#include "Kor/Memory/MemoryOps.h"
#include "Kor/Math/MathOps.h"

KOR_NAMESPACE_BEGIN

// Raw, fixed-size, aligned byte storage with no type information and no
// lifetime semantics. Purely a memory layout primitive — use TTypedBytes<T>
// when you need typed construct/destruct/access on top of raw storage.
template<int32 Size, uint32 Alignment = alignof(uint8)>
struct TBytes
{
	static_assert(Size > 0, "TAlignedBytes requires a positive Size.");
	static_assert(Alignment > 0 && SMathOps::IsPowerOfTwo(Alignment), "Alignment must be a power of two.");

	alignas(Alignment) uint8 Pad[Size];
};

// Untyped, aligned raw storage for a single T, with manual lifetime control.
// Does not construct or destroy T automatically; call Construct()/Destruct()
// explicitly to manage the object living in this storage.
// Non-copyable and non-movable as a container — only the held value can be
// extracted (moved) via the && overloads of operator*()/GetRef().
template<typename T, uint32 Alignment = alignof(T)>
struct TTypedBytes
{
	static_assert(Alignment > 0 && SMathOps::IsPowerOfTwo(Alignment), "Alignment must be a power of two.");

	TTypedBytes() = default;
	~TTypedBytes() = default;

	TTypedBytes(TTypedBytes&&) = delete;
	TTypedBytes(const TTypedBytes&) = delete;
	TTypedBytes& operator=(TTypedBytes&&) = delete;
	TTypedBytes& operator=(const TTypedBytes&) = delete;

	KOR_FORCEINLINE T& operator*() & noexcept { return *Get(); }
	KOR_FORCEINLINE T&& operator*() && noexcept { return Move(*Get()); }
	KOR_FORCEINLINE const T& operator*() const & noexcept { return *Get(); }

	KOR_FORCEINLINE T* operator->() noexcept { return Get(); }
	KOR_FORCEINLINE const T* operator->() const noexcept { return Get(); }

	KOR_FORCEINLINE T& GetRef() & noexcept { return *Get(); }
	KOR_FORCEINLINE T&& GetRef() && noexcept { return Move(*Get()); }
	KOR_FORCEINLINE const T& GetRef() const & noexcept { return *Get(); }

	KOR_FORCEINLINE T* Get() noexcept { return (T*)(this); }
	KOR_FORCEINLINE const T* Get() const noexcept { return (const T*)(this); }

	template<typename... ArgsT>
	KOR_FORCEINLINE void Construct(ArgsT&&... args) noexcept
	{
		SMemoryOps::Construct(Get(), Forward<ArgsT>(args)...);
	}

	KOR_FORCEINLINE void Destruct() noexcept
	{
		SMemoryOps::Destruct(Get());
	}

	alignas(Alignment) uint8 Pad[sizeof(T)];
};

KOR_NAMESPACE_END