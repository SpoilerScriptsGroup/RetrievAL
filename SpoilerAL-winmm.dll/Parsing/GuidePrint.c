#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#ifdef __BORLANDC__
#include "MainUnit.h"
#define TMainForm_Guide(Mes, IsClear) (MainForm)->Guide(Mes, IsClear)
#else
#include "TMainForm.h"
#endif

EXTERN_C HANDLE hHeap;

char *__fastcall Unescape(char *first, char *last);
int __fastcall GuidePrintV(const char *format, va_list argptr);

#if defined(_MSC_VER) && defined(_M_IX86)
__declspec(naked) int __cdecl GuidePrint(const char *format, ...)
{
	__asm
	{
		mov     ecx, dword ptr [esp + 4]
		lea     edx, [esp + 8]
		jmp     GuidePrintV
	}
}
#else
int __cdecl GuidePrint(const char *format, ...)
{
	va_list argptr;
	int     length;

	va_start(argptr, format);
	length = GuidePrintV(format, argptr);
	va_end(argptr);
	return length;
}
#endif

int __fastcall GuidePrintV(const char *format, va_list argptr)
{
	char stackBuffer[256];
	int  length;

	length = _vsnprintf(stackBuffer, _countof(stackBuffer), format, argptr);
	if ((unsigned int)length < _countof(stackBuffer))
	{
		TMainForm_Guide(stackBuffer, FALSE);
	}
	else
	{
		unsigned int size;
		char         *heapBuffer;

		size = (length >= 0 ? (length + 1) * sizeof(char) : sizeof(stackBuffer) * 2);
		heapBuffer = (char *)HeapAlloc(hHeap, 0, size);
		if (heapBuffer)
		{
			for (; ; )
			{
				LPVOID memblock;

				length = _vsnprintf(heapBuffer, size / sizeof(char), format, argptr);
				if ((unsigned int)length < size / sizeof(char))
				{
					Unescape(heapBuffer, heapBuffer + length);
					TMainForm_Guide(heapBuffer, FALSE);
					break;
				}
				if (length >= 0)
					size = (length + 1) * sizeof(char);
				else if ((int)(size <<= 1) < 0)
					break;
				memblock = HeapReAlloc(hHeap, 0, heapBuffer, size);
				if (!memblock)
					break;
				heapBuffer = (char *)memblock;
			}
			HeapFree(hHeap, 0, heapBuffer);
		}
	}
	return length;
}

