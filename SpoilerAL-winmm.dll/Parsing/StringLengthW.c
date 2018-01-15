#include <windows.h>
#include "PageSize.h"

#ifdef __BORLANDC__
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

EXTERN_C size_t __stdcall StringLengthW(HANDLE hProcess, LPCWSTR lpString)
{
	if (lpString)
	{
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
			wchar_t buffer[PAGE_SIZE / sizeof(wchar_t) + 1];
			size_t  size;

			size = -(ptrdiff_t)lpString & (PAGE_SIZE - 1);
			if (ReadProcessMemory(hProcess, lpString, buffer, size, NULL))
			{
				size_t  length;
				LPWSTR  end, p;
				LPCBYTE src;

				length = size / sizeof(wchar_t);
				if (length)
				{
					end = (LPWSTR)((LPBYTE)buffer + (size & ~(size_t)1));
					p = buffer;
					do
						if (!*p)
							return p - buffer;
					while (++p < end);
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
			}
		}
		else
		{
			return wcslen(lpString);
		}
	}
	return 0;
}
