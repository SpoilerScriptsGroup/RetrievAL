#include <windows.h>
#include "GetPageSize.h"

extern HANDLE hHeap;

EXTERN_C size_t __stdcall StringLengthW(HANDLE hProcess, LPCWSTR lpString)
{
	size_t length;
	DWORD  pageSize;
	LPWSTR buffer;
	DWORD  size;

	if (!lpString)
		goto FAILED;
	pageSize = GetPageSize();
	if (!pageSize)
		goto FAILED;
	buffer = (LPWSTR)HeapAlloc(hHeap, 0, pageSize + ((DWORD)lpString & 1));
	if (!buffer)
		goto FAILED;
	size = pageSize - (DWORD)((size_t)lpString % pageSize);
	if (ReadProcessMemory(hProcess, lpString, buffer, size, NULL))
	{
		LPWSTR  end, p;
		LPCBYTE src;

		length = size / sizeof(wchar_t);
		if (length)
		{
			end = (LPWSTR)((LPBYTE)buffer + (size & ~1UL));
			p = buffer;
			do
			{
				if (!*p)
				{
					length = p - buffer;
					goto SUCCESS;
				}
			} while (++p < end);
		}
		end = (LPWSTR)((LPBYTE)buffer + pageSize);
		src = (LPCBYTE)lpString + size;
		if (!(size & 1))
		{
			for (; ; )
			{
				if (!ReadProcessMemory(hProcess, src, buffer, pageSize, NULL))
					break;
				p = buffer;
				do
				{
					if (!*p)
					{
						length += p - buffer;
						goto SUCCESS;
					}
				} while (++p < end);
				length += pageSize / sizeof(wchar_t);
				src += pageSize;
			}
		}
		else
		{
			*(LPBYTE)buffer = *((LPBYTE)buffer + size - 1);
			for (; ; )
			{
				if (!ReadProcessMemory(hProcess, src, (LPBYTE)buffer + 1, pageSize, NULL))
					break;
				p = buffer;
				do
				{
					if (!*p)
					{
						length += p - buffer;
						goto SUCCESS;
					}
				} while (++p < end);
				length += pageSize / sizeof(wchar_t);
				src += pageSize;
				*(LPBYTE)buffer = *(LPBYTE)end;
			}
		}
	}
	length = 0;
SUCCESS:
	HeapFree(hHeap, 0, buffer);
	return length;
FAILED:
	return 0;
}
