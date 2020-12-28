#include <windows.h>
#include <assert.h>
#include "page.h"

#ifdef __BORLANDC__
EXTERN_C DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif
EXTERN_C BOOL __stdcall IsBadStringPtrCompatibleW(IN LPCWSTR lpsz, IN UINT_PTR ucchMax);

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

BOOL __stdcall IsBadStringPtrExW(
	IN HANDLE   hProcess,
	IN LPCWSTR  lpsz,
	IN UINT_PTR ucchMax)
{
	__declspec(align(16)) wchar_t buffer[PAGE_SIZE / sizeof(wchar_t)];
	MEMORY_BASIC_INFORMATION      mbi;
	size_t                        count;

	assert(PAGE_SIZE % sizeof(wchar_t) == 0);

	if (GetProcessId(hProcess) == GetCurrentProcessId())
		return IsBadStringPtrCompatibleW(lpsz, ucchMax);
	if (!ucchMax)
		return FALSE;
	if (count = -(ptrdiff_t)lpsz & (PAGE_SIZE - sizeof(wchar_t)))
	{
		count /= sizeof(wchar_t);
		if (ucchMax <= count)
			goto DONE;
		ucchMax -= count;
		if (!ReadProcessMemory(hProcess, lpsz, buffer, count * sizeof(wchar_t), NULL))
			return TRUE;
#ifndef __BORLANDC__
		if (wcsnlen(buffer, count) < count)
			return FALSE;
		lpsz += count;
#else
		do
			if (!*(lpsz++))
				return FALSE;
		while (--count);
#endif
	}
	while (ucchMax > PAGE_SIZE / sizeof(wchar_t))
	{
		ucchMax -= PAGE_SIZE / sizeof(wchar_t);
		if (!ReadProcessMemory(hProcess, lpsz, buffer, PAGE_SIZE, NULL))
			return TRUE;
#ifndef __BORLANDC__
		if (wcsnlen(buffer, PAGE_SIZE / sizeof(wchar_t)) < PAGE_SIZE / sizeof(wchar_t))
			return FALSE;
		(LPBYTE)lpsz += PAGE_SIZE;
#else
		count = PAGE_SIZE / sizeof(wchar_t);
		do
			if (!*(lpsz++))
				return FALSE;
		while (--count);
#endif
	}
DONE:
	return !VirtualQueryEx(hProcess, lpsz, &mbi, sizeof(mbi)) || !IsReadableProtect(mbi.Protect);
}
