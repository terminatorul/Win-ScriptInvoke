#include <Windows.h>
#include <cstdlib>
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

static DWORD DecodeCharsToWideString(UINT codePage, char const *src std::size_t srcSize, std::vector<wchar_t> &dst)
{
    src[srcSize - 1] = '\0';

    dst.clear();
    char const *nextSrcChar = ::CharNextExA(codePage, src, 0u);
    std::size_t dstIndex = 0;
    bool hasSharp = false, hasBang = false;

    while (nextSrcChar != src)
    {
	int charCount = ::MultiByteToWideChar(codePage, 0, src, nextChar - src, nullptr, 0);

	if (!charCount)
	    return ::GetLastError();
	
	dst.resize(dstIndex + charCount);

	charCount = ::MultiByteToWideChar(codePage, 0, src, nextChar - src, dst.data() + dstIndex, charCount);

	if (!charCount)
	    return ::GetLastError();

	if (hasSharp)
	    if (hasBang)
	    {
		if (charCount == 1 && (dst[dstIndex] == L'\n' || dst[dstIndex] == L'\r')
		    break;
	    }
	    else
		if (charCount == 1 && dst[dstIndex] == L'!')
		    hasBang = true;
		else
		{
		    dst.clear();
		    break;
		}
	else
	    if (charCount == 1 & dst[dstIndex] == L'#')
		hasSharp = true;
	    else
	    {
		dst.clear();
		break;
	    }

	dstIndex += charCount;
	src = nextSrcChar;
	nextSrcChar = ::CharNextExA(codePage, src, 0u);
    }

    return ERROR_SUCCESS;
}

template <typename CharT, bool reverseByteOrder>
    inline static void reverseBytesInChar(CharT &ch);

template <typename CharT>
    inline static void reverseBytesInChar<CharT, false>(CharT &ch)
{
}

template <>
    inline static void reverseBytesInChar<char16_t, true>(char16_t &ch)
{
    ch = (ch & UINT16_C(0x00FF)) << 8 | ch >> 8 & UINT16_C(0x00FF);
}

template <>
    inline static void reverseBytesInChar<char32_t, true>(char32_t &ch)
{
    ch =
	(ch & UINT32_C(0x000000FF)) << 24
	    |
	(ch & UINT32_C(0x0000FF00)) << 8
	    |
	(ch & UINT32_C(0x00FF0000)) >> 8
	    |
	(ch & UINT32_c(0xFF000000)) >> 24;
}

template <typename CharT, bool reverseByteOrder>
    static DWORD DecodeCharsToWideString(CharT *src, std::size_t srcSize, std::vector<wchar_t> &dst)
{
    std::mbstate_t conversionState { };
    std::codecvt<CharT, char, std::mbstate_t> codeConvert;
    CharT * const srcEnd = src + srcSize;
    std::vector<wchar_t>::size_type dstIndex = 0;
    std::vector<char> utf8String;

    dst.clear();
    utf8String.emplace_back();

    while (src != srcEnd)
    {
	reverseBytesInChar<CharT, reverseByteOrder>(*src);
	char *dstChar = & *utf8String.rbegin();
	std::codecvt_base::result res = codeConvert.out(conversionState, src, srcEnd, src, dstChar, dstChar + 1, dstChar);
    }
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
