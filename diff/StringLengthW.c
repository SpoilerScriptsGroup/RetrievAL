#include <windows.h>
#include "PageSize.h"

EXTERN_C size_t __stdcall StringLengthW(HANDLE hProcess, LPCWSTR lpString)
{
	wchar_t buffer[PAGE_SIZE / sizeof(wchar_t) + 1];
	size_t  length;

	if (lpString)
	{
		size_t size;

		size = (size_t)lpString & -(ptrdiff_t)PAGE_SIZE;
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
			end = (LPWSTR)((LPBYTE)buffer + PAGE_SIZE);
			src = (LPCBYTE)lpString + size;
			if (!(size & 1))
			{
				for (; ; )
				{
					if (!ReadProcessMemory(hProcess, src, buffer, PAGE_SIZE, NULL))
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
					length += PAGE_SIZE / sizeof(wchar_t);
					src += PAGE_SIZE;
				}
			}
			else
			{
				*(LPBYTE)buffer = *((LPBYTE)buffer + size - 1);
				for (; ; )
				{
					if (!ReadProcessMemory(hProcess, src, (LPBYTE)buffer + 1, PAGE_SIZE, NULL))
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
					length += PAGE_SIZE / sizeof(wchar_t);
					src += PAGE_SIZE;
					*(LPBYTE)buffer = *(LPBYTE)end;
				}
			}
		}
	}
	length = 0;
SUCCESS:
	return length;
}
