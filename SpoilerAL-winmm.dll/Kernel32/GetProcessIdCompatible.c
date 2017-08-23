#include <windows.h>
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")

EXTERN_C DWORD __stdcall GetProcessIdCompatible(IN HANDLE Process)
{
	PROCESS_BASIC_INFORMATION ProcessBasic;
	NTSTATUS                  Status;

	Status = NtQueryInformationProcess(
		Process,
		ProcessBasicInformation,
		&ProcessBasic,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL);
	if (NT_SUCCESS(Status))
	{
		return (DWORD)ProcessBasic.UniqueProcessId;
	}
	else
	{
		DWORD dwErrCode;

#if 0
		// RtlNtStatusToDosError function
		//
		//   Minimum supported client: Windows XP [desktop apps only]
		//   Minimum supported server: Windows Server 2003 [desktop apps only]
		//
		dwErrCode = RtlNtStatusToDosError(Status);
#else
		dwErrCode = ERROR_ACCESS_DENIED;
#endif
		SetLastError(dwErrCode);
		return 0;
	}
}
