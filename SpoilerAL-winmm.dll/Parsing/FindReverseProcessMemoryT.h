#include "FindReverseProcessMemory.h"
#include "PageSize.h"
#include "intrinsic.h"
#include <tchar.h>

#ifdef _UNICODE
#define FindReverseProcessMemoryT FindReverseProcessMemoryW
extern wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count);
extern wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wchar_t c, size_t count);
#define _tmemrchr _wmemrchr
#define _tmemrichr _wmemrichr
#else
#define FindReverseProcessMemoryT FindReverseProcessMemoryA
extern void * __cdecl _memrchr(const void *buffer, int c, size_t count);
extern void * __cdecl _memrichr(const void *buffer, int c, size_t count);
#define _tmemrchr _memrchr
#define _tmemrichr _memrichr
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

size_t __stdcall FindReverseProcessMemoryT(
	IN OPTIONAL HANDLE  hProcess,
	IN          LPCTSTR lpString,
	IN          TCHAR   c,
	IN          size_t  nMaxLength,
	IN          BOOL    bIgnoreCase)
{
	size_t  size, read;
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
#ifdef _UNICODE
	if (_add_uintptr(nMaxLength, nMaxLength, &size))
		goto READ_FAILED;
#else
	size = nMaxLength;
#endif
	if (_add_uintptr((size_t)lpString, size, (size_t *)&src))
		goto READ_FAILED;
	lpFindMethod = !bIgnoreCase ? _tmemrchr : _tmemrichr;
	if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
	{
		__declspec(align(16)) BYTE buffer[PAGE_SIZE];

		read = (size_t)src & (PAGE_SIZE - sizeof(TCHAR));
		src = (LPCBYTE)(((size_t)src - sizeof(TCHAR)) & -(ptrdiff_t)(PAGE_SIZE - sizeof(TCHAR) + 1));
		if (read)
			if (!ReadProcessMemory(hProcess, max((LPCTSTR)src, lpString), buffer, read = min(read, size), NULL))
				goto READ_FAILED;
			else if (p = (LPBYTE)lpFindMethod((LPCTSTR)buffer, c, read / sizeof(TCHAR)))
				return (LPCTSTR)p - (LPCTSTR)buffer;
			else if (!(size -= read))
				goto NOT_FOUND;
		for (read = min(PAGE_SIZE, size); ReadProcessMemory(hProcess, src -= read, buffer, read, NULL); read = min(PAGE_SIZE, size))
			if (p = (LPBYTE)lpFindMethod((LPCTSTR)buffer, c, read / sizeof(TCHAR)))
				return (LPCTSTR)(src + (p - buffer)) - lpString;
			else if (!(size -= read))
				goto NOT_FOUND;
	}
	else
	{
		MEMORY_BASIC_INFORMATION mbi;

		read = (size_t)src & (PAGE_SIZE - sizeof(TCHAR));
		src = (LPCBYTE)(((size_t)src - sizeof(TCHAR)) & -(ptrdiff_t)(PAGE_SIZE - sizeof(TCHAR) + 1));
		if (!VirtualQuery(src, &mbi, sizeof(mbi)) || !IsReadableProtect(mbi.Protect))
			goto READ_FAILED;
		if (read)
			if (p = (LPBYTE)lpFindMethod(max((LPCTSTR)src, lpString), c, (read = min(read, size)) / sizeof(TCHAR)))
				return (LPCTSTR)p - lpString;
			else if (!(size -= read))
				goto NOT_FOUND;
		do
			if ((PVOID)(src -= read = min(PAGE_SIZE, size)) < mbi.BaseAddress && (!VirtualQuery(src, &mbi, sizeof(mbi)) || !IsReadableProtect(mbi.Protect)))
				break;
			else if (p = (LPBYTE)lpFindMethod((LPCTSTR)src, c, read / sizeof(TCHAR)))
				return (LPCTSTR)p - lpString;
		while (size -= read);
	}

READ_FAILED:
	return SIZE_MAX;

NOT_FOUND:
	return nMaxLength;
}
