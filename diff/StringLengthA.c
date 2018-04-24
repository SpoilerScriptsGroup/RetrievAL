#include <windows.h>
#include "PageSize.h"

#ifdef __BORLANDC__
#pragma warn -8060
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

EXTERN_C size_t __stdcall StringLengthA(HANDLE hProcess, LPCSTR lpString)
{
	char   buffer[PAGE_SIZE];
	size_t size;
	LPSTR  end, p;
	LPCSTR src;

	if (!lpString)
		return 0;
	if (!hProcess || GetProcessId(hProcess) == GetCurrentProcessId())
		return strlen(lpString);
	if (size = -(ptrdiff_t)lpString & (PAGE_SIZE - 1))
	{
		if (!ReadProcessMemory(hProcess, lpString, buffer, size, NULL))
			return 0;
		end = buffer + size;
		p = buffer;
		do
			if (!*p)
				return p - buffer;
		while (++p < end);
	}
	end = buffer + PAGE_SIZE;
	src = lpString + size;
	while (ReadProcessMemory(hProcess, src, buffer, PAGE_SIZE, NULL))
	{
		p = buffer;
		do
			if (!*p)
				return size + (p - buffer);
		while (++p < end);
		size += PAGE_SIZE;
		src += PAGE_SIZE;
	}
	return 0;
}
