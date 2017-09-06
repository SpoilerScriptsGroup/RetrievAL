#ifdef IsBadPtr
#include <windows.h>

#ifdef __BORLANDC__
#pragma warn -8058
#endif

BOOL __stdcall IsBadPtr(
	IN LPVOID   lp,
	IN UINT_PTR ucb)
{
	LPVOID end;

	end = (LPBYTE)lp + ucb;
	do
	{
		MEMORY_BASIC_INFORMATION mbi;

		if (!VirtualQuery(lp, &mbi, sizeof(mbi)))
			return TRUE;
		if (!IsValidAddress(mbi.Protect))
			return TRUE;
		lp = (LPBYTE)mbi.BaseAddress + mbi.RegionSize;
	} while (lp < end);
	return FALSE;
}
#endif
