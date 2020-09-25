#include <windows.h>
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")

#ifndef _M_IX86
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
		return (DWORD)ProcessBasic.UniqueProcessId;
	SetLastError(RtlNtStatusToDosError(Status));
	return 0;
}
#else
EXTERN_C __declspec(naked) DWORD __stdcall GetProcessIdCompatible(IN HANDLE Process)
{
	__asm
	{
		#define Process (esp + 4)

		mov     eax, dword ptr [Process]
		sub     esp, size PROCESS_BASIC_INFORMATION
		mov     ecx, esp
		push    0
		push    size PROCESS_BASIC_INFORMATION
		push    ecx
		push    ProcessBasicInformation
		push    eax
		call    NtQueryInformationProcess
		test    eax, eax
		js      FAILED
		mov     eax, dword ptr [esp + PROCESS_BASIC_INFORMATION.UniqueProcessId]
		add     esp, size PROCESS_BASIC_INFORMATION
		ret     4

		align   16
	FAILED:
		push    eax
		call    RtlNtStatusToDosError
		push    eax
		call    SetLastError
		add     esp, size PROCESS_BASIC_INFORMATION
		xor     eax, eax
		ret     4

		#undef Process
	}
}
#endif
