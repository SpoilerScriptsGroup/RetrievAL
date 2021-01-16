#include <windows.h>
#include <winioctl.h>
#include <assert.h>
#include "intrinsic.h"
#include "ProcessMonitor/GetFileTitlePointer.h"

#if (!defined(_MSC_VER) || _MSC_VER < 1200) && !defined(__assume)
#define __assume(expression)
#endif

#ifdef __BORLANDC__
#pragma warn -8060
#undef InlineIsEqualGUID
#define InlineIsEqualGUID(rguid1, rguid2) (                         \
    ((unsigned long *)rguid1)[0] == ((unsigned long *)rguid2)[0] && \
    ((unsigned long *)rguid1)[1] == ((unsigned long *)rguid2)[1] && \
    ((unsigned long *)rguid1)[2] == ((unsigned long *)rguid2)[2] && \
    ((unsigned long *)rguid1)[3] == ((unsigned long *)rguid2)[3])
#define vector_begin(v)                               (v)->begin()
#define vector_end(v)                                 (v)->end()
#define vector_size(v)                                (v)->size()
#define vector_empty(v)                               (v)->empty()
#define vector_push_back(v, x)                        (v)->push_back(x)
#define vector_pop_back(v)                            (v)->pop_back()
#define vector_insert_range(v, position, first, last) (v)->insert(position, first, last)
#define vector_erase(v, position)                     (v)->erase(position)
#define vector_string                                 vector<string>
#define vector_string_clear(v)                        (v)->clear()
#define vector_string_push_back_range(v, first, last) (v)->push_back(string(first, last))
#define vector_GUID                                   vector<GUID>
#define string_begin(s)                               (s)->begin()
#define string_end(s)                                 (s)->end()
#else
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#define typename GUID
#include "bcb6_std_vector_template.h"
#endif

extern HANDLE hHeap;

#ifndef __BORLANDC__
EXTERN_C void __fastcall CheckSSGVersion(const char *begin, const char *end);
#endif

