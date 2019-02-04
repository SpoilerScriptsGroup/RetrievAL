#ifdef StringLengthT
#ifdef __BORLANDC__
#pragma warn -8017
#endif
#include "StringLength.h"
#include <tchar.h>
#include <limits.h>
#include "PageSize.h"

#ifdef __BORLANDC__
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

#ifndef _tcsnlen
#ifdef _UNICODE
#define _tcsnlen wcsnlen
static size_t wcsnlen(const wchar_t *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		while (*(string++) && --length);
		length = maxlen - length;
	}
	return length;
}
#else
#define _tcsnlen strnlen
static size_t strnlen(const char *string, size_t maxlen)
{
	const char *p;

	if (maxlen && (p = memchr(string, '\0', maxlen)))
		return p - string;
	else
		return maxlen;
}
#endif
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

size_t __stdcall StringLengthT(
	IN OPTIONAL HANDLE  hProcess,
	IN          LPCTSTR lpString)
{
	if (lpString)
	{
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
#ifdef _UNICODE
			BYTE    buffer[PAGE_SIZE + 1];
#else
			BYTE    buffer[PAGE_SIZE];
#endif
			LPCBYTE src;
			size_t  size, length;

			src = (LPCBYTE)lpString;
			if (size = -(ptrdiff_t)lpString & (PAGE_SIZE - 1))
			{
				size_t maxlen;

				if (!ReadProcessMemory(hProcess, src, buffer, size, NULL))
					goto FAILED;
				src += size;
				maxlen = size;
#ifdef _UNICODE
				if (maxlen /= sizeof(TCHAR))
#endif
					if ((length = _tcsnlen((LPCTSTR)buffer, maxlen)) < maxlen)
						return length;
			}
#ifdef _UNICODE
			if (!(size & 1))
#endif
			{
				while (ReadProcessMemory(hProcess, src, buffer, PAGE_SIZE, NULL))
				{
					#define MAXLEN (PAGE_SIZE / sizeof(TCHAR))

					if ((length = _tcsnlen((LPCTSTR)buffer, MAXLEN)) < MAXLEN)
						return ((LPCTSTR)src - lpString) + length;
					src += PAGE_SIZE;

					#undef MAXLEN
				}
			}
#ifdef _UNICODE
			else
			{
				buffer[0] = buffer[size - 1];
				while (ReadProcessMemory(hProcess, src, buffer + 1, PAGE_SIZE, NULL))
				{
					#define MAXLEN (PAGE_SIZE / sizeof(TCHAR))

					if ((length = _tcsnlen((LPCTSTR)buffer, MAXLEN)) < MAXLEN)
						return ((LPCTSTR)src - lpString) + length;
					src += PAGE_SIZE;
					buffer[0] = buffer[PAGE_SIZE];

					#undef MAXLEN
				}
			}
#endif
		}
		else
		{
			MEMORY_BASIC_INFORMATION mbi;

			if (VirtualQuery(lpString, &mbi, sizeof(mbi)) && IsReadableProtect(mbi.Protect))
			{
				LPCTSTR p;

				mbi.RegionSize += (size_t)mbi.BaseAddress - (size_t)(p = lpString);
				do
				{
					size_t maxlen, length;

					maxlen = mbi.RegionSize / sizeof(TCHAR);
					if ((length = _tcsnlen(p, maxlen)) < maxlen)
						return (p - lpString) + length;
					p += maxlen;
#ifndef _UNICODE
				} while (VirtualQuery(p, &mbi, sizeof(mbi)) && IsReadableProtect(mbi.Protect));
#else
				} while (VirtualQuery((LPVOID)(((size_t)p + 1) & (ptrdiff_t)-2), &mbi, sizeof(mbi)) && IsReadableProtect(mbi.Protect));
#endif
			}
		}
	}
FAILED:
	return SIZE_MAX;
}
#endif
