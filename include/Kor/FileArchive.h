// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "Archive.h"
#include <cstdio>

struct SCFileArchive : public SArchive
{
	FORCEINLINE SCFileArchive(EArchiveType type, EArchiveMode mode, const tchar* filename, bool overwrite)
		: SArchive(type, mode)
		, _filename(filename)
		, _overwrite(overwrite)
	{
		OpenImpl();
	}

	FORCEINLINE virtual ~SCFileArchive() override
	{
		CloseImpl();
	}

	FORCEINLINE FILE* GetFile() const { return _file; }

	// SArchive overrides
	/////////////////////////////////

	FORCEINLINE virtual bool IsValid() const override { return !!_file; }
	FORCEINLINE virtual void Flush() override { fflush(_file); }
	virtual SizeType GetTotalBytes() const override
	{
		const SizeType currOff = ftell(_file);
		fseek(_file, 0, SEEK_END);
		const SizeType result = ftell(_file);
		fseek(_file, currOff, SEEK_SET);
		return result;
	}
	FORCEINLINE_DEBUGGABLE virtual SizeType GetBytesOffset() const override
	{
		return ftell(_file);
	}
	FORCEINLINE_DEBUGGABLE virtual bool SetBytesOffset(SizeType offset) override
	{
		return fseek(_file, offset, SEEK_SET) == 0;
	}
	FORCEINLINE_DEBUGGABLE virtual SizeType ReadBytes(void* ptr, SizeType size) override
	{
		return fread(ptr, sizeof(uint8), size, _file);
	}
	FORCEINLINE_DEBUGGABLE virtual SizeType WriteBytes(const void* ptr, SizeType size) override
	{
		return fwrite(ptr, sizeof(uint8), size, _file);
	}

private:
	void OpenImpl()
	{
		if (!_file)
		{
			switch (GetMode())
			{
				case EArchiveMode::Read:
					OpenCallImpl(TEXT("r"));
				break;
				case EArchiveMode::Write:
					OpenCallImpl(TEXT("w"));
				break;
				case EArchiveMode::ReadWrite:
				{
					OpenCallImpl(_overwrite ? TEXT("w+") : TEXT("r+"));
					if (!_file)
					{
						OpenCallImpl(TEXT("w+"));
					}
				}
				break;
			}

		}
	}

	void OpenCallImpl(const tchar* modes)
	{
#if PLATFORM_WINDOWS && ASTD_USE_UNICODE
		_file = _wfopen(_filename, modes);
#else
		_file = fopen(_filename, modes);
#endif
	}

	void CloseImpl()
	{
		if (_file)
		{
			fclose(_file);
			_file = nullptr;
		}
	}

	FILE* _file = nullptr;
	const tchar* _filename = nullptr;
	bool _overwrite = false;
};
