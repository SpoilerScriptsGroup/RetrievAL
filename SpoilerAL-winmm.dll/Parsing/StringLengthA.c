#include <windows.h>
#include "PageSize.h"

#ifdef __BORLANDC__
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

EXTERN_C size_t __stdcall StringLengthA(HANDLE hProcess, LPCSTR lpString)
{
	if (lpString)
	{
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
			char   buffer[PAGE_SIZE];
			size_t size;

			size = -(ptrdiff_t)lpString & (PAGE_SIZE - 1);
			if (ReadProcessMemory(hProcess, lpString, buffer, size, NULL))
			{
				LPSTR  end, p;
				LPCSTR src;
				size_t length;

				end = buffer + size;
				p = buffer;
				do
					if (!*p)
						return p - buffer;
				while (++p < end);
				length = size;
				end = buffer + PAGE_SIZE;
				src = lpString + size;
				while (ReadProcessMemory(hProcess, src, buffer, PAGE_SIZE, NULL))
				{
					p = buffer;
					do
						if (!*p)
							return length + (p - buffer);
					while (++p < end);
					length += PAGE_SIZE;
					src += PAGE_SIZE;
				}
			}
		}
		else
		{
			return strlen(lpString);
		}
	}
	return 0;
}
