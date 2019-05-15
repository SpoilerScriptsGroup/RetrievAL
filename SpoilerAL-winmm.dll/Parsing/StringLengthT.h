#ifdef StringLengthT
#ifdef __BORLANDC__
#pragma warn -8017
#endif
#include "StringLength.h"
#include <stdint.h>
#include <tchar.h>
#include <limits.h>
#include "PageSize.h"
#include "intrinsic.h"

#ifndef _WIN64
#define _sub_uintptr _sub_u32
#else
#define _sub_uintptr _sub_u64
#endif

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

static BOOL __stdcall Read(
	IN     HANDLE  hProcess,
	IN     LPCVOID lpBaseAddress,
	OUT    LPVOID  lpBuffer,
	IN     size_t  nSize,
	IN OUT size_t  *lpnCount)
{
	BOOL   bSuccess;
	size_t nCount;

	nCount = *lpnCount;
	if (bSuccess = ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, min(nSize, nCount), NULL))
	{
		if (_sub_uintptr(nCount, nSize, &nCount))
		{
#ifdef _UNICODE
			if (nCount < (size_t)-1)
#endif
				*(LPTSTR)((LPBYTE)lpBuffer + nCount + nSize) = TEXT('\0');
			nCount = 0;
		}
		*lpnCount = nCount;
	}
	return bSuccess;
}

size_t __stdcall StringLengthT(
	IN OPTIONAL HANDLE  hProcess,
	IN          LPCTSTR lpString,
	IN          size_t  nMaxLength)
{
	LPCBYTE src;
	size_t  count, length;

	if (!lpString)
		goto FAILED;
	if (!nMaxLength)
		goto RETURN_MAXLEN;
	src = (LPCBYTE)lpString;
	if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
	{
#ifdef _UNICODE
		BYTE    buffer[PAGE_SIZE + 1];
#else
		BYTE    buffer[PAGE_SIZE];
#endif
		size_t  size;

		count = nMaxLength;
#ifdef _UNICODE
		if (count <= ((size_t)-1 >> 1))
			count <<= 1;
		else
			count = -1;
#endif
		if (size = -(ptrdiff_t)lpString & (PAGE_SIZE - 1))
		{
			size_t limit;

			if (!Read(hProcess, src, buffer, size, &count))
				goto FAILED;
			limit = size;
#ifdef _UNICODE
			if (limit /= sizeof(TCHAR))
#endif
				if ((length = _tcsnlen((LPCTSTR)buffer, limit)) < limit)
					goto RETURN_LENGTH;
				else if (!count)
					goto RETURN_MAXLEN;
			src += size;
		}

		#define PAGE_LENGTH (PAGE_SIZE / sizeof(TCHAR))

#ifdef _UNICODE
		if (!(size & 1))
		{
#endif
			while (Read(hProcess, src, buffer, PAGE_SIZE, &count))
			{
				if ((length = _tcsnlen((LPCTSTR)buffer, PAGE_LENGTH)) < PAGE_LENGTH)
					goto RETURN_LENGTH;
				if (!count)
					goto RETURN_MAXLEN;
				src += PAGE_SIZE;
			}
#ifdef _UNICODE
		}
		else
		{
			buffer[0] = buffer[size - 1];
			while (Read(hProcess, src, buffer + 1, PAGE_SIZE, &count))
			{
				if ((length = _tcsnlen((LPCTSTR)buffer, PAGE_LENGTH)) < PAGE_LENGTH)
					goto RETURN_LENGTH;
				if (!count)
					goto RETURN_MAXLEN;
				src += PAGE_SIZE;
				buffer[0] = buffer[PAGE_SIZE];
			}
		}
#endif

		#undef PAGE_LENGTH
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

			count = nMaxLength;
			mbi.RegionSize += (size_t)mbi.BaseAddress - (size_t)src;
			do
			{
				size_t limit;

				limit = mbi.RegionSize / sizeof(TCHAR);
				if (limit > count)
					limit = count;
				if ((length = _tcsnlen((LPCTSTR)src, limit)) < limit)
					goto RETURN_LENGTH;
				if (!(count -= limit))
					goto RETURN_MAXLEN;
				src += mbi.RegionSize;
			} while (VirtualQuery(QueryAddress, &mbi, sizeof(mbi)) && IsReadableProtect(mbi.Protect));

			#undef QueryAddress
		}
	}

FAILED:
	return SIZE_MAX;

RETURN_LENGTH:
	return ((LPCTSTR)src - lpString) + length;

RETURN_MAXLEN:
	return nMaxLength;
}
#endif
