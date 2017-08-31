#include <windows.h>
#include <winioctl.h>
#include <assert.h>
#include "intrinsic.h"

#if (!defined(_MSC_VER) || _MSC_VER < 1200) && !defined(__assume)
#define __assume(expression)
#endif

#ifdef __BORLANDC__
#ifndef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
#endif
#define vector_string                                 vector<string>
#define vector_GUID                                   vector<GUID>
#define vector_begin(v)                               (v)->begin()
#define vector_end(v)                                 (v)->end()
#define vector_push_back(v, x)                        (v)->push_back(x)
#define vector_string_clear(v)                        (v)->clear()
#define vector_string_push_back_range(v, first, last) (v)->push_back(string(first, last))
#define string_length(s)                              (s)->length()
#define string_begin(s)                               (s)->begin()
#undef InlineIsEqualGUID
#define InlineIsEqualGUID(rguid1, rguid2) (                         \
    ((unsigned long *)rguid1)[0] == ((unsigned long *)rguid2)[0] && \
    ((unsigned long *)rguid1)[1] == ((unsigned long *)rguid2)[1] && \
    ((unsigned long *)rguid1)[2] == ((unsigned long *)rguid2)[2] && \
    ((unsigned long *)rguid1)[3] == ((unsigned long *)rguid2)[3])
#else
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#define typename GUID
#include "bcb6_std_vector_template.h"
#endif

#define MODE_END_LINE        0x0001
#define MODE_END_STR         0x0002
#define MODE_END_BYTE        0x0004
#define MODE_START_BYTE      0x0010
#define MODE_LINE_FEED_COUNT 0x0100
#define MODE_APPEND          0x0200
#define MODE_RECURSIVE       0x0400
#define MODE_EXTRACT_SCRIPT  (MODE_END_BYTE | MODE_START_BYTE)

extern HANDLE hHeap;

EXTERN_C char * __fastcall GetFileTitlePointerA(const char *lpFileNeme);
#ifndef __BORLANDC__
EXTERN_C void __fastcall CheckSSGVersion(const vector_string *lines);
#endif

#ifdef __BORLANDC__
#define TStringFiler_LoadFromFile TStringFiler::LoadFromFile
#else
unsigned long __cdecl TStringFiler_LoadFromFile(
	IN OUT vector_string  *SList,
	IN     const char     *FileName,
	IN     unsigned long  GetSize,
	IN     unsigned long  Mode,
	IN     unsigned long  StartPos,    // unused
	IN     unsigned long  EndPos,      // unused
	IN     const char     *StartWord,  // unused
	IN     const char     *EndWord);   // unused
#endif

static BOOL __stdcall GetIncludeFileName(
	OUT char       *lpFileName,
	IN  const char *begin,
	IN  const char *end,
	IN  const char *lpBaseFileName)
{
	size_t fileNameLength;
	size_t directoryLength;
	size_t length;

	assert(begin <= end);
	assert(*end == '\r' || *end == '\n' || *end == '\0');
	assert(lpBaseFileName != NULL);

	if (begin >= end)
		return FALSE;
	if (__intrinsic_isspace(*begin))
		do { if (++begin >= end) return FALSE; } while (__intrinsic_isspace(*begin));
	if (*begin != '#')
		return FALSE;
	do { if (++begin >= end) return FALSE; } while (__intrinsic_isspace(*begin));
	if (end - begin <= 8)
		return FALSE;
	if (*(LPDWORD)begin != BSWAP32('incl'))
		return FALSE;
	if ((*(LPDWORD)(begin + 4) & 0x00FFFFFF) != BSWAP32('ude\0'))
		return FALSE;
	begin += 7;
	if (__intrinsic_isspace(*begin))
		do { if (++begin >= end) return FALSE; } while (__intrinsic_isspace(*begin));
	else if (*begin != '"' && *begin != '<')
		return FALSE;
	if (*begin == '"' || *begin == '<')
		do { if (++begin >= end) return FALSE; } while (__intrinsic_isspace(*begin));
	if (begin == end)
		return FALSE;
	do { if (--end < begin) return FALSE; } while (__intrinsic_isspace(*end));
	if (*end == '"' || *end == '>')
		do { if (--end < begin) return FALSE; } while (__intrinsic_isspace(*end));
	if (!(fileNameLength = ++end - begin))
		return FALSE;
	if (*begin != '\\' && (!__intrinsic_isalpha(*begin) || *(begin + 1) != ':'))
		directoryLength = GetFileTitlePointerA(lpBaseFileName) - lpBaseFileName;
	else
		directoryLength = 0;
	if ((length = directoryLength + fileNameLength) >= MAX_PATH)
		return FALSE;
	memcpy(lpFileName, lpBaseFileName, directoryLength);
	memcpy(lpFileName + directoryLength, begin, fileNameLength);
	lpFileName[length] = '\0';
	return TRUE;
}

