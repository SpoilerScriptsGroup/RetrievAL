#include <windows.h>
#include <mbstring.h>
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
#define vector_begin(v)                               (v)->begin()
#define vector_end(v)                                 (v)->end()
#define vector_string_clear(v)                        (v)->clear()
#define vector_string_push_back_range(v, first, last) (v)->push_back(string(first, last))
#define string_length(s)                              (s)->length()
#define string_begin(s)                               (s)->begin()
#else
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#endif

#define MODE_END_LINE        0x0001
#define MODE_END_STR         0x0002
#define MODE_END_BYTE        0x0004
#define MODE_START_BYTE      0x0010
#define MODE_LINE_FEED_COUNT 0x0100
#define MODE_APPEND          0x0200
#define MODE_RECURSIVE       0x0400
#define IS_INCLUDE_LINE_FEED(mode) ((mode) & (MODE_END_BYTE | MODE_START_BYTE | MODE_LINE_FEED_COUNT))

extern HANDLE hHeap;

EXTERN_C char * __fastcall GetFileTitlePointerA(const char *lpFileNeme);
#ifndef __BORLANDC__
EXTERN_C void __fastcall CheckSSGVersion(const string *FirstLine);
#endif

#ifdef __BORLANDC__
#define TStringFiler_LoadFromFile TStringFiler::LoadFromFile
#else
unsigned long __cdecl TStringFiler_LoadFromFile(
	vector_string  *SList,
	const char     *FileName,
	unsigned long  GetSize,
	unsigned long  Mode,
	unsigned long  StartPos,
	unsigned long  EndPos,
	const char     *StartWord,
	const char     *EndWord);
#endif

static BOOL __stdcall GetIncludeFileName(char *includeFileName, const char *begin, const char *end, const char *baseFileName)
{
	assert(begin <= end);
	assert(*end == '\r' || *end == '\n' || *end == '\0');
	assert(baseFileName != NULL);

	do	/* do { ... } while (0); */
	{
		size_t fileNameLength;
		size_t directoryLength;
		char   fileName[MAX_PATH];
		char   fullPath[MAX_PATH];
		char   *filePart;

		while (__intrinsic_isspace(*begin))
			begin++;
		if (*begin != '#')
			break;
		if (end - begin <= 9)
			break;
		do { ++begin; } while (__intrinsic_isspace(*begin));
		if (end - begin <= 8)
			break;
		if (*(LPDWORD)begin != BSWAP32('incl'))
			break;
		if (*(LPWORD)(begin + 4) != BSWAP16('ud'))
			break;
		if (*(begin + 6) != 'e')
			break;
		begin += 7;
		if (__intrinsic_isspace(*begin))
			do { ++begin; } while (__intrinsic_isspace(*begin));
		else if (*begin != '"' && *begin != '<')
			break;
		if (*begin == '"' || *begin == '<')
			do { ++begin; } while (__intrinsic_isspace(*begin));
		if (begin == end)
			break;
		while (--end > begin && __intrinsic_isspace(*end));
		if (end == begin)
			break;
		if (*end == '"' || *end == '>')
			while (--end > begin && __intrinsic_isspace(*end));
		if (end == begin)
			break;
		end++;
		fileNameLength = end - begin;
		if (*begin != '\\' && (!__intrinsic_isalpha(*begin) || *(begin + 1) != ':'))
			directoryLength = GetFileTitlePointerA(baseFileName) - baseFileName;
		else
			directoryLength = 0;
		if (directoryLength + fileNameLength >= MAX_PATH)
			break;
		memcpy(fileName, baseFileName, directoryLength);
		memcpy(fileName + directoryLength, begin, fileNameLength);
		fileName[directoryLength + fileNameLength] = '\0';
		if (!GetFullPathNameA(fileName, MAX_PATH, fullPath, &filePart))
			break;
		return GetLongPathNameA(fullPath, includeFileName, MAX_PATH);
	} while (0);
	return FALSE;
}

