#include <windows.h>

BOOL __cdecl IsSingleInstance()
{
	#define lpMutexName (LPCSTR)0x006020C4

	SECURITY_DESCRIPTOR sd;

	if (InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
	{
		if (SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE))
		{
			SECURITY_ATTRIBUTES sa;
			HANDLE              hMutex;

			sa.nLength = sizeof(sa);
			sa.lpSecurityDescriptor = &sd;
			sa.bInheritHandle = TRUE;
			if (hMutex = CreateMutexA(&sa, FALSE, lpMutexName))
			{
				if (GetLastError() != ERROR_ALREADY_EXISTS)
					return TRUE;
				CloseHandle(hMutex);
			}
		}
	}
	return FALSE;

	#undef lpMutexName
}
