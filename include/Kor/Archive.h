// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "ASTD/CString.h"

enum class EArchiveType : uint8
{
	Binary = 0,
	String
};

enum class EArchiveMode : uint8
{
	Read = 0,
	Write,
	ReadWrite
};

struct SArchive
{
	// Types
	/////////////////////////

	typedef int64 SizeType;

	// Constructors
	/////////////////////////

	FORCEINLINE SArchive() = delete;
	FORCEINLINE SArchive(EArchiveType type, EArchiveMode mode)
		: _type(type)
		, _mode(mode)
	{}

	FORCEINLINE SArchive(const SArchive& other) = delete;
	FORCEINLINE SArchive(SArchive&& other) = delete;

	FORCEINLINE virtual ~SArchive() = default;

	// Type & Mode
	/////////////////////////

	FORCEINLINE EArchiveType GetType() const { return _type; }
	FORCEINLINE bool IsBinary() const { return _type == EArchiveType::Binary; }
	FORCEINLINE bool IsString() const { return _type == EArchiveType::String; }

	FORCEINLINE EArchiveMode GetMode() const { return _mode; }
	FORCEINLINE bool AllowsRead() const { return _mode == EArchiveMode::Read || _mode == EArchiveMode::ReadWrite; }
	FORCEINLINE bool AllowsWrite() const { return _mode == EArchiveMode::Write || _mode == EArchiveMode::ReadWrite; }

	// Essentials
	/////////////////////////

	virtual bool IsValid() const = 0;
	virtual void Flush() = 0;

	// Position
	/////////////////////////

	FORCEINLINE bool GetIsBegin() const { return GetBytesOffset() <= 0; }
	FORCEINLINE bool GetIsEnd() const { return GetBytesOffset() >= GetTotalBytes(); }

	// Gets number of bytes
	FORCEINLINE bool IsEmpty() const { return GetTotalBytes() <= 0; }

	template<typename T = uint8>
	FORCEINLINE_DEBUGGABLE SizeType GetTotal() const
	{
		if constexpr (sizeof(T) == sizeof(uint8))
		{
			return GetTotalBytes();
		}
		else
		{
			const SizeType bytes = GetTotalBytes();
			return bytes >= sizeof(T) ? bytes / sizeof(T) : 0;
		}
	}

	// Gets remaining offset in T size (aka. how many Ts till the end of the archive)
	template<typename T = uint8>
	FORCEINLINE_DEBUGGABLE SizeType GetRemainingOffset() const
	{
		if constexpr (sizeof(T) == sizeof(uint8))
		{
			return GetTotalBytes() - GetBytesOffset();
		}
		else
		{
			const SizeType offsetBytes = GetBytesOffset();
			const SizeType offsetElements = offsetBytes >= sizeof(T) ? offsetBytes / sizeof(T) : 0;

			return GetOffset<T>() - offsetElements;
		}
	}

	template<typename T = uint8>
	FORCEINLINE_DEBUGGABLE SizeType GetOffset() const
	{
		if constexpr (sizeof(T) == sizeof(uint8))
		{
			return GetBytesOffset();
		}
		else
		{
			const SizeType bytes = GetBytesOffset();
			return bytes >= sizeof(T) ? bytes / sizeof(T) : 0;
		}
	}

	// Sets offset for T size
	template<typename T = uint8>
	FORCEINLINE_DEBUGGABLE void SetOffset(SizeType num)
	{
		SetBytesOffset(num * sizeof(T));
	}

	// Gets end position
	virtual SizeType GetTotalBytes() const = 0;
	FORCEINLINE SizeType GCount() const { return GetTotalBytes(); }

	// Gets current position (tell)
	virtual SizeType GetBytesOffset() const = 0;
	FORCEINLINE SizeType Tell() const { return GetBytesOffset(); }

	// Moves to position (seek)
	virtual bool SetBytesOffset(SizeType offset) = 0;
	FORCEINLINE bool Seek(SizeType offset) { return SetBytesOffset(offset); }

	// Read / Write
	/////////////////////////

	// Reads bytes and moves while doing so
	virtual SizeType ReadBytes(void* ptr, SizeType size) = 0;

