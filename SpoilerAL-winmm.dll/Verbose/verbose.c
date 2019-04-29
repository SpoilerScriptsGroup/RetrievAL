#define _CRT_SECURE_NO_WARNINGS
#include "verbose.h"
#if ENABLE_VERBOSE
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include "intrinsic.h"

extern HANDLE hHeap;

char * __fastcall GetFileTitlePointerA(const char *lpFileNeme);
char * __fastcall UnescapeA(char *first, char **plast, BOOL breakSingleQuate);

static char lpFileName[MAX_PATH];

void init_verbose(HMODULE hModule)
{
	HANDLE hFile;

	if (!(hHeap = GetProcessHeap()))
		return;
	GetModuleFileNameA(hModule, lpFileName, _countof(lpFileName));
	strcpy(GetFileTitlePointerA(lpFileName), VERBOSE_LOG);
	hFile = CreateFileA(lpFileName, 0, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	CloseHandle(hFile);
}

static void __cdecl output(const char *buffer, unsigned int length)
{
	HANDLE hFile;
	DWORD  dwWritten;

	hFile = CreateFileA(lpFileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, buffer, length, &dwWritten, NULL);
	CloseHandle(hFile);
}

void __cdecl verbose_output(const char *buffer)
{
	output(buffer, strlen(buffer));
}

void __cdecl verbose_format(const char *format, ...)
{
	va_list      argptr;
	char         initialBuffer[1024], *heapBuffer, *buffer;
	const char   *src;
	unsigned int count, capacity, length;

	va_start(argptr, format);
	if ((capacity = count = (length = strlen(src = format)) + 1) <= _countof(initialBuffer))
	{
		heapBuffer = NULL;
		format = initialBuffer;
	}
	else
	{
		if (!(heapBuffer = (char *)HeapAlloc(hHeap, 0, (capacity *= 4) * sizeof(char))))
			goto FAILED_ALLOC;
		format = heapBuffer;
	}
	memcpy((void *)format, src, count * sizeof(char));
	buffer = (char *)format + length;
	UnescapeA((char *)format, &buffer, FALSE);
	*(buffer++) = '\0';
	length = _vsnprintf(buffer, count = capacity - (buffer - format), format, argptr);
	if (length < count)
		output(buffer, length);
	else if ((int)length >= 0)
	{
		char *memblock;

		count = length + 1;
		if (heapBuffer)
		{
			if (!(memblock = HeapReAlloc(hHeap, 0, heapBuffer, (*(size_t *)buffer -= (size_t)format) + count * sizeof(char))))
				goto FAILED_REALLOC;
			*(size_t *)buffer += (size_t)(format = heapBuffer = memblock);
		}
		else
		{
			if (!(memblock = HeapAlloc(hHeap, 0, count * sizeof(char))))
				goto FAILED_ALLOC;
			buffer = heapBuffer = memblock;
		}
		output(buffer, _vsnprintf(buffer, count, format, argptr));
	}
	if (heapBuffer)
FAILED_REALLOC:
		HeapFree(hHeap, 0, heapBuffer);
FAILED_ALLOC:
	va_end(argptr);
}
#endif
