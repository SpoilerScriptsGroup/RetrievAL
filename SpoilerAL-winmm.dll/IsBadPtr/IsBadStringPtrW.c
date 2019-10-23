#include <windows.h>
#include <assert.h>
#include "PageSize.h"

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

BOOL __stdcall IsBadStringPtrCompatibleW(
	IN LPCWSTR  lpsz,
	IN UINT_PTR ucchMax)
{
	MEMORY_BASIC_INFORMATION mbi;
	size_t                   count;

	assert(PAGE_SIZE % sizeof(wchar_t) == 0);

	if (!ucchMax)
		return FALSE;
	if (!VirtualQuery(lpsz, &mbi, sizeof(mbi)))
		return TRUE;
	if (!IsReadableProtect(mbi.Protect))
		return TRUE;
	count = ((size_t)mbi.BaseAddress + mbi.RegionSize - (size_t)lpsz) / sizeof(wchar_t);
	for (; ; )
	{
		if (ucchMax <= count)
			return FALSE;
		ucchMax -= count;
#ifndef __BORLANDC__
		if (wcsnlen(lpsz, count) < count)
			return FALSE;
		lpsz += count;
#else
		while (count--)
			if (!*(lpsz++))
				return FALSE;
#endif
		if (!VirtualQuery((LPCVOID)(((size_t)lpsz + 1) & -2), &mbi, sizeof(mbi)))
			break;
		if (!IsReadableProtect(mbi.Protect))
			break;
		count = mbi.RegionSize / sizeof(wchar_t);
	}
	return TRUE;
}
