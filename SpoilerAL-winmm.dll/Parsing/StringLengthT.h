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

#ifndef _M_IX86
static BOOL __stdcall Read(
	IN     HANDLE  hProcess,
	IN     LPCVOID lpBaseAddress,
	OUT    LPVOID  lpBuffer,
	IN     size_t  nSize,
	IN OUT size_t  *lpnCount)
{
	size_t nCount;

	nCount = *lpnCount;
	if (_sub_uintptr(nCount, nSize, &nCount))
	{
		nSize += nCount;
#ifdef _UNICODE
		if (nCount != (size_t)-1)
#endif
			*(LPTSTR)((LPBYTE)lpBuffer + nSize) = TEXT('\0');
		nCount = 0;
	}
	*lpnCount = nCount;
	return ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, NULL);
}
#else
__declspec(naked) static BOOL __stdcall Read(
	IN     HANDLE  hProcess,
	IN     LPCVOID lpBaseAddress,
	OUT    LPVOID  lpBuffer,
	IN     size_t  nSize,
	IN OUT size_t  *lpnCount)
{
	__asm
	{
		#define hProcess            (esp + 4)
		#define lpBaseAddress       (esp + 8)
		#define lpBuffer            (esp + 12)
		#define nSize               (esp + 16)
		#define lpnCount            (esp + 20)
		#define lpNumberOfBytesRead (esp + 20)

		mov     edx, dword ptr [lpnCount]
		mov     ecx, dword ptr [nSize]
		mov     eax, dword ptr [edx]
		mov     dword ptr [lpNumberOfBytesRead], 0
		sub     eax, ecx
		jb      L1
		mov     dword ptr [edx], eax
		jmp     ReadProcessMemory

		align   16
	L1:
#ifdef _UNICODE
		add     ecx, eax
		cmp     eax, -1
		mov     eax, dword ptr [lpBuffer]
		je      L2
		mov     word ptr [eax + ecx], 0
	L2:
#else
		add     ecx, eax
		mov     eax, dword ptr [lpBuffer]
		mov     byte ptr [eax + ecx], 0
#endif
		mov     dword ptr [nSize], ecx
		mov     dword ptr [edx], 0
		jmp     ReadProcessMemory

		#undef hProcess
		#undef lpBaseAddress
		#undef lpBuffer
		#undef nSize
		#undef lpnCount
		#undef lpNumberOfBytesRead
	}
}
#endif

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

#ifdef _UNICODE
		count = min(nMaxLength, (size_t)-1 / sizeof(wchar_t)) * sizeof(wchar_t);
#else
		count = nMaxLength;
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
				src += limit * sizeof(TCHAR);
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
