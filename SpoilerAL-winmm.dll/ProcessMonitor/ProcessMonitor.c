#include <windows.h>
#include <winternl.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include <mbstring.h>

#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#pragma intrinsic(_InterlockedCompareExchange)
#else
#define _InterlockedCompareExchange InterlockedCompareExchange
#endif

#include <pdh.h>
#pragma comment(lib, "pdh.lib")
#ifndef PDH_MAX_COUNTER_PATH
#define PDH_MAX_COUNTER_PATH 2048
#endif

#define PSAPI_VERSION 1
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#include <Shlwapi.h>

#ifndef __BORLANDC__
#define USING_REGEX 1
#endif

#if USING_REGEX
#include <regex.h>
#endif

#include "ProcessMonitor.h"
#include "GetFileTitlePointer.h"
#include "ProcessContainsModule.h"
#include "FindWindowContainsModule.h"

#pragma warning(disable:4996)

#ifdef __BORLANDC__
#pragma warn -8065
#endif

extern HANDLE hHeap;

static CRITICAL_SECTION          cs;
static BOOL             volatile bInitialized          = FALSE;
static HANDLE           volatile hMonitorThread        = NULL;
static size_t           volatile nMonitorPIDsCapacity  = 0;
static LPDWORD          volatile lpdwMonitorPIDs       = NULL;
static LPDWORD          volatile lpdwMonitorEndOfPIDs  = NULL;
static size_t           volatile nMonitorNamesCapacity = 0;
static LPBYTE           volatile lpMonitorNames        = NULL;

void __cdecl InitializeProcessMonitor()
{
	InitializeCriticalSection(&cs);
	bInitialized = TRUE;
}

void __cdecl DeleteProcessMonitor()
{
	StopProcessMonitor();
	DeleteCriticalSection(&cs);
	bInitialized = FALSE;
}

void __cdecl StopProcessMonitor()
{
	HANDLE hThread;

	hThread = hMonitorThread;
	hMonitorThread = NULL;
	EnterCriticalSection(&cs);
	nMonitorPIDsCapacity = 0;
	if (lpdwMonitorPIDs)
	{
		HeapFree(hHeap, 0, lpdwMonitorPIDs);
		lpdwMonitorPIDs = NULL;
	}
	lpdwMonitorEndOfPIDs = NULL;
	nMonitorNamesCapacity = 0;
	if (lpMonitorNames)
	{
		HeapFree(hHeap, 0, lpMonitorNames);
		lpMonitorNames = NULL;
	}
	LeaveCriticalSection(&cs);
	if (hThread)
		WaitForSingleObject(hThread, INFINITE);
}

static BOOL __cdecl EnumProcessId()
{
	size_t  nPIDsCapacity;
	LPDWORD lpdwPIDs;
	size_t  nNamesCapacity;
	LPBYTE  lpNames;
	DWORD   dwPIDsSize;

	if (lpdwMonitorPIDs)
	{
		nPIDsCapacity  = nMonitorPIDsCapacity;
		lpdwPIDs       = lpdwMonitorPIDs;
		nNamesCapacity = nMonitorNamesCapacity;
		lpNames        = lpMonitorNames;
		nMonitorPIDsCapacity  = 0;
		lpdwMonitorPIDs       = NULL;
		lpdwMonitorEndOfPIDs  = NULL;
		nMonitorNamesCapacity = 0;
		lpMonitorNames        = NULL;
	}
	else
	{
		lpNames = (LPBYTE)HeapAlloc(hHeap, 0, nNamesCapacity = 4096);
		if (!lpNames)
			goto FAILED1;
		lpdwPIDs = (LPDWORD)HeapAlloc(hHeap, 0, nPIDsCapacity = 1024 * sizeof(DWORD));
		if (!lpdwPIDs)
			goto FAILED2;
	}
	while (EnumProcesses(lpdwPIDs, nPIDsCapacity, &dwPIDsSize))
	{
		size_t  nNamesSize;
		LPBYTE  lpBaseName;
		LPDWORD lpdwEndOfPIDs, lpdwProcessId;

		if (nPIDsCapacity == dwPIDsSize)
		{
			HeapFree(hHeap, 0, lpdwPIDs);
			lpdwPIDs = (LPDWORD)HeapAlloc(hHeap, 0, nPIDsCapacity *= 2);
			if (lpdwPIDs)
				continue;
			else
				goto FAILED2;
		}
		nNamesSize = 0;
		lpBaseName = lpNames;
		lpdwEndOfPIDs = (LPDWORD)((LPBYTE)lpdwPIDs + (dwPIDsSize & ~(sizeof(DWORD) - 1)));
		for (lpdwProcessId = lpdwPIDs; lpdwProcessId != lpdwEndOfPIDs; lpdwProcessId++)
		{
			HANDLE hProcess;
			DWORD  dwLength;

			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, *lpdwProcessId);
			if (hProcess != NULL)
			{
				if (GetModuleBaseNameA(hProcess, NULL, lpBaseName + sizeof(DWORD), MAX_PATH))
					dwLength = strlen(lpBaseName + sizeof(DWORD));
				else
					dwLength = 0;
				CloseHandle(hProcess);
			}
			else
			{
				dwLength = 0;
			}
			*(LPDWORD)lpBaseName = dwLength;
			nNamesSize += sizeof(DWORD) + dwLength + 1;
			if (nNamesSize > nNamesCapacity)
			{
				LPVOID lpMem;

				lpBaseName -= (size_t)lpNames;
				lpMem = HeapReAlloc(hHeap, 0, lpNames, nNamesCapacity *= 2);
				if (!lpMem)
					goto FAILED3;
				lpNames = (LPBYTE)lpMem;
				lpBaseName += (size_t)lpMem;
			}
			lpBaseName += sizeof(DWORD) + dwLength + 1;
		}
		nMonitorPIDsCapacity = nPIDsCapacity;
		lpdwMonitorPIDs = lpdwPIDs;
		lpdwMonitorEndOfPIDs = lpdwEndOfPIDs;
		nMonitorNamesCapacity = nNamesCapacity;
		lpMonitorNames = lpNames;
		return TRUE;
	}
