#include <windows.h>
#include "PageSize.h"

#ifdef __BORLANDC__
EXTERN_C DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif
EXTERN_C BOOL __stdcall IsBadStringPtrCompatibleA(IN LPCSTR lpsz, IN UINT_PTR ucchMax);

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

BOOL __stdcall IsBadStringPtrExA(
	IN HANDLE   hProcess,
	IN LPCSTR   lpsz,
	IN UINT_PTR ucchMax)
{
	__declspec(align(16)) char buffer[PAGE_SIZE];
	MEMORY_BASIC_INFORMATION   mbi;
	size_t                     count;

	if (GetProcessId(hProcess) == GetCurrentProcessId())
		return IsBadStringPtrCompatibleA(lpsz, ucchMax);
	if (!ucchMax)
		return FALSE;
	if (count = -(ptrdiff_t)lpsz & (PAGE_SIZE - sizeof(char)))
	{
		if (ucchMax <= count)
			goto DONE;
		ucchMax -= count;
		if (!ReadProcessMemory(hProcess, lpsz, buffer, count, NULL))
			return TRUE;
#ifndef __BORLANDC__
		if (strnlen(buffer, count) < count)
#else
		if (memchr(buffer, '\0', count))
#endif
			return FALSE;
		lpsz += count;
	}
	while (ucchMax > PAGE_SIZE)
	{
		ucchMax -= PAGE_SIZE;
		if (!ReadProcessMemory(hProcess, lpsz, buffer, PAGE_SIZE, NULL))
			return TRUE;
#ifndef __BORLANDC__
		if (strnlen(buffer, PAGE_SIZE) < PAGE_SIZE)
#else
		if (memchr(buffer, '\0', PAGE_SIZE))
#endif
			return FALSE;
		lpsz += PAGE_SIZE;
	}
DONE:
	return !VirtualQueryEx(hProcess, lpsz, &mbi, sizeof(mbi)) || !IsReadableProtect(mbi.Protect);
}
