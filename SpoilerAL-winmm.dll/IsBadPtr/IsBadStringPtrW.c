#include <windows.h>
#include <assert.h>
#include "PageSize.h"

#define IsValidAddress(Protect) (   \
    ((Protect) & (                  \
        PAGE_READONLY          |    \
        PAGE_READWRITE         |    \
        PAGE_WRITECOPY         |    \
        PAGE_EXECUTE_READ      |    \
        PAGE_EXECUTE_READWRITE |    \
        PAGE_EXECUTE_WRITECOPY)) && \
    !((Protect) & (                 \
        PAGE_NOACCESS          |    \
        PAGE_GUARD)))

BOOL __stdcall IsBadStringPtrCompatibleW(
	IN LPCWSTR lpsz,
	IN UINT    ucchMax)
{
	MEMORY_BASIC_INFORMATION mbi;
	size_t                   count;

	assert(PAGE_SIZE % sizeof(wchar_t) == 0);
	if (!VirtualQuery(lpsz, &mbi, sizeof(mbi)))
		return TRUE;
	if (!IsValidAddress(mbi.Protect))
		return TRUE;
	if (!ucchMax)
		return FALSE;
	count = ((size_t)mbi.BaseAddress + mbi.RegionSize - (size_t)lpsz) / sizeof(wchar_t);
	for (; ; )
	{
		if (ucchMax <= count)
			return FALSE;
		ucchMax -= count;
		while (count--)
			if (!*(lpsz++))
				return FALSE;
		if (!VirtualQuery((LPCVOID)(((size_t)lpsz + sizeof(wchar_t) - 1) & -(ptrdiff_t)sizeof(wchar_t)), &mbi, sizeof(mbi)))
			break;
		if (!IsValidAddress(mbi.Protect))
			break;
		count = mbi.RegionSize / sizeof(wchar_t);
	}
	return TRUE;
}
