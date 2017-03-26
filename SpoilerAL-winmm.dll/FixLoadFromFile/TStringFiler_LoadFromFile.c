#include <windows.h>
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
#define bcb6_std_vector_string                   vector<string>
#define bcb6_std_vector_string_iterator          vector<string>::iterator
#define bcb6_std_vector_size(vec, elemType)      (vec)->size()
#define bcb6_std_vector_string_clear(vec)        (vec)->clear()
#define bcb6_std_vector_string_resize(vec, size) (vec)->resize(size)
#define bcb6_std_vector_begin(vec, elemType)     vec->begin()
#define bcb6_std_vector_end(vec, elemType)       vec->end()
#define bcb6_std_string                          std::string
__inline void bcb6_std_string_assign_cstr_with_length(string *dest, LPCSTR src, size_t length)
{
	dest->resize(length);
	memcpy(dest->begin(), src, length);
}
#else
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#define bcb6_std_vector_string          bcb6_std_vector
#define bcb6_std_vector_string_iterator bcb6_std_string *
EXTERN_C void __fastcall bcb6_std_vector_string_clear(bcb6_std_vector *vec);
EXTERN_C void __fastcall bcb6_std_vector_string_resize(bcb6_std_vector *vec, size_t size);
#pragma function(memcmp)
#endif

extern HANDLE hHeap;

__inline void bcb6_std_vector_string_push_back(bcb6_std_vector_string *vec, const char *cstr, size_t length)
{
	bcb6_std_vector_string_resize(vec, bcb6_std_vector_size(vec, bcb6_std_string) + 1);
	bcb6_std_string_assign_cstr_with_length(bcb6_std_vector_end(vec, bcb6_std_string) - 1, cstr, length);
}

#ifdef __BORLANDC__
unsigned long TStringFiler::LoadFromFile(
#else
unsigned long __cdecl TStringFiler_LoadFromFile(
#endif
	bcb6_std_vector_string *SList,
	const char             *FileName,
	unsigned long          GetSize,
	unsigned long          Mode,
	unsigned long          StartPos,
	unsigned long          EndPos,
	const char             *StartWord,
	const char             *EndWord)
{
	#define MODE_END_LINE        0x0001
	#define MODE_END_STR         0x0002
	#define MODE_END_BYTE        0x0004
	#define MODE_START_BYTE      0x0010
	#define MODE_LINE_FEED_COUNT 0x0100
	#define READ_BLOCK_SIZE      0x00010000

	HANDLE    hFile;
	DWORD     dwFileSize;
	LPSTR     tmpS;
	size_t    tmpSLength;
	size_t    tmpSBufferSize;
	ptrdiff_t nDifference;
	DWORD     dwNumberOfBytesRead;

	assert((Mode & MODE_END_LINE) == 0);
	assert((Mode & MODE_END_STR) == 0);
	assert(*StartWord == '\0');
	assert(*EndWord == '\0');

	__assume(GetSize == 0xFFFF);
	__assume((Mode & MODE_END_LINE) == 0);
	__assume((Mode & MODE_END_STR) == 0);
	__assume(*StartWord == '\0');
	__assume(*EndWord == '\0');

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

	// 一旦空になります
	bcb6_std_vector_string_clear(SList);

	if (dwFileSize == 0)
		goto DONE;

	if (EndPos > dwFileSize)
		EndPos = dwFileSize;

	// 前スキップ
	if ((Mode & MODE_START_BYTE) && StartPos)
	{
		DWORD dwFilePosition;

		// ファイルサイズより大きいよ…
		if (EndPos <= StartPos)
			goto DONE;
		// バイト数指定
		dwFilePosition = SetFilePointer(hFile, StartPos, NULL, FILE_BEGIN);
		if (dwFilePosition == INVALID_SET_FILE_POINTER)
			goto DONE;
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
		// 改行で切り分け
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

				next = p + 1;
				switch (c)
				{
				case '\r':
					if (next == end)
						goto NESTED_BREAK;
					if (*next == '\n')
						next++;
				case '\n':
					bcb6_std_vector_string_push_back(SList, line, (!(Mode & MODE_LINE_FEED_COUNT) ? p : next) - line);
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

	// 最終行を格納
	if (tmpSLength)
		bcb6_std_vector_string_push_back(SList, tmpS, tmpSLength);

	HeapFree(hHeap, 0, tmpS);

DONE:
	CloseHandle(hFile);
	return 0;

FAILED3:
	HeapFree(hHeap, 0, tmpS);
FAILED2:
	CloseHandle(hFile);
FAILED1:
	return 1;

	#undef MODE_END_LINE
	#undef MODE_END_STR
	#undef MODE_END_BYTE
	#undef MODE_START_BYTE
	#undef MODE_LINE_FEED_COUNT
	#undef READ_BLOCK_SIZE
}

