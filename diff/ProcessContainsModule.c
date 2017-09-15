#include <windows.h>

#ifdef __BORLANDC__
#pragma warn -8060
#endif

#ifndef __BORLANDC__
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")
#else
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#define ProcessBasicInformation 0

typedef /*__success(return >= 0) */LONG NTSTATUS;

typedef int PROCESSINFOCLASS;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _PEB_LDR_DATA {
	BYTE       Reserved1[8];
	PVOID      Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB {
	BYTE                                   Reserved1[2];
	BYTE                                   BeingDebugged;
	BYTE                                   Reserved2[1];
	PVOID                                  Reserved3[2];
	PPEB_LDR_DATA                          Ldr;
	PVOID/*PRTL_USER_PROCESS_PARAMETERS*/  ProcessParameters;
	BYTE                                   Reserved4[104];
	PVOID                                  Reserved5[52];
	PVOID/*PPS_POST_PROCESS_INIT_ROUTINE*/ PostProcessInitRoutine;
	BYTE                                   Reserved6[128];
	PVOID                                  Reserved7[1];
	ULONG                                  SessionId;
} PEB, *PPEB;

typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID     Reserved1;
	PPEB      PebBaseAddress;
	PVOID     Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID     Reserved3;
} PROCESS_BASIC_INFORMATION;
#endif

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#ifndef __BORLANDC__
#define USING_REGEX 1
#endif

#if USING_REGEX
#include <regex.h>
#endif

#include "GetFileTitlePointer.h"

extern HANDLE hHeap;

BOOL __stdcall ProcessContainsModule(
	IN DWORD   dwProcessId,
	IN BOOL    bIsRegex,
	IN LPCVOID lpModuleName)
{
	BOOL   bFound;
	HANDLE hProcess;

#ifdef __BORLANDC__
	typedef NTSTATUS(WINAPI *PFN_NTQUERYINFORMATIONPROCESS)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

	PFN_NTQUERYINFORMATIONPROCESS NtQueryInformationProcess =
		(PFN_NTQUERYINFORMATIONPROCESS)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	if (NtQueryInformationProcess == NULL)
		return FALSE;
#endif

	bFound = FALSE;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
	if (hProcess)
	{
		do	/* do { ... } while (0); */
		{
			PROCESS_BASIC_INFORMATION pbi;
			PPEB_LDR_DATA             Ldr;
			PLIST_ENTRY               Flink, EndOfFlink;

			if (!NT_SUCCESS(NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL)))
				break;
			if (!ReadProcessMemory(hProcess, &pbi.PebBaseAddress->Ldr, &Ldr, sizeof(Ldr), NULL))
				break;
			if (!ReadProcessMemory(hProcess, &Ldr->InMemoryOrderModuleList.Flink, &Flink, sizeof(Flink), NULL))
				break;
			EndOfFlink = Flink;
			do
			{
				typedef struct _PARTIAL_LDR_DATA_TABLE_ENTRY {
					LIST_ENTRY     InMemoryOrderLinks;
					PVOID          Reserved2[2];
					PVOID          DllBase;
					PVOID          Reserved3[2];
					UNICODE_STRING FullDllName;
				} PARTIAL_LDR_DATA_TABLE_ENTRY, *PPARTIAL_LDR_DATA_TABLE_ENTRY;

				PARTIAL_LDR_DATA_TABLE_ENTRY LdrData;

				if (!ReadProcessMemory(hProcess, Flink, &LdrData, sizeof(LdrData), NULL))
					break;
				if (LdrData.FullDllName.Length)
				{
					wchar_t lpBuffer[MAX_PATH];
					PWSTR   lpFullDllName;
					LPCWSTR lpFileTitle;
					char    lpMultiByteStr[MAX_PATH];
#if USING_REGEX
					BOOL    bUsedDefaultChar;
#endif

					if (LdrData.FullDllName.Length < _countof(lpFullDllName))
						lpFullDllName = lpBuffer;
					else if (!(lpFullDllName = (PWSTR)HeapAlloc(hHeap, 0, (LdrData.FullDllName.Length + 1) * sizeof(wchar_t))))
						break;
					if (!ReadProcessMemory(hProcess, LdrData.FullDllName.Buffer, lpFullDllName, LdrData.FullDllName.Length * sizeof(wchar_t), NULL))
						break;
					lpFullDllName[LdrData.FullDllName.Length] = L'\0';
					lpFileTitle = GetFileTitlePointerW(lpFullDllName);
					bFound = !bIsRegex ?
						_wcsicmp(lpModuleName, lpFileTitle) == 0 :
#if USING_REGEX
						WideCharToMultiByte(CP_THREAD_ACP, 0, lpFileTitle, -1, lpMultiByteStr, _countof(lpMultiByteStr), NULL, &bUsedDefaultChar) &&
						!bUsedDefaultChar &&
						regexec(lpModuleName, lpMultiByteStr, 0, NULL, 0) == 0;
#else
						FALSE;
#endif
					if (lpFullDllName != lpBuffer)
						HeapFree(hHeap, 0, lpFullDllName);
					if (bFound)
						break;
				}
				Flink = LdrData.InMemoryOrderLinks.Flink;
			} while (Flink != EndOfFlink);
		} while (0);
		CloseHandle(hProcess);
	}
	return bFound;
}
