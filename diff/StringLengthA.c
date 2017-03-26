#include <windows.h>

extern HANDLE hHeap;

EXTERN_C size_t __stdcall StringLengthA(HANDLE hProcess, LPCSTR lpString)
{
	size_t      length;
	SYSTEM_INFO systemInfo;
	DWORD       pageSize;
	LPSTR       buffer;
	DWORD       size;

	if (!lpString)
		goto FAILED;
	GetSystemInfo(&systemInfo);
	pageSize = systemInfo.dwPageSize;
	if (!pageSize)
		goto FAILED;
	buffer = (LPSTR)HeapAlloc(hHeap, 0, pageSize);
	if (!buffer)
		goto FAILED;
	size = pageSize - (DWORD)((size_t)lpString % pageSize);
	if (ReadProcessMemory(hProcess, lpString, buffer, size, NULL))
	{
		LPSTR  end, p;
		LPCSTR src;

		end = buffer + size;
		p = buffer;
		do
		{
			if (!*p)
			{
				length = p - buffer;
				goto SUCCESS;
			}
		} while (++p < end);
		length = size;
		end = buffer + pageSize;
		src = lpString + size;
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
			length += pageSize;
			src += pageSize;
		}
	}
	length = 0;
SUCCESS:
	HeapFree(hHeap, 0, buffer);
	return length;
FAILED:
	return 0;
}
