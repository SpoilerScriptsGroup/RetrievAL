#include <windows.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#ifdef __BORLANDC__
#include <stdio.h>
#else
#include "bcb6_stdio.h"
#define _vsnprintf bcb6__vsnprintf
#endif

#ifdef __BORLANDC__
#include "MainUnit.h"
#define TMainForm_Guide(Mes, IsClear) (MainForm)->Guide(Mes, IsClear)
#else
#include "TMainForm.h"
#endif

EXTERN_C HANDLE hHeap;

int __stdcall GuidePrintV(const char *format, va_list argptr)
{
	char buffer[256];
	int  length;

	length = _vsnprintf(buffer, _countof(buffer), format, argptr);
	if ((unsigned int)length < _countof(buffer))
	{
		TMainForm_Guide(buffer, FALSE);
	}
	else
	{
		unsigned int size;
		char         *heapBuffer;

		size = (length >= 0 ? (length + 1) * sizeof(char) : sizeof(buffer) * 2);
		heapBuffer = (char *)HeapAlloc(hHeap, 0, size);
		if (heapBuffer)
		{
			for (; ; )
			{
				LPVOID memblock;

				length = _vsnprintf(heapBuffer, size / sizeof(char), format, argptr);
				if ((unsigned int)length < size / sizeof(char))
				{
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

int __cdecl GuidePrint(const char *format, ...)
{
	va_list argptr;
	int     length;

	va_start(argptr, format);
	length = GuidePrintV(format, argptr);
	va_end(argptr);
	return length;
}