FAILED3:
	HeapFree(hHeap, 0, lpdwPIDs);
FAILED2:
	HeapFree(hHeap, 0, lpNames);
FAILED1:
	return FALSE;
}

static DWORD WINAPI ProcessMonitor(LPVOID lpParameter)
{
	HQUERY hQuery;

	if (PdhOpenQueryW(NULL, 0, &hQuery) == ERROR_SUCCESS)
	{
		wchar_t szInstanceName[MAX_PATH];

		DWORD                       dwProcessId;
		PDH_COUNTER_PATH_ELEMENTS_W cpe;

		GetModuleFileNameW(GetModuleHandleW(NULL), szInstanceName, _countof(szInstanceName));
		PathRemoveExtensionW(szInstanceName);
		dwProcessId = GetCurrentProcessId();
		cpe.szMachineName    = NULL;
		cpe.szObjectName     = L"Process";
		cpe.szInstanceName   = PathFindFileNameW(szInstanceName);
		cpe.szParentInstance = NULL;
		cpe.dwInstanceIndex  = 0;
		cpe.szCounterName    = L"ID Process";
		do
		{
			wchar_t              szCounterPath[PDH_MAX_COUNTER_PATH];
			DWORD                cchCounterPathLength;
			HANDLE               hCounter;
			PDH_STATUS           Status;
			PDH_FMT_COUNTERVALUE fmtValue;
			DWORD                dwThreadId;
			DWORD                dwThreadIndex;
			wchar_t              szNumberString[11];

			cchCounterPathLength = _countof(szCounterPath);
			if (PdhMakeCounterPathW(&cpe, szCounterPath, &cchCounterPathLength, 0) != ERROR_SUCCESS)
				break;
			if (PdhAddCounterW(hQuery, szCounterPath, 0, &hCounter) != ERROR_SUCCESS)
				break;
			if ((Status = PdhCollectQueryData(hQuery)) == ERROR_SUCCESS)
				Status = PdhGetFormattedCounterValue(hCounter, PDH_FMT_LONG, NULL, &fmtValue);
			PdhRemoveCounter(hCounter);
			if (Status != ERROR_SUCCESS)
				break;
			if ((DWORD)fmtValue.longValue != dwProcessId)
				continue;
			dwThreadId = GetCurrentThreadId();
			dwThreadIndex = 0;
			cpe.szObjectName     = L"Thread";
			cpe.szInstanceName   = szNumberString;
			cpe.szParentInstance = PathFindFileNameW(szInstanceName);
			cpe.szCounterName    = L"ID Thread";
			do
			{
				DWORD dwSpan;

				_ultow(dwThreadIndex, szNumberString, 10);
				cchCounterPathLength = _countof(szCounterPath);
				if (PdhMakeCounterPathW(&cpe, szCounterPath, &cchCounterPathLength, 0) != ERROR_SUCCESS)
					break;
				if (PdhAddCounterW(hQuery, szCounterPath, 0, &hCounter) != ERROR_SUCCESS)
					break;
				if ((Status = PdhCollectQueryData(hQuery)) == ERROR_SUCCESS)
					Status = PdhGetFormattedCounterValue(hCounter, PDH_FMT_LONG, NULL, &fmtValue);
				PdhRemoveCounter(hCounter);
				if (Status != ERROR_SUCCESS)
					break;
				if ((DWORD)fmtValue.longValue != dwThreadId)
					continue;
				cpe.szObjectName  = L"Processor";
				cpe.szCounterName = L"% Processor Time";
				cchCounterPathLength = _countof(szCounterPath);
				if (PdhMakeCounterPathW(&cpe, szCounterPath, &cchCounterPathLength, 0) != ERROR_SUCCESS)
					break;
				if (PdhAddCounterW(hQuery, szCounterPath, 0, &hCounter) != ERROR_SUCCESS)
					break;
				dwSpan = 500;
				while (hMonitorThread)
				{
					EnterCriticalSection(&cs);
					if (hMonitorThread)
					{
						if (dwSpan < 4000)
							Status = PdhCollectQueryData(hQuery);
						EnumProcessId();
						LeaveCriticalSection(&cs);
						Sleep(dwSpan);
						if (dwSpan >= 4000)
							continue;
						if (Status != ERROR_SUCCESS)
							continue;
						if (PdhCollectQueryData(hQuery) != ERROR_SUCCESS)
							continue;
						if (PdhGetFormattedCounterValue(hCounter, PDH_FMT_LONG | PDH_FMT_1000, NULL, &fmtValue) != ERROR_SUCCESS)
							continue;
						if (fmtValue.longValue >= 500)
							dwSpan *= 2;
					}
					else
					{
						LeaveCriticalSection(&cs);
						break;
					}
				}
				PdhRemoveCounter(hCounter);
				break;
			} while (++dwThreadIndex);
			break;
		} while (++cpe.dwInstanceIndex);
		PdhCloseQuery(hQuery);
	}
	hMonitorThread = NULL;
	return 0;
}