static unsigned long RecursiveLoad(
	vector_string *SList,
	const char    *FileName,
	unsigned long Mode,
	unsigned long GetSize)
{
	vector_string loaded;
	size_t        length;
	unsigned long recursiveMode;
	long          result;

	length = strlen(FileName);
	if (!(Mode & MODE_RECURSIVE))
	{
#ifndef __BORLANDC__
		vector_ctor(&loaded);
#endif
		GetSize = (unsigned long)&loaded;
	}
	else
	{
		vector_string *v = (vector_string *)GetSize;
		for (string *it = vector_begin(v); it != vector_end(v); it++)
			if (string_length(it) == length)
				if (_mbsicmp(string_begin(it), FileName) == 0)
					return 0;
	}
	vector_string_push_back_range((vector_string *)GetSize, FileName, FileName + length);
	recursiveMode = Mode;
	if (Mode & (MODE_END_BYTE | MODE_START_BYTE))
		recursiveMode = (recursiveMode & ~(MODE_END_BYTE | MODE_START_BYTE)) | MODE_LINE_FEED_COUNT;
	recursiveMode |= MODE_APPEND | MODE_RECURSIVE;
	result = TStringFiler_LoadFromFile(SList, FileName, GetSize, recursiveMode, 0, ULONG_MAX, NULL, NULL);
#ifndef __BORLANDC__
	if (!(Mode & MODE_RECURSIVE))
		vector_string_dtor(&loaded);
#endif
	return result;
}

