// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"
#include "Kor/Archive.h"

#include <cstdio>

KOR_NAMESPACE_BEGIN

struct SCFileArchive : public SArchive
{
	KOR_FORCEINLINE SCFileArchive(EArchiveType type, EArchiveMode mode, const tchar* filename, bool overwrite)
		: SArchive(type, mode)
		, _filename(filename)
		, _overwrite(overwrite)
	{
		OpenImpl();
	}

	KOR_FORCEINLINE virtual ~SCFileArchive() override
	{
		CloseImpl();
	}

	KOR_FORCEINLINE FILE* GetFile() const { return _file; }

	// SArchive overrides
	/////////////////////////////////

	KOR_FORCEINLINE virtual bool IsValid() const override { return !!_file; }
	KOR_FORCEINLINE virtual void Flush() override { fflush(_file); }
	virtual SizeType GetTotalBytes() const override
	{
		const SizeType currOff = ftell(_file);
		fseek(_file, 0, SEEK_END);
		const SizeType result = ftell(_file);
		fseek(_file, currOff, SEEK_SET);
		return result;
	}
	KOR_FORCEINLINE_DEBUGGABLE virtual SizeType GetBytesOffset() const override
	{
		return ftell(_file);
	}
	KOR_FORCEINLINE_DEBUGGABLE virtual bool SetBytesOffset(SizeType offset) override
	{
		return fseek(_file, offset, SEEK_SET) == 0;
	}
	KOR_FORCEINLINE_DEBUGGABLE virtual SizeType ReadBytes(void* ptr, SizeType size) override
	{
		return fread(ptr, sizeof(uint8), size, _file);
	}
	KOR_FORCEINLINE_DEBUGGABLE virtual SizeType WriteBytes(const void* ptr, SizeType size) override
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
					OpenCallImpl(KTEXT("r"));
				break;
				case EArchiveMode::Write:
					OpenCallImpl(KTEXT("w"));
				break;
				case EArchiveMode::ReadWrite:
				{
					OpenCallImpl(_overwrite ? KTEXT("w+") : KTEXT("r+"));
					if (!_file)
					{
						OpenCallImpl(KTEXT("w+"));
					}
				}
				break;
			}

		}
	}

	void OpenCallImpl(const tchar* modes)
	{
#if KOR_PLATFORM_WINDOWS && KOR_USE_UNICODE
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

KOR_NAMESPACE_END