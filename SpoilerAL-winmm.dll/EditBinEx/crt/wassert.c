#ifndef NDEBUG

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include "wcslen.h"
#include "wmemcpy.h"
#include <stdlib.h>

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

void __cdecl _wassert(const wchar_t *Message, const wchar_t *File, unsigned Line)
{
	wchar_t String[2048 / sizeof (wchar_t)];
	DWORD   LastError;
	size_t  Length;
	wchar_t *p;

	LastError = GetLastError();
	__movsw(String,
		L"Debug Assertion Failed!\n"
		L"\n"
		L"Program: ",
		34);
	p = String + 34;
	Length = GetModuleFileNameW(NULL, p, _countof(String) - (p - (wchar_t *)String));
	if (Length != 0)
	{
		if (Length <= MAX_PATH + 3)
		{
			p += Length;
		}
		else
		{
			*p       = L'.';
			*(p + 1) = L'.';
			*(p + 2) = L'.';
			__movsw(p + 3, p + Length - MAX_PATH, MAX_PATH);
			p += MAX_PATH + 3;
		}
	}
	*p = L'\n';
	*(p + 1) = L'F';
	*(p + 2) = L'i';
	*(p + 3) = L'l';
	*(p + 4) = L'e';
	*(p + 5) = L':';
	*(p + 6) = L' ';
	p += 7;
	Length = wcslen(File);
	if (Length != 0)
	{
		if (Length <= MAX_PATH + 3)
		{
			__movsw(p, File, Length);
			p += Length;
		}
		else
		{
			*p       = L'.';
			*(p + 1) = L'.';
			*(p + 2) = L'.';
			__movsw(p += 3, File + Length - MAX_PATH, MAX_PATH);
			p += MAX_PATH;
		}
	}
	*p = L'\n';
	*(p + 1) = L'L';
	*(p + 2) = L'i';
	*(p + 3) = L'n';
	*(p + 4) = L'e';
	*(p + 5) = L':';
	*(p + 6) = L' ';
	p += 7;
	if (Line >= 10UL)
	{
		if (Line >= 100UL)
		{
			if (Line >= 1000UL)
			{
				if (Line >= 10000UL)
				{
					if (Line >= 100000UL)
					{
						if (Line >= 1000000UL)
						{
							if (Line >= 10000000UL)
							{
								if (Line >= 100000000UL)
								{
									if (Line >= 1000000000UL)
									{
										*(p++) = L'0' + (wchar_t)(Line / 1000000000UL);
										Line %= 1000000000UL;
									}
									*(p++) = L'0' + (wchar_t)(Line / 100000000UL);
									Line %= 100000000UL;
								}
								*(p++) = L'0' + (wchar_t)(Line / 10000000UL);
								Line %= 10000000UL;
							}
							*(p++) = L'0' + (wchar_t)(Line / 1000000UL);
							Line %= 1000000UL;
						}
						*(p++) = L'0' + (wchar_t)(Line / 100000UL);
						Line %= 100000UL;
					}
					*(p++) = L'0' + (wchar_t)(Line / 10000UL);
					Line %= 10000UL;
				}
				*(p++) = L'0' + (wchar_t)(Line / 1000UL);
				Line %= 1000UL;
			}
			*(p++) = L'0' + (wchar_t)(Line / 100UL);
			Line %= 100UL;
		}
		*(p++) = L'0' + (wchar_t)(Line / 10UL);
		Line %= 10UL;
	}
	*(p++) = L'0' + (wchar_t)Line;
	__movsw(p, L"\n\nExpression: ", 14);
	p += 14;
	Length = wcslen(Message);
	if (Length != 0)
	{
		if (Length <= MAX_PATH + 3)
		{
			__movsw(p, Message, Length);
			p += Length;
		}
		else
		{
			*p       = L'.';
			*(p + 1) = L'.';
			*(p + 2) = L'.';
			__movsw(p += 3, Message + Length - MAX_PATH, MAX_PATH);
			p += MAX_PATH;
		}
	}
	__movsw(p,
		L"\n"
		L"\n"
		L"For infomation on how your program can cause an assertion\n"
		L"failure, see the Visual C++ documentation on asserts.\0",
		114);
	MessageBoxW(NULL, String, L"Microsoft Visual C++ Debug Library", MB_OK | MB_ICONHAND | MB_TOPMOST);
	ExitProcess(LastError);
}

#endif
