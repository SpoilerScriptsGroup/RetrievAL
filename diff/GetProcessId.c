#include <windows.h>

EXTERN_C DWORD __stdcall GetProcessId(IN HANDLE Process)
{
	#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
	#define ProcessBasicInformation 0

	typedef /*__success(return >= 0) */LONG NTSTATUS;

	typedef int PROCESSINFOCLASS;

	typedef struct _PROCESS_BASIC_INFORMATION {
		ULONG_PTR ExitStatus;
		PVOID     PebBaseAddress;
		ULONG_PTR AffinityMask;
		ULONG_PTR BasePriority;
		ULONG_PTR UniqueProcessId;
		ULONG_PTR InheritedFromUniqueProcessId;
	} PROCESS_BASIC_INFORMATION;

	typedef NTSTATUS(WINAPI *NTQUERYINFORMATIONPROCESS)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

	NTQUERYINFORMATIONPROCESS NtQueryInformationProcess;

	NtQueryInformationProcess = (NTQUERYINFORMATIONPROCESS)
		GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	if (NtQueryInformationProcess)
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
		}
	}
	else
	{
		SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	}
	return 0;
}