static BOOL __fastcall ProcessCmdlineInspect(DWORD pid, LPCSTR lpCommandArg) {
	BOOL match = FALSE;
	HANDLE hProcess;
	if (hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid))
	{
		PEB peb;
		RTL_USER_PROCESS_PARAMETERS upp;
		PROCESS_BASIC_INFORMATION pbi;
		LPSTR cmdLine;
		if (!NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL)
			&& ReadProcessMemory(hProcess, pbi.PebBaseAddress   , &peb, sizeof(peb), NULL)
			&& ReadProcessMemory(hProcess, peb.ProcessParameters, &upp, sizeof(upp), NULL)
			&& (cmdLine = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, upp.CommandLine.MaximumLength << 1))
			)
		{
			regex_t cmdArg;
			if (ReadProcessMemory(hProcess, upp.CommandLine.Buffer, &cmdLine[upp.CommandLine.MaximumLength], upp.CommandLine.Length, NULL)
				&& WideCharToMultiByte(CP_THREAD_ACP, 0, (LPCWCH)&cmdLine[upp.CommandLine.MaximumLength], upp.CommandLine.Length >> 1,
									   cmdLine, upp.CommandLine.MaximumLength, NULL, NULL)
				&& !regcomp(&cmdArg, lpCommandArg, REG_EXTENDED | REG_NEWLINE | REG_NOSUB)
				)
			{
				match = !regexec(&cmdArg, cmdLine, 0, NULL, 0);
				regfree(&cmdArg);
			}
			HeapFree(hHeap, 0, cmdLine);
		}
		CloseHandle(hProcess);
	}
	return match;
}

