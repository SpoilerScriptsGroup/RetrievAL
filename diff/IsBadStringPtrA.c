#include <windows.h>

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

BOOL __stdcall IsBadStringPtrCompatibleA(
	IN LPCSTR   lpsz,
	IN UINT_PTR ucchMax)
{
	MEMORY_BASIC_INFORMATION mbi;
	size_t                   count;

	if (!VirtualQuery(lpsz, &mbi, sizeof(mbi)))
		return TRUE;
	if (!IsValidAddress(mbi.Protect))
		return TRUE;
	if (!ucchMax)
		return FALSE;
	count = (LPCSTR)mbi.BaseAddress + mbi.RegionSize - lpsz;
	for (; ; )
	{
		if (ucchMax <= count)
			return FALSE;
		ucchMax -= count;
		if (memchr(lpsz, '\0', count))
			return FALSE;
		if (!VirtualQuery(lpsz += count, &mbi, sizeof(mbi)))
			break;
		if (!IsValidAddress(mbi.Protect))
			break;
		count = mbi.RegionSize;
	}
	return TRUE;
}