	template<typename T>
	FORCEINLINE_DEBUGGABLE SizeType Read(T* ptr, SizeType num)
	{
		const SizeType bytesRead = ReadBytes(ptr, sizeof(T) * num);
		return bytesRead >= sizeof(T) ? bytesRead / sizeof(T) : 0;
	}

	// Writes bytes and moves while doing so
	virtual SizeType WriteBytes(const void* ptr, SizeType size) = 0;

	template<typename T>
	FORCEINLINE_DEBUGGABLE SizeType Write(const T* ptr, SizeType num)
	{
		const SizeType bytesWritten = WriteBytes(ptr, sizeof(T) * num);
		return bytesWritten >= sizeof(T) ? bytesWritten / sizeof(T) : 0;
	}

	// Packet
	// * Packet is a bunch of bytes basically
	/////////////////////////

	// Func: (const void* packet, SizeType numOfBytes) -> bool
	template<SizeType MaxPacketSize, typename FuncType>
	bool ReadPacketsUntil(FuncType&& func)
	{
		uint8 packet[MaxPacketSize];
		while(true)
		{
			const SizeType readBytes = ReadBytes(&packet, MaxPacketSize);
			if (readBytes > 0)
			{
				if (func((const void*)&packet, readBytes)) break;
			}

			if (readBytes < MaxPacketSize)
			{
				break;
			}
		}

		return true;
	}

	// Func: (const void* packet, SizeType numOfBytes) -> bool
	template<SizeType MaxPacketSize, typename FuncType>
	FORCEINLINE_DEBUGGABLE bool ReadPacketsUntil(SizeType startOffset, FuncType&& func)
	{
		if (!SetBytesOffset(startOffset)) return false;
		return ReadPacketsUntil<MaxPacketSize>(Forward(func));
	}

	// Container
	// * Takes all data and copies to provided container
	/////////////////////////

	template<typename ContainerT, typename ContainerTT = TContainerTypeTraits<ContainerT>>
	typename TEnableIf<ContainerTT::IsContainer, bool>::Type CopyToContainer(ContainerT& outContainer)
	{
		if (!AllowsRead()) return false;

		const SizeType oldOffset = GetBytesOffset();

		SetBytesOffset(0);
		*this >> outContainer;
		SetBytesOffset(oldOffset);

		return true;
	}

	// Reads string with defined memory pool
	/////////////////////////

	template<typename ArchiveT, typename PredT>
	const tchar* ReadPooledStringByPred(ArchiveT& ar, PredT&& predicate)
	{
		if (!AllowsRead()) return nullptr;
		else if (!IsString()) return nullptr;

		// pooled pointer
		thread_local tchar buffer[SCString::LARGE_BUFFER_SIZE];

		const SizeType oldOffset = ar.template GetOffset<tchar>();
		const SizeType expectedReadNum = SMath::Min<SizeType>(SCString::LARGE_BUFFER_SIZE, GetTotal<tchar>());

		if (expectedReadNum <= 0) return nullptr;

		// Copy to buffer
		const SizeType readNum = ar.Read(&buffer, expectedReadNum);

		uint16 usedNum = 0;
		while(usedNum < readNum)
		{
			const tchar& character = buffer[usedNum];
			if (!predicate(character))
			{
				break;
			}

			++usedNum;
		}

		ar.template SetOffset<tchar>(oldOffset + usedNum);

		buffer[usedNum] = CHAR_TERM;
		return buffer;
	}

private:
	EArchiveType _type = EArchiveType::Binary;
	EArchiveMode _mode = EArchiveMode::ReadWrite;
};

// Archive operator<< && operator>>
////////////////////////////////////////////

static SArchive& operator<<(SArchive& ar, SArchive& otherAr)
{
	if (ar.GetMode() == otherAr.GetMode() && otherAr.AllowsRead())
	{
		const uint32 remainingBytes = (otherAr.GetTotalBytes() - otherAr.GetBytesOffset());
		if (remainingBytes > 0)
		{
			uint8* buffer = SMemory::MallocTyped<uint8>(remainingBytes);
			{
				otherAr.ReadBytes(buffer, remainingBytes);
				ar.WriteBytes(buffer, remainingBytes);
			}
			SMemory::Free(buffer);
		}
	}

	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, SArchive& otherAr)
{
	otherAr << ar; // just switch streaming
	return ar;
}

