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
	if (!NT_SUCCESS(Status))
	{
		SetLastError(RtlNtStatusToDosError(Status));
		ProcessBasic.UniqueProcessId = 0;
	}
	return (DWORD)ProcessBasic.UniqueProcessId;
}
