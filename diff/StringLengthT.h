#ifdef StringLengthT
#ifdef __BORLANDC__
#pragma warn -8017
#endif
#include <windows.h>
#include <tchar.h>
#include "PageSize.h"

#ifdef __BORLANDC__
#pragma warn -8058
#pragma warn -8060
#pragma warn -8075
EXTERN_C DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

#ifndef _tcsnlen
#ifdef _UNICODE
#define _tcsnlen wcsnlen
static size_t wcsnlen(const wchar_t *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		while (*(string++) && --length);
		length = maxlen - length;
	}
	return length;
}
#else
#define _tcsnlen strnlen
static size_t strnlen(const char *string, size_t maxlen)
{
	const char *p;

	if (maxlen && (p = memchr(string, '\0', maxlen)))
		return p - string;
	else
		return maxlen;
}
#endif
#endif

EXTERN_C size_t __stdcall StringLengthT(HANDLE hProcess, LPCTSTR lpString)
{
	if (lpString)
	{
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
#ifdef _UNICODE
			BYTE    buffer[PAGE_SIZE + 1];
#else
			BYTE    buffer[PAGE_SIZE];
#endif
			LPCBYTE src;
			size_t  size, length;

			src = (LPCBYTE)lpString;
			if (size = -(ptrdiff_t)lpString & (PAGE_SIZE - 1))
			{
				size_t maxlen;

				if (!ReadProcessMemory(hProcess, src, buffer, size, NULL))
					goto FAILED;
				src += size;
				maxlen = size;
#ifdef _UNICODE
				if (maxlen /= sizeof(TCHAR))
#endif
					if ((length = _tcsnlen((LPCTSTR)buffer, maxlen)) < maxlen)
						return length;
			}
#ifdef _UNICODE
			if (!(size & 1))
#endif
			{
				while (ReadProcessMemory(hProcess, src, buffer, PAGE_SIZE, NULL))
				{
					#define MAXLEN (PAGE_SIZE / sizeof(TCHAR))

					if ((length = _tcsnlen((LPCTSTR)buffer, MAXLEN)) < MAXLEN)
						return ((LPCTSTR)src - lpString) + length;
					src += PAGE_SIZE;

					#undef MAXLEN
				}
			}
#ifdef _UNICODE
			else
			{
				buffer[0] = buffer[size - 1];
				while (ReadProcessMemory(hProcess, src, buffer + 1, PAGE_SIZE, NULL))
				{
					#define MAXLEN (PAGE_SIZE / sizeof(TCHAR))

					if ((length = _tcsnlen((LPCTSTR)buffer, MAXLEN)) < MAXLEN)
						return ((LPCTSTR)src - lpString) + length;
					src += PAGE_SIZE;
					buffer[0] = buffer[PAGE_SIZE];

					#undef MAXLEN
				}
			}
#endif
		}
		else
		{
			return _tcslen(lpString);
		}
	}
FAILED:
	return 0;
}
#endif
