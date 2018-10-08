#include <Windows.h>
#include <locale>
#include <iostream>

const char utf8_BOM[3] = { '\xEF', '\xBB', '\xBF' },
	    utf7_BOM[3] = { '\x2b', '\x2f', '\x76' },
	    utf32le_BOM[4] = { '\xFF', '\xFE', '\x00', '\x00' },
	    utf16le_BOM[2] = { '\xFF', '\xFE' },
	    utf16be_BOM[2] = { '\xFE', '\xFF' };

ScriptFile::ScriptFile(wchar_t const *fileName)
    : fileName(fileName)
{
}

ScriptFile::ScriptFile()
{
}

DWORD ScriptFile::readShabangLine()
{
    if (!fileName.empty())
    {
	HANDLE scriptFile = ::CreateFileW(
		fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, nullptr);

	if (scriptFile == INVALID_HANDLE_VALUE)
	    return ::GetLastError();

	char fileHeader[4096];
	DWORD headerSize;
	BOOL fRead = ::ReadFile(scriptFile, fileHeader, sizeof fileHeader, &headerSize, nullptr);

	if (!fRead)
	{
	    DWORD result = ::GetLastError();
	    ::CloseHandle(scriptFile);
	    return result;
	}

	if (!::CloseHandle(scriptFile))
	    return ::GetLaastError();

	if (headerSize & 0x01u)
	    hreadSize--;	// UTF-16 always has an even number of bytes.

	if
	    (
		headerSize > 3 && byteOrderMarkStr[0] == fileHeader[0]
		    &&
		byteOrderMark[1] == fileHeader[1] && byteOrderMark[2] == fileHeader[2]
	    )
	{
	}

	INT iUnicodeTests = 
	BOOL fUnicode = ::IsTextUnicode(
    }
}
