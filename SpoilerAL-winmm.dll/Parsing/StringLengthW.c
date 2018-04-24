#include <windows.h>
#include "PageSize.h"

#ifdef __BORLANDC__
#pragma warn -8060
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

EXTERN_C size_t __stdcall StringLengthW(HANDLE hProcess, LPCWSTR lpString)
{
	wchar_t buffer[PAGE_SIZE / sizeof(wchar_t) + 1];
	size_t  size;
	size_t  length;
	LPWSTR  end, p;
	LPCBYTE src;

	if (!lpString)
		return 0;
	if (!hProcess || GetProcessId(hProcess) == GetCurrentProcessId())
		return wcslen(lpString);
	if (length = size = -(ptrdiff_t)lpString & (PAGE_SIZE - 1))
	{
		if (!ReadProcessMemory(hProcess, lpString, buffer, size, NULL))
			return 0;
		if (length /= sizeof(wchar_t))
		{
			end = buffer + length;
			p = buffer;
			do
				if (!*p)
					return p - buffer;
			while (++p < end);
		}
	}
	end = (LPWSTR)((LPBYTE)buffer + PAGE_SIZE);
	src = (LPCBYTE)lpString + size;
	if (!(size & 1))
	{
		while (ReadProcessMemory(hProcess, src, buffer, PAGE_SIZE, NULL))
		{
			p = buffer;
			do
				if (!*p)
					return length + (p - buffer);
			while (++p < end);
			length += PAGE_SIZE / sizeof(wchar_t);
			src += PAGE_SIZE;
		}
	}
	else
	{
		*(LPBYTE)buffer = *((LPBYTE)buffer + size - 1);
		while (ReadProcessMemory(hProcess, src, (LPBYTE)buffer + 1, PAGE_SIZE, NULL))
		{
			p = buffer;
			do
				if (!*p)
					return length + (p - buffer);
			while (++p < end);
			length += PAGE_SIZE / sizeof(wchar_t);
			src += PAGE_SIZE;
			*(LPBYTE)buffer = *(LPBYTE)end;
		}
	}
	return 0;
}
