#include <windows.h>

static DWORD dwOSMajorVersion = 0;

DWORD __stdcall GetOSMajorVersion()
{
	OSVERSIONINFOW OSVersionInfo;

	if (dwOSMajorVersion)
		return dwOSMajorVersion;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVersionInfo);
#pragma warning(push)
#pragma warning(disable:4996)
	if (!GetVersionExW(&OSVersionInfo))
#pragma warning(pop)
		return 0;
	return dwOSMajorVersion = OSVersionInfo.dwMajorVersion;
}
