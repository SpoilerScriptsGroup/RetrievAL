#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

EXTERN_C HANDLE hHeap;

char * __fastcall UnescapePrintfBuffer(char *first, char *last);
int __fastcall DebugPrintV(const char *format, va_list argptr);

#if defined(_MSC_VER) && defined(_M_IX86)
__declspec(naked) int __cdecl DebugPrint(const char *format, ...)
{
	__asm
	{
		mov     ecx, dword ptr [esp + 4]
		lea     edx, [esp + 8]
		jmp     DebugPrintV
	}
}
#else
int __cdecl DebugPrint(const char *format, ...)
{
	va_list argptr;
	int     length;

	va_start(argptr, format);
	length = DebugPrintV(format, argptr);
	va_end(argptr);
	return length;
}
#endif

int __fastcall DebugPrintV(const char *format, va_list argptr)
{
	char stackBuffer[256];
	int  length;

	length = _vsnprintf(stackBuffer, _countof(stackBuffer), format, argptr);
	if ((unsigned int)length < _countof(stackBuffer))
	{
		UnescapePrintfBuffer(stackBuffer, stackBuffer + length);
		OutputDebugStringA(stackBuffer);
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
				OutputDebugStringA(heapBuffer);
			}
			HeapFree(hHeap, 0, heapBuffer);
		}
	}
	return length;
}

