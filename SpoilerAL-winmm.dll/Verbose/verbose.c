#define _CRT_SECURE_NO_WARNINGS
#include "verbose.h"
#if ENABLE_VERBOSE
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>

extern HANDLE hHeap;

char * __fastcall GetFileTitlePointerA(const char *lpFileNeme);
char * __fastcall UnescapePrintfBuffer(char *first, char *last);

static char lpFileName[MAX_PATH];

void init_verbose(HMODULE hModule)
{
	HANDLE hFile;

	GetModuleFileNameA(hModule, lpFileName, _countof(lpFileName));
	strcpy(GetFileTitlePointerA(lpFileName), VERBOSE_LOG);
	hFile = CreateFileA(lpFileName, 0, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	CloseHandle(hFile);
}

static void output(VERBOSE_LEVEL level, const char *buffer)
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

void __cdecl verbose_output(VERBOSE_LEVEL level, const char *format, ...)
{
	char    stackBuffer[1024];
	va_list argptr;
	int     length;

	va_start(argptr, format);
	length = _vsnprintf(stackBuffer, _countof(stackBuffer), format, argptr);
	if ((unsigned int)length < _countof(stackBuffer))
	{
		UnescapePrintfBuffer(stackBuffer, stackBuffer + length);
		output(level, stackBuffer);
	}
	else if (length >= 0)
	{
		unsigned int size;
		char         *heapBuffer;

		size = length + 1;
		heapBuffer = (char *)HeapAlloc(hHeap, 0, size * sizeof(char));
		if (heapBuffer)
		{
			length = _vsnprintf(heapBuffer, size, format, argptr);
			if ((unsigned int)length < size)
			{
				UnescapePrintfBuffer(heapBuffer, heapBuffer + length);
				output(level, heapBuffer);
			}
			HeapFree(hHeap, 0, heapBuffer);
		}
	}
	va_end(argptr);
}
#endif