template<typename ContainerT, typename ContainerTT = TContainerTypeTraits<ContainerT>>
inline static typename TEnableIf<ContainerTT::IsContainer, SArchive&>::Type operator<<(SArchive& ar, const ContainerT& container)
{
	if constexpr (ContainerTT::InlineMemory)
	{
		ar.Write(container.Begin(), container.GetNum());
	}
	else
	{
		for (auto it = container.Begin(); it != container.End(); ++it)
		{
			ar << *it;
		}
	}

	return ar;
}

template<typename ContainerT, typename ContainerTT = TContainerTypeTraits<ContainerT>>
inline static typename TEnableIf<ContainerTT::IsContainer, SArchive&>::Type operator>>(SArchive& ar, ContainerT& container)
{
	container.Resize(ar.GetRemainingOffset<typename ContainerTT::ElementType>());

	if constexpr (ContainerTT::InlineMemory)
	{
		ar.Read(container.begin(), container.GetNum());
	}
	else
	{
		for (auto it = container.begin(); it != container.end(); ++it)
		{
			ar >> *it;
		}
	}

	return ar;
}

static SArchive& operator<<(SArchive& ar, const int32 val)
{
	if (ar.IsBinary())
	{
		ar.Write(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT32];
		if (SCString::FromInt32(val, buffer, SCString::MAX_BUFFER_SIZE_INT32))
		{
			ar.Write(buffer, SCString::GetLength(buffer));
		}
	}

	return ar;
}

static SArchive& operator>>(SArchive& ar, int32& val)
{
	if (ar.IsBinary())
	{
		ar.Read(&val, 1);
	}
	else if (ar.IsString())
	{
		const tchar* buffer = ar.ReadPooledStringByPred(
			ar,
			[](const tchar& character) -> bool
			{
				return (character >= TEXT('0') && character <= TEXT('9')) || character == TEXT('-');
			}
		);

		val = buffer ? SCString::ToInt32(buffer) : 0;
	}

	return ar;
}

static SArchive& operator<<(SArchive& ar, const int64 val)
{
	if (ar.IsBinary())
	{
		ar.Write(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT64];
		if (SCString::FromInt64(val, buffer, SCString::MAX_BUFFER_SIZE_INT64))
		{
			ar.Write(buffer, SCString::GetLength(buffer));
		}
	}

	return ar;
}

static SArchive& operator>>(SArchive& ar, int64& val)
{
	if (ar.IsBinary())
	{
		ar.Read(&val, 1);
	}
	else if (ar.IsString())
	{
		const tchar* buffer = ar.ReadPooledStringByPred(
			ar,
			[](const tchar& character) -> bool
			{
				return (character >= TEXT('0') && character <= TEXT('9')) || character == TEXT('-');
			}
		);

		val = buffer ? SCString::ToInt64(buffer) : 0;
	}

	return ar;
}

static SArchive& operator<<(SArchive& ar, const double val)
{
	if (ar.IsBinary())
	{
		ar.Write(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_DOUBLE];
		if (SCString::FromDouble(val, 4, buffer, SCString::MAX_BUFFER_SIZE_DOUBLE))
		{
			ar.Write(buffer, SCString::GetLength(buffer));
		}
	}

	return ar;
}

static SArchive& operator>>(SArchive& ar, double& val)
{
	if (ar.IsBinary())
	{
		ar.Read(&val, 1);
	}
	else if (ar.IsString())
	{
		const tchar* buffer = ar.ReadPooledStringByPred(
			ar,
			[](const tchar& character) -> bool
			{
				return (character >= TEXT('0') && character <= TEXT('9')) || character == TEXT('.') || character == TEXT('-');
			}
		);

		val = buffer ? SCString::ToDouble(buffer) : 0.0;
	}

	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, tchar val)
{
	ar.Read(&val, 1);
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, tchar& val)
{
	ar.Write(&val, 1);
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const tchar* val)
{
	ar.Write(val, SCString::GetLength(val));
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, tchar* val)
{
	ar.Read(val, SCString::GetLength(val));
	return ar;
}
