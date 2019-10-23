#ifdef IsBadPtr
#include <windows.h>

#ifdef __BORLANDC__
#pragma warn -8058
#endif

BOOL __stdcall IsBadPtr(
#ifndef VirtualQueryEx
	IN HANDLE   hProcess,
#endif
	IN LPVOID   lp,
	IN UINT_PTR ucb)
{
	if (ucb)
	{
		LPVOID end;

		end = (LPBYTE)lp + ucb;
		do
		{
			MEMORY_BASIC_INFORMATION mbi;

			if (!VirtualQueryEx(hProcess, lp, &mbi, sizeof(mbi)))
				return TRUE;
			if (!IsValidProtect(mbi.Protect))
				return TRUE;
			lp = (LPBYTE)mbi.BaseAddress + mbi.RegionSize;
		} while (lp < end);
	}
	return FALSE;
}
#endif
