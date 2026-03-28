// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "ASTD/Archive.h"
#include "ASTD/Array.h"

// TODO(krisfis): we might want to in-place memory, meaning that it will write to memory block provided
// Also setting read/write mode is annoying, maybe make it optional
template<typename ElementT, typename AllocatorT = typename TArray<tchar>::AllocatorType>
struct TArrayArchive : public SArchive
{
	typedef TArray<ElementT, AllocatorT> ArrayType;
	static constexpr SizeType ELEMENT_SIZE = sizeof(ElementT);

	FORCEINLINE TArrayArchive(EArchiveType type, EArchiveMode mode)
		: SArchive(type, mode)
	{
		SetData({});
	}

	FORCEINLINE TArrayArchive(EArchiveType type, EArchiveMode mode, ArrayType&& data)
		: SArchive(type, mode)
	{
		SetData(Forward(data));
	}

	// Data set/get
	/////////////////////////////////

	FORCEINLINE void SetData(ArrayType&& data) { _data = data; _offset = _data.GetNum(); }
	FORCEINLINE const ArrayType& GetData() const { return _data; }

	// SArchive overrides
	/////////////////////////////////

	FORCEINLINE virtual bool IsValid() const override { return true; }
	FORCEINLINE virtual void Flush() override { SetData(ArrayType()); }
	FORCEINLINE virtual SizeType GetTotalBytes() const override { return _data.GetNum() * ELEMENT_SIZE; }
	FORCEINLINE virtual SizeType GetBytesOffset() const override { return _offset * ELEMENT_SIZE; }
	virtual bool SetBytesOffset(SizeType offset) override
	{
		const SizeType offsetInElements = offset >= ELEMENT_SIZE ? SMath::Floor(offset / ELEMENT_SIZE) : 0;
		if (_data.IsValidIndex(offsetInElements))
		{
			_offset = offsetInElements;
			return true;
		}

		return false;
	}

	virtual SizeType ReadBytes(void* ptr, SizeType size) override
	{
		if (!ptr || size < ELEMENT_SIZE) return 0;
		else if (!AllowsRead()) return 0;

		const SizeType elementsToRead = SMath::Min<SizeType>(
			SMath::Floor(size / ELEMENT_SIZE),
			_data.GetNum() - _offset
		);

		if (elementsToRead > 0)
		{
			SMemory::Copy(ptr, _data.GetData() + (_offset * ELEMENT_SIZE), elementsToRead * ELEMENT_SIZE);
			_offset += elementsToRead;
		}

		return elementsToRead * ELEMENT_SIZE;
	}

	virtual SizeType WriteBytes(const void* ptr, SizeType size) override
	{
		if (!ptr || size < ELEMENT_SIZE) return 0;
		else if (!AllowsWrite()) return 0;

		const SizeType elementsToWrite = SMath::Floor(size / ELEMENT_SIZE);
		if (elementsToWrite + _offset > _data.GetNum())
		{
			_data.Resize(elementsToWrite + _offset);
		}

		SMemory::Copy(_data.GetData() + (_offset * ELEMENT_SIZE), ptr, size);
		_offset += elementsToWrite;

		return elementsToWrite * ELEMENT_SIZE;
	}

private:
	ArrayType _data;
	SizeType _offset = 0;
};
