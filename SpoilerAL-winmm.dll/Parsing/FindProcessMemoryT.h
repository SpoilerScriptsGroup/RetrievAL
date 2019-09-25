#ifndef _DEBUG
#include "noinline_wchar.h"
#endif
#include "FindProcessMemory.h"
#include "PageSize.h"
#include <tchar.h>

#ifdef _UNICODE
#define FindProcessMemoryT FindProcessMemoryW
extern wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wchar_t c, size_t count);
#define _tmemchr wmemchr
#define _tmemichr _wmemichr
#else
#define FindProcessMemoryT FindProcessMemoryA
#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma function(memchr)
#endif
extern void * __cdecl _memichr(const void *buffer, int c, size_t count);
#define _tmemchr memchr
#define _tmemichr _memichr
#endif

#ifdef __BORLANDC__
#pragma warn -8017
#pragma warn -8058
#pragma warn -8060
#pragma warn -8075
#undef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
EXTERN_C DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

#define IsReadableProtect(Protect) ( \
    ((Protect) & (                   \
        PAGE_READONLY          |     \
        PAGE_READWRITE         |     \
        PAGE_WRITECOPY         |     \
        PAGE_EXECUTE_READ      |     \
        PAGE_EXECUTE_READWRITE |     \
        PAGE_EXECUTE_WRITECOPY)) &&  \
    !((Protect) & (                  \
        PAGE_NOACCESS          |     \
        PAGE_GUARD)))

size_t __stdcall FindProcessMemoryT(
	IN OPTIONAL HANDLE  hProcess,
	IN          LPCTSTR lpString,
	IN          TCHAR   c,
	IN          size_t  nMaxLength,
	IN          BOOL    bIgnoreCase)
{
	LPCBYTE src;
	LPBYTE  p;
#ifdef _UNICODE
	wchar_t *(__cdecl *lpFindMethod)(const void *buffer, wchar_t c, size_t count);
#else
	void *(__cdecl *lpFindMethod)(const void *buffer, int c, size_t count);
#endif

	if (!lpString)
		goto READ_FAILED;
	if (!nMaxLength)
		goto NOT_FOUND;
	lpFindMethod = !bIgnoreCase ? _tmemchr : _tmemichr;
	src = (LPCBYTE)lpString;
	if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
	{
		#define SIZE_OF_BUFFER (PAGE_SIZE + sizeof(TCHAR) - 1)

		__declspec(align(16)) BYTE buffer[SIZE_OF_BUFFER];
		size_t                     size, read;
#ifdef _UNICODE
		size_t                     remainder;
#else
		#define                    remainder 0
#endif

#ifdef _UNICODE
		size = min(nMaxLength, SIZE_MAX / sizeof(wchar_t)) * sizeof(wchar_t);
#else
		size = nMaxLength;
#endif
		if (read = -(ptrdiff_t)lpString & (PAGE_SIZE - 1))
		{
			size_t limit;

			if (!ReadProcessMemory(hProcess, src, buffer, read = min(read, size), NULL))
				goto READ_FAILED;
			limit = read;
#ifdef _UNICODE
			if (limit /= sizeof(TCHAR))
#endif
				if (p = (LPBYTE)lpFindMethod((LPCTSTR)buffer, c, limit))
					return (LPCTSTR)p - (LPCTSTR)buffer;
			if (!(size -= read))
				goto NOT_FOUND;
			src += read;
		}
#ifdef _UNICODE
		if (remainder = read & 1)
			buffer[0] = buffer[read - 1];
#endif
		while (ReadProcessMemory(hProcess, src, buffer + remainder, read = min(PAGE_SIZE, size), NULL))
		{
			if (p = (LPBYTE)lpFindMethod((LPCTSTR)buffer, c, (read + sizeof(TCHAR) - 1) / sizeof(TCHAR)))
				return (LPCTSTR)((src - remainder) + (p - buffer)) - lpString;
			if (!(size -= read))
				goto NOT_FOUND;
			src += PAGE_SIZE;
			if (remainder)
				buffer[0] = buffer[SIZE_OF_BUFFER - 1];
		}

#ifndef _UNICODE
		#undef remainder
#endif
		#undef SIZE_OF_BUFFER
	}
	else
	{
		MEMORY_BASIC_INFORMATION mbi;

		if (VirtualQuery(src, &mbi, sizeof(mbi)) && IsReadableProtect(mbi.Protect))
		{
#ifndef _UNICODE
			#define QueryAddress src
#else
			#define QueryAddress (LPVOID)(((size_t)src + 1) & (ptrdiff_t)-2)
#endif

			size_t count;

			count = nMaxLength;
			mbi.RegionSize += (size_t)mbi.BaseAddress - (size_t)src;
			do
			{
				size_t limit;

				limit = mbi.RegionSize / sizeof(TCHAR);
				if (limit > count)
					limit = count;
				if (p = (LPBYTE)lpFindMethod((LPCTSTR)src, c, limit))
					return (LPCTSTR)p - lpString;
				if (!(count -= limit))
					goto NOT_FOUND;
				src += limit * sizeof(TCHAR);
			} while (VirtualQuery(QueryAddress, &mbi, sizeof(mbi)) && IsReadableProtect(mbi.Protect));

			#undef QueryAddress
		}
	}

READ_FAILED:
	return SIZE_MAX;

NOT_FOUND:
	return nMaxLength;
}
