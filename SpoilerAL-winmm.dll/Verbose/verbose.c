#define _CRT_SECURE_NO_WARNINGS
#include "verbose.h"
#if ENABLE_VERBOSE
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>

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

static void output(const char *buffer)
{
	HANDLE hFile;
	DWORD  dwWritten;

	hFile = CreateFileA(lpFileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, buffer, strlen(buffer), &dwWritten, NULL);
	WriteFile(hFile, "\r\n", 2, &dwWritten, NULL);
	CloseHandle(hFile);
}

void __cdecl verbose_output(const char *format, ...)
{
	va_list      argptr;
	unsigned int count, capacity;
	char         *buffer;

	va_start(argptr, format);
	if (buffer = (char *)HeapAlloc(hHeap, 0, (capacity = (count = strlen(format) + 1) + 1024) * sizeof(char)))
	{
		char *dest;
		int  ret;

		memcpy(buffer, format, count * sizeof(char));
		dest = buffer + count - 1;
		UnescapeA(buffer, &dest, FALSE);
		*(dest++) = '\0';
		ret = _vsnprintf(dest, count = capacity - (dest - buffer), buffer, argptr);
		if ((unsigned int)ret < count)
			output(dest);
		else if (ret >= 0)
		{
			void *memblock;

			if (memblock = HeapReAlloc(hHeap, 0, buffer, (capacity = (count = dest - buffer) + ret) * sizeof(char)))
			{
				dest = (buffer = (char *)memblock) + count;
				ret = _vsnprintf(dest, count = ret, buffer, argptr);
				if ((unsigned int)ret < count)
					output(dest);
			}
		}
		HeapFree(hHeap, 0, buffer);
	}
	va_end(argptr);
}
#endif
