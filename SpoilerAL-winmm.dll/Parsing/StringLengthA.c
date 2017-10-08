#include <windows.h>
#include "PageSize.h"

EXTERN_C size_t __stdcall StringLengthA(HANDLE hProcess, LPCSTR lpString)
{
	char   buffer[PAGE_SIZE];
	size_t length;

	if (lpString)
	{
		size_t size;

		size = -(ptrdiff_t)lpString & (PAGE_SIZE - 1);
		if (ReadProcessMemory(hProcess, lpString, buffer, size, NULL))
		{
			LPSTR  end, p;
			LPCSTR src;

			end = buffer + size;
			p = buffer;
			do
				if (!*p)
				{
					length = p - buffer;
					goto SUCCESS;
				}
			while (++p < end);
			length = size;
			end = buffer + PAGE_SIZE;
			src = lpString + size;
			for (; ; )
			{
				if (!ReadProcessMemory(hProcess, src, buffer, PAGE_SIZE, NULL))
					break;
				p = buffer;
				do
					if (!*p)
					{
						length += p - buffer;
						goto SUCCESS;
					}
				while (++p < end);
				length += PAGE_SIZE;
				src += PAGE_SIZE;
			}
		}
	}
	length = 0;
SUCCESS:
	return length;
}
