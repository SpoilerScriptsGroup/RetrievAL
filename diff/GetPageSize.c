#include <windows.h>

DWORD __stdcall GetPageSize()
{
	static DWORD dwPageSize = 0;

	if (!dwPageSize)
	{
		SYSTEM_INFO SystemInfo;

		GetSystemInfo(&SystemInfo);
		dwPageSize = SystemInfo.dwPageSize;
	}
	return dwPageSize;
}