static __inline BOOL GetIncludeFileName(
	OUT char       *lpFileName,
	IN  const char *begin,
	IN  const char *end,
	IN  const char *lpBaseFileName)
{
	size_t fileNameLength;
	size_t directoryLength;
	size_t length;

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

static __inline BOOL GetFileObjectIdA(
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
#ifndef USE_DeviceIoControl
		BY_HANDLE_FILE_INFORMATION fileInfo;

		if (GetFileInformationByHandle(hFile, &fileInfo)) {
			lpFileObjectId->Data1 = fileInfo.nFileIndexLow;
			lpFileObjectId->Data2 = LOWORD(fileInfo.nFileIndexHigh);
			lpFileObjectId->Data3 = HIWORD(fileInfo.nFileIndexHigh);
			*(LPDWORD) lpFileObjectId->Data4    = fileInfo.dwVolumeSerialNumber;
			*(LPDWORD)&lpFileObjectId->Data4[4] = 0;
#else
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
#endif
			bSuccess = TRUE;
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

#ifdef __BORLANDC__
unsigned long TStringFiler::LoadFromFile(
#else
unsigned long __cdecl TStringFiler_LoadFromFile(
#endif
	IN OUT vector_string *SList,
	IN     const char    *FileName,
	IN     ULONG_PTR     GetSize,
	IN     unsigned long Mode,
	IN     unsigned long StartPos,
	IN     unsigned long EndPos,
	IN     const char    *StartWord,  // unused
	IN     const char    *EndWord)    // unused
{
	#define MODE_END_LINE   0x0001
	#define MODE_END_STR    0x0002
	#define MODE_END_BYTE   0x0004
	#define MODE_START_BYTE 0x0010
	#define MODE_LINE_FEED  0x0100
	#define MODE_RECURSIVE  0x0200
	#define MODE_EXTRACT    (MODE_END_BYTE | MODE_START_BYTE)
	#define READ_BLOCK_SIZE 0x00010000

	HANDLE    hFile;
	DWORD     dwFileSize, dwNumberOfBytesToRead, dwNumberOfBytesRead;
	char      *buffer;
	size_t    bufferLength, bufferCapacity;
	ptrdiff_t difference;
	size_t    position;
#ifndef __BORLANDC__
	BOOLEAN   firstLine;
#endif

	assert((Mode & MODE_END_LINE) == 0);
	assert((Mode & MODE_END_STR) == 0);

	// 一旦空になります
	vector_string_clear(SList);

	hFile = CreateFileA(
		FileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	// 読み込みエラー
	if (hFile == INVALID_HANDLE_VALUE)
		goto FAILED1;

	dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == MAXDWORD)
		goto FAILED2;

	if (dwFileSize == 0)
		goto DONE;

	if (Mode & MODE_EXTRACT)
 	{
		if (EndPos > dwFileSize)
			EndPos = dwFileSize;
		// ファイルサイズより大きいよ…
		if (EndPos <= StartPos)
			goto DONE;
#ifdef __BORLANDC__
		if (SetFilePointer(hFile, StartPos, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			goto DONE;
		position = StartPos;
	}
	else
	{
		position = 0;
#endif
	}

	bufferCapacity = min(dwFileSize, READ_BLOCK_SIZE * 2 - 1);
	buffer = (char *)HeapAlloc(hHeap, 0, bufferCapacity + 1);
	if (!buffer)
		goto FAILED2;
	bufferLength = 0;

#ifndef __BORLANDC__
	firstLine = TRUE;
	position = 0;
#endif
	difference = 0;
	dwNumberOfBytesToRead = min(READ_BLOCK_SIZE, bufferCapacity);
	while (ReadFile(hFile, buffer + bufferLength, dwNumberOfBytesToRead, &dwNumberOfBytesRead, NULL) && dwNumberOfBytesRead)
	{
		char   *p, *end, *line;
		size_t length, index;

		//--------------
		// 改行で切り分け
		p = buffer + bufferLength + difference;
		bufferLength += dwNumberOfBytesRead;
		end = buffer + bufferLength;
		*end = '\0';

		line = buffer;
		while (p < end)
		{
			char c, *next;

			c = *p;
			next = p + 1;
			switch ((unsigned char)c)
			{
			case '\r':
				if (next == end)
					break;
				if (*next == '\n')
					next++;
				/* FALLTHROUGH */
			case '\n':
#ifndef __BORLANDC__
				if (firstLine)
				{
					firstLine = FALSE;
					if (Mode & MODE_EXTRACT)
					{
						CheckSSGVersion(line, p);
						if (SetFilePointer(hFile, StartPos, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
							goto END_OF_READ;
						p = end;
						line = buffer + bufferLength;
						position = StartPos;
						break;
					}
				}
#endif
				vector_string_push_back_range(SList, line, !(Mode & MODE_LINE_FEED) ? p : next);
				if ((Mode & MODE_EXTRACT) && (position += next - line) >= EndPos)
					goto END_OF_READ;
				line = p = next;
				continue;
			case '\\':
			ESCAPE_SEQUENCE:
				if (next == end)
					break;
				c = *(next++);
				switch ((unsigned char)c)
				{
				case ' ':
				case '\t':
				case '\v':
				case '\f':
					goto ESCAPE_SEQUENCE;
				case '\r':
					if (next == end)
						break;
					if (*next == '\n')
						next++;
					/* FALLTHROUGH */
				case '\n':
					memcpy(p, next, end - next + 1);
					length = next - p;
					bufferLength -= length;
					end -= length;
					continue;
#if CODEPAGE_SUPPORT
				default:
					if (__intrinsic_isleadbyte(p[2]))
						p++;
#else
				case_unsigned_leadbyte:
					p++;
					/* FALLTHROUGH */
				default:
#endif
					p += 2;
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
				/* FALLTHROUGH */
			default:
#endif
				p = next;
				continue;
			}
			break;
		}
		difference = p - end;

		index = line - buffer;
		if (bufferLength -= index)
		{
			size_t require;

			require = bufferLength + READ_BLOCK_SIZE;
			if (require > bufferCapacity)
			{
				void *lpMem;

				if ((ptrdiff_t)++bufferCapacity >= 0)
					bufferCapacity <<= 1;
				else
					bufferCapacity = require + 1;
				lpMem = HeapReAlloc(hHeap, 0, buffer, bufferCapacity--);
				if (!lpMem)
					goto FAILED3;
				buffer = (char *)lpMem;
				line = (char *)lpMem + index;
			}
			memcpy(buffer, line, bufferLength);
		}
		//------
	}

	// 最終行を格納
	if (bufferLength)
		vector_string_push_back_range(SList, buffer, buffer + bufferLength);

END_OF_READ:
	HeapFree(hHeap, 0, buffer);
	CloseHandle(hFile);

	if (!(Mode & MODE_LINE_FEED))
	{
		vector_GUID   loaded;
		vector_GUID   *loadedFiles;
		unsigned long recursiveMode;
		GUID          fileObjectId;
		string        *it;

		recursiveMode = Mode;
		if (recursiveMode & MODE_RECURSIVE)
		{
			loadedFiles = (vector_GUID *)GetSize;
		}
		else
		{
			recursiveMode &= ~MODE_EXTRACT;
			recursiveMode |= MODE_RECURSIVE;
			if (!GetFileObjectIdA(&fileObjectId, FileName))
				goto FAILED1;
#ifndef __BORLANDC__
			vector_ctor(&loaded);
#endif
			vector_push_back(loadedFiles = &loaded, fileObjectId);
		}
		for (it = vector_begin(SList); it != vector_end(SList); it++)
		{
			char          includeFileName[MAX_PATH];
			GUID          *find;
			vector_string buffer;
			unsigned long ret;

			if (!GetIncludeFileName(includeFileName, string_begin(it), string_end(it), FileName))
				continue;
			if (!GetFileObjectIdA(&fileObjectId, includeFileName))
				continue;
			for (find = vector_begin(loadedFiles); find != vector_end(loadedFiles); find++)
				if (InlineIsEqualGUID(find, &fileObjectId))
					break;
			if (find != vector_end(loadedFiles))
				continue;
			vector_push_back(loadedFiles, fileObjectId);
#ifdef __BORLANDC__
			ret = LoadFromFile(
#else
			vector_ctor(&buffer);
			ret = TStringFiler_LoadFromFile(
#endif
				&buffer, includeFileName, (ULONG_PTR)loadedFiles, recursiveMode, 0, 0, NULL, NULL);
			vector_pop_back(loadedFiles);
			if (!ret)
			{
				ptrdiff_t offset = (char *)it - (char *)vector_begin(SList);
#ifndef __BORLANDC__
				string_dtor(it);
#endif
				if (!vector_empty(&buffer))
				{
					string *src;

					src = vector_begin(&buffer);
					*(it++) = *(src++);
					if (src != vector_end(&buffer))
						vector_insert_range(SList, it, src, vector_end(&buffer));
				}
				else
				{
					vector_erase(SList, it);
				}
				it = (string *)((char *)vector_begin(SList) + offset) - 1;
#ifndef __BORLANDC__
				vector_dtor(&buffer);
#endif
			}
			else
			{
#ifndef __BORLANDC__
				vector_string_dtor(&buffer);
				if (!(Mode & MODE_RECURSIVE))
					vector_dtor(&loaded);
#endif
				goto FAILED1;
			}
		}
#ifndef __BORLANDC__
		if (!(Mode & MODE_RECURSIVE))
			vector_dtor(&loaded);
#endif
	}

#ifndef __BORLANDC__
	vector_shrink_to_fit(SList);
#endif

	return 0;

DONE:
	CloseHandle(hFile);
	return 0;

FAILED3:
	HeapFree(hHeap, 0, buffer);
FAILED2:
	CloseHandle(hFile);
FAILED1:
	return 1;

	#undef MODE_END_LINE
	#undef MODE_END_STR
	#undef MODE_END_BYTE
	#undef MODE_START_BYTE
	#undef MODE_LINE_FEED
	#undef MODE_RECURSIVE
	#undef MODE_EXTRACT
	#undef READ_BLOCK_SIZE
}
