#include <windows.h>
#include "tlhelp32fix.h"
#define PSAPI_VERSION 1
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#include "intrinsic.h"

#ifdef _MSC_VER
#include "bcb6_std_vector.h"
EXTERN_C void __stdcall VectorPushModuleEntry32A(bcb6_std_vector *vector, LPMODULEENTRY32A element);
#endif

#if defined(__BORLANDC__)
void __stdcall AddEntryModule(vector<MODULEENTRY32A> *moduleList, DWORD th32ProcessID)
#else
void __stdcall AddEntryModule(bcb6_std_vector *moduleList, DWORD th32ProcessID)
#endif
{
#if 0
#if 0
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

#if !defined(__BORLANDC__)
	NTSTATUS WINAPI NtQueryInformationProcess(
		_In_      HANDLE           ProcessHandle,
		_In_      PROCESSINFOCLASS ProcessInformationClass,
		_Out_     PVOID            ProcessInformation,
		_In_      ULONG            ProcessInformationLength,
		_Out_opt_ PULONG           ReturnLength
		);
#else
	typedef NTSTATUS(WINAPI *PFN_NTQUERYINFORMATIONPROCESS)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

	PFN_NTQUERYINFORMATIONPROCESS NtQueryInformationProcess;

	NtQueryInformationProcess = (PFN_NTQUERYINFORMATIONPROCESS)
		GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	if (NtQueryInformationProcess == NULL)
		return;
#endif

	LPVOID ImageBaseAddress;
	HANDLE hProcess;

	ImageBaseAddress = NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, th32ProcessID);
	if (hProcess != NULL)
	{
		PROCESS_BASIC_INFORMATION pbi;

		if (NT_SUCCESS(NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL)))
		{
			#define offsetof_PEB32_ImageBaseAddress 0x08

			LPVOID lpAddress;

			lpAddress = (LPVOID)((LPBYTE)pbi.PebBaseAddress + offsetof_PEB32_ImageBaseAddress);
			if (!ReadProcessMemory(hProcess, lpAddress, &ImageBaseAddress, sizeof(ImageBaseAddress), NULL))
				ImageBaseAddress = NULL;

			#undef offsetof_PEB32_ImageBaseAddress
		}
		CloseHandle(hProcess);
	}

	#undef NT_SUCCESS
	#undef ProcessBasicInformation

#else
	LPVOID ImageBaseAddress;
	HANDLE hProcess;

	ImageBaseAddress = NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, th32ProcessID);
	if (hProcess != NULL)
	{
		MODULEINFO modinfo;

		// GetModuleInformation required Windows NT 4.0
		if (GetModuleInformation(hProcess, NULL, &modinfo, sizeof(modinfo)))
		{
			// MEMORY_BASIC_INFORMATION required Windows XP
			MEMORY_BASIC_INFORMATION mbi;

			if (VirtualQueryEx(hProcess, modinfo.EntryPoint, &mbi, sizeof(mbi)))
			{
				ImageBaseAddress = mbi.AllocationBase;
			}
		}
		CloseHandle(hProcess);
	}
#endif
	if (ImageBaseAddress == NULL)
		return;
#if defined(__BORLANDC__)
	for (vector<MODULEENTRY32A>::iterator it = moduleList->begin(), end = moduleList->end(); it < end; it++)
#else
	for (LPMODULEENTRY32A it = moduleList->_M_start, end = moduleList->_M_finish; it < end; it++)
#endif
	{
		typedef struct {
			char szExePath[MAX_PATH];
		} *COMPILER_DEPENDENT_COPY_BLOCK;

		MODULEENTRY32A EntryModule;

		if (it->modBaseAddr != ImageBaseAddress)
			continue;
		EntryModule.dwSize        = it->dwSize       ;
		EntryModule.th32ModuleID  = it->th32ModuleID ;
		EntryModule.th32ProcessID = it->th32ProcessID;
		EntryModule.GlblcntUsage  = it->GlblcntUsage ;
		EntryModule.ProccntUsage  = it->ProccntUsage ;
		EntryModule.modBaseAddr   = it->modBaseAddr  ;
		EntryModule.modBaseSize   = it->modBaseSize  ;
		EntryModule.hModule       = it->hModule      ;
		*(LPDWORD)EntryModule.szModule = BSWAP32('nul\0');
		*(COMPILER_DEPENDENT_COPY_BLOCK)EntryModule.szExePath = *(COMPILER_DEPENDENT_COPY_BLOCK)it->szExePath;
#if defined(__BORLANDC__)
		moduleList->push_back(EntryModule);
#else
		VectorPushModuleEntry32A(moduleList, &EntryModule);
#endif
		break;
	}
#else
	HMODULE hModule;
	HANDLE  hProcess;

	hModule = NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, th32ProcessID);
	if (hProcess != NULL)
	{
		DWORD cbNeeded;

		// Microsoft Knowledge Base - kb175030
		// https://support.microsoft.com/ja-jp/kb/175030
		// >プロセス内の最初のモジュールはそのプロセスの実行可能モジュールになることを覚えておいてください。

		if (!EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
			hModule = NULL;
		CloseHandle(hProcess);
	}
	if (hModule == NULL)
		return;
#if defined(__BORLANDC__)
	for (vector<MODULEENTRY32A>::iterator it = moduleList->begin(), end = moduleList->end(); it < end; it++)
#else
	for (LPMODULEENTRY32A it = moduleList->_M_start, end = moduleList->_M_finish; it < end; it++)
#endif
	{
		typedef struct {
			char szExePath[MAX_PATH];
		} *COMPILER_DEPENDENT_COPY_BLOCK;

		MODULEENTRY32A EntryModule;

		if (it->hModule != hModule)
			continue;
		EntryModule.dwSize        = it->dwSize       ;
		EntryModule.th32ModuleID  = it->th32ModuleID ;
		EntryModule.th32ProcessID = it->th32ProcessID;
		EntryModule.GlblcntUsage  = it->GlblcntUsage ;
		EntryModule.ProccntUsage  = it->ProccntUsage ;
		EntryModule.modBaseAddr   = it->modBaseAddr  ;
		EntryModule.modBaseSize   = it->modBaseSize  ;
		EntryModule.hModule       = it->hModule      ;
		*(LPDWORD)EntryModule.szModule = BSWAP32('nul\0');
		*(COMPILER_DEPENDENT_COPY_BLOCK)EntryModule.szExePath = *(COMPILER_DEPENDENT_COPY_BLOCK)it->szExePath;
#if defined(__BORLANDC__)
		moduleList->push_back(EntryModule);
#else
		VectorPushModuleEntry32A(moduleList, &EntryModule);
#endif
		break;
	}
#endif
}