DWORD __stdcall FindProcessId(
	IN          BOOL   bIsRegex,
	IN          LPCSTR lpProcessName,
	IN          size_t nProcessNameLength,
	IN OPTIONAL LPCSTR lpModuleName,
	IN OPTIONAL LPCSTR lpCommandArg)
{
	static BOOL InProcessing = FALSE;
	DWORD       dwProcessId;
	wchar_t     lpWideCharStr[MAX_PATH];
	LPDWORD     lpdwProcessId;
#if USING_REGEX
	size_t      nModuleNameLength;
	LPSTR       lpBuffer;
#endif

	if (!lpProcessName && !lpModuleName)
		return 0;
	if (!bInitialized || (BOOL)_InterlockedCompareExchange((long *)&InProcessing, TRUE, FALSE))
		return 0;
	dwProcessId = 0;
	if (!hMonitorThread)
		if (!EnumProcessId())
			goto FINALLY;
	if (!bIsRegex && lpModuleName)
		if (!MultiByteToWideChar(CP_THREAD_ACP, 0, lpModuleName, -1, lpWideCharStr, _countof(lpWideCharStr)))
			goto FINALLY;
	EnterCriticalSection(&cs);
	if (!bIsRegex)
	{
		if (lpProcessName)
		{
			LPCSTR lpBaseName;

			lpBaseName = (LPCSTR)lpMonitorNames;
			for (lpdwProcessId = lpdwMonitorPIDs; lpdwProcessId != lpdwMonitorEndOfPIDs; lpdwProcessId++)
			{
				DWORD dwLength;

				dwLength = *(LPDWORD)lpBaseName;
				lpBaseName += sizeof(DWORD);
				if (dwLength == nProcessNameLength)
				{
					if (_mbsicmp(lpProcessName, lpBaseName) == 0)
					{
						if ((!lpModuleName || ProcessContainsModule(*lpdwProcessId, FALSE, lpWideCharStr)) &&
							(!lpCommandArg || ProcessCmdlineInspect(*lpdwProcessId, lpCommandArg)))
						{
							dwProcessId = *lpdwProcessId;
							break;
						}
					}
				}
				lpBaseName += dwLength + 1;
			}
		}
		else
		{
			for (lpdwProcessId = lpdwMonitorPIDs; lpdwProcessId != lpdwMonitorEndOfPIDs; lpdwProcessId++)
			{
				if (ProcessContainsModule(*lpdwProcessId, FALSE, lpWideCharStr))
				{
					dwProcessId = *lpdwProcessId;
					break;
				}
			}
		}
	}
#if USING_REGEX
	else if (lpBuffer = HeapAlloc(hHeap, 0,
		nProcessNameLength + (lpProcessName ? 3 : 0) +
		(nModuleNameLength = lpModuleName ? strlen(lpModuleName) : 0) + (lpModuleName ? 3 : 0)))
	{
		regex_t reProcessName;
		regex_t reModuleName;
		LPSTR   dest;
		LPCSTR  src;

		dest = lpBuffer;
		if (lpProcessName)
		{
			src = lpProcessName;
			lpProcessName = dest;
			*(dest++) = '^';
			memcpy(dest, src, nProcessNameLength);
			dest += nProcessNameLength;
			*(((LPWORD)dest)++) = (BYTE)'$';
		}
		if (!lpProcessName || regcomp(&reProcessName, lpProcessName, REG_EXTENDED | REG_ICASE | REG_NOSUB) == 0)
		{
			if (lpModuleName)
			{
				src = lpModuleName;
				lpModuleName = dest;
				*(dest++) = '^';
				memcpy(dest, src, nModuleNameLength);
				dest += nModuleNameLength;
				*(LPWORD)dest = (BYTE)'$';
			}
			if (!lpModuleName || regcomp(&reModuleName, lpModuleName, REG_EXTENDED | REG_ICASE | REG_NOSUB) == 0)
			{
				if (lpProcessName)
				{
					LPCSTR lpBaseName;

					lpBaseName = (LPCSTR)lpMonitorNames;
					for (lpdwProcessId = lpdwMonitorPIDs; lpdwProcessId != lpdwMonitorEndOfPIDs; lpdwProcessId++)
					{
						DWORD dwLength;

						dwLength = *(LPDWORD)lpBaseName;
						lpBaseName += sizeof(DWORD);
						if (regexec(&reProcessName, lpBaseName, 0, NULL, 0) == 0)
						{
							if (!lpModuleName || ProcessContainsModule(*lpdwProcessId, TRUE, &reModuleName))
							{
								dwProcessId = *lpdwProcessId;
								break;
							}
						}
						lpBaseName += dwLength + 1;
					}
				}
				else
				{
					for (lpdwProcessId = lpdwMonitorPIDs; lpdwProcessId != lpdwMonitorEndOfPIDs; lpdwProcessId++)
					{
						if (ProcessContainsModule(*lpdwProcessId, TRUE, &reModuleName))
						{
							dwProcessId = *lpdwProcessId;
							break;
						}
					}
				}
				if (lpModuleName)
					regfree(&reModuleName);
			}
			if (lpProcessName)
				regfree(&reProcessName);
		}
		HeapFree(hHeap, 0, lpBuffer);
	}
#endif
	LeaveCriticalSection(&cs);
	if (!dwProcessId)
	{
		if (!hMonitorThread)
		{
#if 0
			DWORD dwThreadId;

			hMonitorThread = CreateThread(NULL, 0, ProcessMonitor, NULL, 0, &dwThreadId);
#endif
		}
	}
	else
	{
		StopProcessMonitor();
	}
FINALLY:
	InProcessing = FALSE;
	return dwProcessId;
}
