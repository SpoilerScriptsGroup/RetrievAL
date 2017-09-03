#include "AllocERW.h"

extern DWORD __stdcall GetPageSize();

LPVOID __stdcall ReAllocERW(LPVOID lpAddress, size_t nSize)
{
	do	/* do { ... } while (0); */
	{
		DWORD                    dwPageSize;
		MEMORY_BASIC_INFORMATION mbi;

		if (!nSize)
			break;
		dwPageSize = GetPageSize();
		nSize = (nSize + dwPageSize - 1) & -(ptrdiff_t)dwPageSize;
		if (!VirtualQuery(lpAddress, &mbi, sizeof(mbi)))
			break;
		if (nSize > mbi.RegionSize)
		{
			if (!VirtualAlloc(lpAddress, nSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE))
			{
				LPVOID lpNewAddress;

				if (!(lpNewAddress = VirtualAlloc(NULL, nSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE)))
					break;
				memcpy(lpNewAddress, lpAddress, mbi.RegionSize);
				VirtualFree(lpAddress, 0, MEM_RELEASE);
				lpAddress = lpNewAddress;
			}
		}
		else if (nSize < mbi.RegionSize)
			VirtualFree((LPBYTE)lpAddress + nSize, mbi.RegionSize - nSize, MEM_DECOMMIT);
		return lpAddress;
	} while (0);
	return NULL;
}
