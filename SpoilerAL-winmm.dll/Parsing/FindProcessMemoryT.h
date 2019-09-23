#ifndef _DEBUG
#include "noinline_wchar.h"
#endif
#include "FindProcessMemory.h"
#include "PageSize.h"
#include <tchar.h>

#ifdef _UNICODE
#define FindProcessMemoryT FindProcessMemoryW
#define _tmemchr wmemchr
#else
#define FindProcessMemoryT FindProcessMemoryA
#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma function(memchr)
#endif
#define _tmemchr memchr
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
	IN          size_t  nMaxLength)
{
	LPCBYTE src;
	LPBYTE  p;

	if (!lpString)
		goto READ_FAILED;
	if (!nMaxLength)
		goto NOT_FOUND;
	src = (LPCBYTE)lpString;
	if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
	{
		__declspec(align(16)) BYTE buffer[PAGE_SIZE + sizeof(TCHAR) - 1];
		size_t                     size, read;
#ifdef _UNICODE
		size_t                     remainder;
#else
		#define remainder 0
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
				if (p = (LPBYTE)_tmemchr((LPCTSTR)buffer, c, limit))
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
			if (p = (LPBYTE)_tmemchr((LPCTSTR)buffer, c, (read + sizeof(TCHAR) - 1) / sizeof(TCHAR)))
				return (LPCTSTR)(src + (p - buffer)) - lpString;
			if (!(size -= read))
				goto NOT_FOUND;
			src += PAGE_SIZE;
			if (remainder)
				buffer[0] = buffer[sizeof(buffer) - 1];
		}

#ifndef _UNICODE
		#undef remainder
#endif
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
				if (p = (LPBYTE)_tmemchr((LPCTSTR)src, c, limit))
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