#ifdef __BORLANDC__
unsigned long TStringFiler::LoadFromFile(
#else
unsigned long __cdecl TStringFiler_LoadFromFile(
#endif
	vector_string  *SList,
	const char     *FileName,
	unsigned long  GetSize,
	unsigned long  Mode,
	unsigned long  StartPos,
	unsigned long  EndPos,
	const char     *StartWord,
	const char     *EndWord)
{
	#define READ_BLOCK_SIZE 0x00010000

	vector_string buffer;
	vector_string *lines;
	HANDLE        hFile;
	DWORD         dwFileSize;
	LPSTR         tmpS;
	size_t        tmpSLength;
	size_t        tmpSBufferSize;
	ptrdiff_t     nDifference;
	DWORD         dwNumberOfBytesRead;

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

#ifndef __BORLANDC__
	vector_ctor(&buffer);
#endif
	if (Mode & (MODE_END_BYTE | MODE_START_BYTE))
	{
		if (!(Mode & MODE_START_BYTE))
			StartPos = 0;
		if (!(Mode & MODE_END_BYTE))
			EndPos = ULONG_MAX;
		if (StartPos >= EndPos)
			goto DONE;
		lines = &buffer;
	}
	else
	{
		lines = SList;
	}

	tmpS = (char *)HeapAlloc(hHeap, 0, READ_BLOCK_SIZE * 2);
	if (tmpS == NULL)
		goto FAILED2;
	tmpSLength = 0;
	tmpSBufferSize = READ_BLOCK_SIZE * 2;

	nDifference = 0;
	while (ReadFile(hFile, tmpS + tmpSLength, READ_BLOCK_SIZE, &dwNumberOfBytesRead, NULL) && dwNumberOfBytesRead)
	{
		LPSTR  p, end, line;
		size_t length, index;

		//--------------
		// â¸çsÇ≈êÿÇËï™ÇØ
		p = tmpS + tmpSLength + nDifference;
		tmpSLength += dwNumberOfBytesRead;
		end = tmpS + tmpSLength;
		*end = '\0';

		line = tmpS;
		while (p < end)
		{
			char c;

			c = *p;
			if (!__intrinsic_isleadbyte(c))
			{
				LPSTR next;
				char  includeFileName[MAX_PATH];

				next = p + 1;
				switch (c)
				{
				case '\r':
					if (next == end)
						goto NESTED_BREAK;
					if (*next == '\n')
						next++;
				case '\n':
					if (!GetIncludeFileName(includeFileName, line, p, FileName))
						vector_string_push_back_range(lines, line, !IS_INCLUDE_LINE_FEED(Mode) ? p : next);
					else if (RecursiveLoad(lines, includeFileName, Mode, GetSize))
						goto FAILED3;
					line = p = next;
					break;
				case '\\':
					if (next == end)
						goto NESTED_BREAK;
					c = *(next++);
					switch (c)
					{
					case '\r':
						if (next == end)
							goto NESTED_BREAK;
						if (*next == '\n')
							next++;
					case '\n':
						memcpy(p, next, end - next + 1);
						length = next - p;
						tmpSLength -= length;
						end -= length;
						break;
					default:
						p = next;
						if (__intrinsic_isleadbyte(c))
							p++;
						break;
					}
					break;
				default:
					p = next;
					break;
				}
			}
			else
			{
				p += 2;
			}
		}
	NESTED_BREAK:
		nDifference = p - end;

		index = line - tmpS;
		if (tmpSLength -= index)
		{
			size_t nRequired;

			nRequired = tmpSLength + READ_BLOCK_SIZE + 1;
			if (nRequired > tmpSBufferSize)
			{
				LPVOID lpMem;

				if ((INT_PTR)tmpSBufferSize >= 0)
					tmpSBufferSize <<= 1;
				else
					tmpSBufferSize = nRequired;
				lpMem = HeapReAlloc(hHeap, 0, tmpS, tmpSBufferSize);
				if (lpMem == NULL)
					goto FAILED3;
				tmpS = (LPSTR)lpMem;
				line = (LPSTR)lpMem + index;
			}
			memcpy(tmpS, line, tmpSLength);
		}
		//------
	}

	// ç≈èIçsÇäiî[
	if (tmpSLength)
	{
		char includeFileName[MAX_PATH];

		if (!GetIncludeFileName(includeFileName, tmpS, tmpS + tmpSLength, FileName))
			vector_string_push_back_range(lines, tmpS, tmpS + tmpSLength);
		else if (RecursiveLoad(lines, includeFileName, Mode, GetSize))
			goto FAILED3;
	}

	HeapFree(hHeap, 0, tmpS);

	if (Mode & (MODE_END_BYTE | MODE_START_BYTE))
	{
		size_t size;
		string *first, *last, *it;

#ifdef __BORLANDC__
		size = 0;
		for (first = lines->begin(); size < StartPos && first != lines->end(); first++)
			size += first->size();
		for (last = first; size < EndPos && last != lines->end(); last++)
			size += last->size();
		if (Mode & MODE_LINE_FEED_COUNT)
		{
			SList->insert(SList->end(), first, last);
		}
		else
		{
			SList->reserve(SList->size() + last - first);
			for (it = first; it != last; it++)
			{
				const char *begin = it->begin();
				const char *end = it->end();
				if (end != begin && *(end - 1) == '\n')
					if (--end != begin && *(end - 1) == '\r')
						--end;
				vector_string_push_back_range(SList, begin, end);
			}
		}
#else
		CheckSSGVersion(vector_begin(lines));
		size = 0;
		for (first = vector_begin(lines); size < StartPos && first != vector_end(lines); string_dtor(first), first++)
			size += string_size(first);
		for (last = first; size < EndPos && last != vector_end(lines); last++)
			size += string_size(last);
		for (it = last; it != vector_end(lines); it++)
			string_dtor(it);
		if (!(Mode & MODE_LINE_FEED_COUNT))
		{
			for (it = first; it != last; it++)
			{
				char *begin = string_begin(it);
				char *end = string_end(it);
				if (end == begin || *(end - 1) != '\n')
					continue;
				if (--end != begin && *(end - 1) == '\r')
					--end;
				*(string_end(it) = end) = '\0';
				string_shrink_to_fit(it);
			}
		}
		size = (size_t)last - (size_t)first;
		vector_BYTE_reserve(SList, vector_BYTE_size(SList) + size);
		memcpy(vector_end(SList), first, size);
		(LPBYTE)vector_end(SList) += size;
		vector_dtor(lines);
#endif
	}

DONE:
	CloseHandle(hFile);
	return 0;

FAILED3:
#ifndef __BORLANDC__
	vector_string_dtor(&buffer);
#endif
	HeapFree(hHeap, 0, tmpS);
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
#undef IS_INCLUDE_LINE_FEED
