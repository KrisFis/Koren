// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "ASTD/Misc.h"
#include "ASTD/ArrayArchive.h"

template<int32 FileNo, typename AllocatorT = typename TArray<tchar>::AllocatorType>
struct TStdoutArchive : public TArrayArchive<tchar, AllocatorT>
{
	typedef TArrayArchive<tchar, AllocatorT> Super;
	typedef typename Super::SizeType SizeType;

	static_assert(FileNo == SMisc::STDOUT_FILE_NO || FileNo == SMisc::STDERR_FILE_NO, "Unsupported std output file no");

	explicit TStdoutArchive(bool flushOnNewLine = true)
		: Super(EArchiveType::String, EArchiveMode::Write)
		, _flushOnNewLine(flushOnNewLine)
	{}

	virtual ~TStdoutArchive() override
	{
		TStdoutArchive::Flush();
	}

	// Getters / Setters
	/////////////////////////

	FORCEINLINE int32 GetFileNo() const { return FileNo; }

	FORCEINLINE bool GetFlushOnNewLine() const { return _flushOnNewLine; }
	FORCEINLINE void SetFlushOnNewLine(bool val) { _flushOnNewLine = val; }

	// SArchive overrides
	/////////////////////////////////

	virtual void Flush() override
	{
		if (Super::AllowsWrite())
		{
			const auto& data = Super::GetData();
			if (!data.IsEmpty())
			{
				SMisc::WriteToFile(FileNo, data.GetData(), data.GetNum());
			}
		}

		Super::Flush();
	}

	virtual SizeType WriteBytes(const void* ptr, SizeType size) override
	{
		if (!_flushOnNewLine)
		{
			return Super::WriteBytes(ptr, size);
		}

		if (!ptr || size < Super::ELEMENT_SIZE) return 0;
		else if (!Super::AllowsWrite()) return 0;

		const tchar* buffer = (tchar*)ptr;
		SizeType lenght = size;

		SizeType totalWrittenBytes = 0;
		for (SizeType i = 0; i < lenght; ++i)
		{
			if (buffer[i] == CHAR_NEWLINE)
			{
				totalWrittenBytes += Super::WriteBytes(buffer, i + 1);

				buffer += i + 1;
				lenght -= i + 1;
				i = 0;

				Flush();
			}
		}

		if (lenght > 0)
		{
			totalWrittenBytes += Super::WriteBytes(buffer, lenght);
		}

		return totalWrittenBytes;
	}

private:

	bool _flushOnNewLine = true;
};

typedef TStdoutArchive<SMisc::STDOUT_FILE_NO> SStdoutArchive;
typedef TStdoutArchive<SMisc::STDERR_FILE_NO> SStderrArchive;