static __inline BOOL __stdcall GetFileObjectIdA(
	OUT GUID   *lpFileObjectId,
	IN  LPCSTR lpFileName)
{
	BOOL   bSuccess;
	HANDLE hFile;

	bSuccess = FALSE;
	hFile = CreateFileA(
		lpFileName,
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		FILE_OBJECTID_BUFFER buffer;
		DWORD                dwBytesReturned;

		if (DeviceIoControl(
			hFile,
			FSCTL_CREATE_OR_GET_OBJECT_ID,
			NULL,
			0,
			&buffer,
			sizeof(buffer),
			&dwBytesReturned,
			NULL))
		{
			*lpFileObjectId = *(GUID *)&buffer.ObjectId;
			bSuccess = TRUE;
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

static unsigned long RecursiveLoad(
	IN OUT vector_string *SList,
	IN     const char    *lpFileName,
	IN     unsigned long dwLoadedFiles,
	IN     unsigned long dwMode)
{
	vector_GUID   loaded;
	GUID          fileObjectId;
	unsigned long dwRecursiveMode;
	long          ret;

	if (!GetFileObjectIdA(&fileObjectId, lpFileName))
		return 0;
	if (!(dwMode & MODE_RECURSIVE))
	{
#ifndef __BORLANDC__
		vector_ctor(&loaded);
#endif
		dwLoadedFiles = (unsigned long)&loaded;
	}
	else
	{
		vector_GUID *v = (vector_GUID *)dwLoadedFiles;
		for (GUID *it = vector_begin(v); it != vector_end(v); it++)
			if (InlineIsEqualGUID(it, &fileObjectId))
				return 0;
	}
	vector_push_back((vector_GUID *)dwLoadedFiles, fileObjectId);
	dwRecursiveMode = (dwMode & ~MODE_EXTRACT_SCRIPT) | MODE_APPEND | MODE_RECURSIVE;
	ret = TStringFiler_LoadFromFile(SList, lpFileName, dwLoadedFiles, dwRecursiveMode, 0, 0, NULL, NULL);
#ifndef __BORLANDC__
	if (!(dwMode & MODE_RECURSIVE))
		vector_dtor(&loaded);
#endif
	return ret;
}

#ifdef __BORLANDC__
unsigned long TStringFiler::LoadFromFile(
#else
unsigned long __cdecl TStringFiler_LoadFromFile(
#endif
	IN OUT vector_string  *SList,
	IN     const char     *FileName,
	IN     unsigned long  GetSize,
	IN     unsigned long  Mode,
	IN     unsigned long  StartPos,    // unused
	IN     unsigned long  EndPos,      // unused
	IN     const char     *StartWord,  // unused
	IN     const char     *EndWord)    // unused
{
	#define READ_BLOCK_SIZE 0x00010000

	HANDLE    hFile;
	DWORD     dwFileSize, dwNumberOfBytesToRead, dwNumberOfBytesRead;
	char      *lpBuffer;
	size_t    nBufferLength, nBufferCapacity;
	ptrdiff_t difference;

	assert((Mode & MODE_END_LINE) == 0);
	assert((Mode & MODE_END_STR) == 0);

	__assume((Mode & MODE_END_LINE) == 0);
	__assume((Mode & MODE_END_STR) == 0);

	hFile = CreateFileA(
		FileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	// ì«Ç›çûÇ›ÉGÉâÅ[
	if (hFile == INVALID_HANDLE_VALUE)
		goto FAILED1;

	dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == MAXDWORD)
		goto FAILED2;

	// àÍíUãÛÇ…Ç»ÇËÇ‹Ç∑
	if (!(Mode & MODE_APPEND))
		vector_string_clear(SList);

	if (dwFileSize == 0)
		goto DONE;

	nBufferCapacity = min(dwFileSize, READ_BLOCK_SIZE * 2 - 1);
	lpBuffer = (char *)HeapAlloc(hHeap, 0, nBufferCapacity + 1);
	if (!lpBuffer)
		goto FAILED2;
	nBufferLength = 0;

	difference = 0;
	dwNumberOfBytesToRead = min(READ_BLOCK_SIZE, nBufferCapacity);
	while (ReadFile(hFile, lpBuffer + nBufferLength, dwNumberOfBytesToRead, &dwNumberOfBytesRead, NULL) && dwNumberOfBytesRead)
	{
		char   *p, *end, *line;
		size_t length, index;

		//--------------
		// â¸çsÇ≈êÿÇËï™ÇØ
		p = lpBuffer + nBufferLength + difference;
		nBufferLength += dwNumberOfBytesRead;
		end = lpBuffer + nBufferLength;
		*end = '\0';

		line = lpBuffer;
		while (p < end)
		{
			char c, *next, includeFileName[MAX_PATH];

			c = *p;
			next = p + 1;
			switch ((unsigned char)c)
			{
			case '\r':
				if (next == end)
					break;
				if (*next == '\n')
					next++;
			case '\n':
				if (!GetIncludeFileName(includeFileName, line, p, FileName))
					vector_string_push_back_range(SList, line, p);
				else if (RecursiveLoad(SList, includeFileName, GetSize, Mode))
					goto FAILED3;
				line = p = next;
				continue;
			case '\\':
				if (next == end)
					break;
				c = *(next++);
				switch ((unsigned char)c)
				{
				case '\r':
					if (next == end)
						break;
					if (*next == '\n')
						next++;
				case '\n':
					memcpy(p, next, end - next + 1);
					length = next - p;
					nBufferLength -= length;
					end -= length;
					continue;
#if CODEPAGE_SUPPORT
				default:
					if (__intrinsic_isleadbyte(c))
						next++;
#else
				case_unsigned_leadbyte:
					next++;
				default:
#endif
					p = next;
					continue;
				}
				break;
#if CODEPAGE_SUPPORT
			default:
				if (__intrinsic_isleadbyte(c))
					next++;
#else
			case_unsigned_leadbyte:
				next++;
			default:
#endif
				p = next;
				continue;
			}
			break;
		}
		difference = p - end;

		index = line - lpBuffer;
		if (nBufferLength -= index)
		{
			size_t require;

			require = nBufferLength + READ_BLOCK_SIZE;
			if (require > nBufferCapacity)
			{
				void *lpMem;

				if ((ptrdiff_t)++nBufferCapacity >= 0)
					nBufferCapacity <<= 1;
				else
					nBufferCapacity = require + 1;
				lpMem = HeapReAlloc(hHeap, 0, lpBuffer, nBufferCapacity--);
				if (!lpMem)
					goto FAILED3;
				lpBuffer = (char *)lpMem;
				line = (char *)lpMem + index;
			}
			memcpy(lpBuffer, line, nBufferLength);
		}
		//------
	}

	// ç≈èIçsÇäiî[
	if (nBufferLength)
	{
		char includeFileName[MAX_PATH];

		lpBuffer[nBufferLength] = '\0';
		if (!GetIncludeFileName(includeFileName, lpBuffer, lpBuffer + nBufferLength, FileName))
			vector_string_push_back_range(SList, lpBuffer, lpBuffer + nBufferLength);
		else if (RecursiveLoad(SList, includeFileName, GetSize, Mode))
			goto FAILED3;
	}

	HeapFree(hHeap, 0, lpBuffer);

	if (Mode & MODE_EXTRACT_SCRIPT)
	{
#ifdef __BORLANDC__
		string *first, *last, *it;

		last = SList->begin();
		while (last != SList->end())
		{
			char *p = (it = last++)->begin();
			while (*p == ' ' || *p == '\t')
				p++;
			if (it->end() - p >= 7)
				if (*(LPDWORD)p == BSWAP32('[scr'))
					if ((*(LPDWORD)(p + 4) & 0x00FFFFFF) == BSWAP32('ipt\0'))
						break;
		}
		SList->erase(SList->begin(), last);
		for (first = SList->begin(); first != SList->end(); first++)
		{
			char *p = first->begin();
			while (*p == ' ' || *p == '\t')
				p++;
			if (first->end() - p >= 8)
				if (*(LPDWORD)p == BSWAP32('[/sc'))
					if (*(LPDWORD)(p + 4) == BSWAP32('ript'))
						break;
		}
		SList->erase(first, SList->end());
#else
		string *first, *last, *it;
		size_t size;

		if (!(Mode & MODE_APPEND))
			CheckSSGVersion(SList);
		first = vector_begin(SList);
		while (first != vector_end(SList))
		{
			char *p = string_begin(first);
			while (*p == ' ' || *p == '\t')
				p++;
			BOOLEAN equals = FALSE;
			if (string_end(first) - p >= 7)
				if (*(LPDWORD)p == BSWAP32('[scr'))
					if ((*(LPDWORD)(p + 4) & 0x00FFFFFF) == BSWAP32('ipt\0'))
						equals = TRUE;
			string_dtor(first++);
			if (equals)
				break;
		}
		for (last = first; last != vector_end(SList); last++)
		{
			char *p = string_begin(last);
			while (*p == ' ' || *p == '\t')
				p++;
			if (string_end(last) - p >= 8)
				if (*(LPDWORD)p == BSWAP32('[/sc'))
					if (*(LPDWORD)(p + 4) == BSWAP32('ript'))
						break;
		}
		for (it = last; it != vector_end(SList); it++)
			string_dtor(it);
		size = (size_t)last - (size_t)first;
		vector_end(SList) = (string *)((char *)vector_begin(SList) + size);
		memcpy(vector_begin(SList), first, size);
		vector_shrink_to_fit(SList);
#endif
	}

DONE:
	CloseHandle(hFile);
	return 0;

FAILED3:
	HeapFree(hHeap, 0, lpBuffer);
FAILED2:
	CloseHandle(hFile);
FAILED1:
	return 1;

	#undef READ_BLOCK_SIZE
}

#undef MODE_END_LINE
#undef MODE_END_STR
#undef MODE_END_BYTE
#undef MODE_START_BYTE
#undef MODE_LINE_FEED_COUNT
#undef MODE_APPEND
#undef MODE_RECURSIVE
#undef MODE_EXTRACT_SCRIPT
