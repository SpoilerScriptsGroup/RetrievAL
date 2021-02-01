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
			((LPDWORD)lpFileObjectId)[0] = fileInfo.nFileIndexLow;
			((LPDWORD)lpFileObjectId)[1] = fileInfo.nFileIndexHigh;
			((LPDWORD)lpFileObjectId)[2] = fileInfo.dwVolumeSerialNumber;
			((LPDWORD)lpFileObjectId)[3] = 0;
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

	HANDLE  hFile;
	DWORD   dwFileSize;
	HANDLE  hMap;
	LPCBYTE lpMapViewOfFile, p, end, begin, prev;
	string  *s;

	assert((Mode & MODE_END_LINE) == 0);
	assert((Mode & MODE_END_STR) == 0);

	// ˆê’U‹ó‚É‚È‚è‚Ü‚·
	vector_string_clear(SList);

	hFile = CreateFileA(
		FileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		// “Ç‚Ýž‚ÝƒGƒ‰[
		goto FAILED1;

	dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == MAXDWORD)
		goto FAILED2;

	if (!(Mode & MODE_START_BYTE))
		StartPos = 0;
	if (!(Mode & MODE_END_BYTE) || EndPos > dwFileSize)
		EndPos = dwFileSize;
	if (StartPos >= EndPos)
		goto DONE;

	hMap = CreateFileMappingA(
		hFile,
		NULL,
		PAGE_READONLY,
		0,
		0,
		NULL);
	if (!hMap)
		goto FAILED3;

	lpMapViewOfFile = (LPCBYTE)MapViewOfFile(
		hMap,
		FILE_MAP_READ,
		0,
		0,
		0);
	if (!lpMapViewOfFile)
		goto FAILED4;

#ifndef __BORLANDC__
	end = (p = lpMapViewOfFile) + dwFileSize;
	do
	{
		switch (*(prev = p++))
		{
		default:
			continue;
		case '\r':
			if (p < end && *p == '\n')
				p++;
			/* FALLTHROUGH */
		case '\n':
			CheckSSGVersion(lpMapViewOfFile, prev);
			break;
		case_unsigned_leadbyte:
			if (p >= end)
				break;
			p++;
			continue;
		}
		break;
	} while (p < end);
#endif

	p = lpMapViewOfFile + StartPos;
	end = lpMapViewOfFile + EndPos;

	//--------------
	// ‰üs‚ÅØ‚è•ª‚¯
	begin = p;
	s = NULL;
	do
	{
		BYTE c;

		switch (*(prev = p++))
		{
		default:
			continue;
		case '\r':
			if (p < end && *p == '\n')
				p++;
			/* FALLTHROUGH */
		case '\n':
			if (!s)
			{
				vector_string_push_back_range(SList, begin, !(Mode & MODE_LINE_FEED) ? prev : p);
			}
			else
			{
				string_append_range(s, begin, !(Mode & MODE_LINE_FEED) ? prev : p);
				string_shrink_to_fit(s);
				s = NULL;
			}
			begin = p;
			continue;
		case '\\':
			if (p >= end)
				break;
			switch (c = *(p++))
			{
			default:
				continue;
			case '\r':
				if (p >= end)
					break;
				if (*p == '\n')
					p++;
				/* FALLTHROUGH */
			case '\n':
				if (p >= end)
					break;
				if (!s)
				{
					vector_string_push_back_range(SList, begin, prev);
					s = vector_end(SList) - 1;
				}
				else
				{
					string_append_range(s, begin, prev);
				}
				begin = p;
				continue;
			case_unsigned_leadbyte:
				goto LEADBYTE;
			}
			end = prev;
			break;
		case_unsigned_leadbyte:
		LEADBYTE:
			if (p >= end)
				break;
			p++;
			continue;
		}
		break;
	} while (p < end);
	//------

	// ÅIs‚ðŠi”[
	if (begin < end)
	{
		if (!s)
		{
			vector_string_push_back_range(SList, begin, end);
		}
		else
		{
			string_append_range(s, begin, end);
			string_shrink_to_fit(s);
		}
	}

	UnmapViewOfFile(lpMapViewOfFile);
	CloseHandle(hMap);
DONE:
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

FAILED4:
	UnmapViewOfFile(lpMapViewOfFile);
FAILED3:
	CloseHandle(hMap);
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
}
