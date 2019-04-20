#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <wincon.h>
#include <stdlib.h>
#include <assert.h>
#include "crt\ParseArgument.h"
#include "crt\wcsnlen.h"
#include "crt\strnlen.h"
#include "crt\wcslen.h"
#include "crt\wmemcpy.h"
#include "crt\wmemmove.h"
#include "Internal.h"
#include "SetTimeDateStamp.h"
#include "SetLinkerVersion.h"
#include "SetOperatingSystemVersion.h"
#include "SetDelayImport.h"
#include "RemoveDebugSection.h"
#include "SetLastWriteTime.h"

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif
#ifndef offsetof
#define offsetof(st, m) ((size_t)(&((st *)0)->m))
#endif

#pragma comment(linker, "/NODEFAULTLIB:msvcrt.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:libc.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcd.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcpmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcpmtd.lib")

#define SHOW_USAGES                             1100
#define FATAL_ERROR_CAN_NOT_OPEN_INPUT_FILE     1181
#define STRING_OF_CODE_CAN_NOT_OPEN_INPUT_FILE  L"1181"
#define LENGTH_OF_CODE_CAN_NOT_OPEN_INPUT_FILE  4
#define FATAL_ERROR_FAILED_TO_CREATE_PROCESS    2000
#define STRING_OF_CODE_FAILED_TO_CREATE_PROCESS L"2000"
#define LENGTH_OF_CODE_FAILED_TO_CREATE_PROCESS 4

HANDLE hHeap;
HANDLE hStdOutput;

/***********************************************************************
 *      Execute
 */
static DWORD Execute(LPWSTR lpCommandLine, LPWSTR *lpScreenBuffer, LPDWORD lpExitCode)
{
	DWORD               dwErrCode;
	STARTUPINFOW        si;
	HANDLE              hReadPipe;
	SECURITY_ATTRIBUTES PipeAttributes;

	*lpScreenBuffer = NULL;
	if (lpExitCode)
		*lpExitCode = ERROR_SUCCESS;
	PipeAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	PipeAttributes.lpSecurityDescriptor = NULL;
	PipeAttributes.bInheritHandle = TRUE;
	if (!CreatePipe(&hReadPipe, &si.hStdOutput, &PipeAttributes, 0))
		goto FAILED5;
	do
	{
		PROCESS_INFORMATION pi;
		DWORD               dwSize;
		LPSTR               lpMultiByteStr;

		si.cb = sizeof(si);
		si.lpReserved = NULL;
		si.lpDesktop = NULL;
		si.lpTitle = NULL;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;
		si.cbReserved2 = 0;
		si.hStdInput = NULL;
		si.hStdError = NULL;
		if (!CreateProcessW(NULL, lpCommandLine, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
			goto FAILED3;
		CloseHandle(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
		if (lpExitCode)
			GetExitCodeProcess(pi.hProcess, lpExitCode);
		CloseHandle(pi.hProcess);
		dwSize = GetFileSize(hReadPipe, NULL);
		if (!dwSize)
			goto SUCCESS;
		if (dwSize == 0xFFFFFFFF)
			goto FAILED4;
		lpMultiByteStr = HeapAlloc(hHeap, 0, (dwSize + 1) * sizeof(wchar_t));
		if (!lpMultiByteStr)
			goto FAILED4;
		do
		{
			DWORD  dwNumberOfBytesRead;
			int    cchWideChar;
			LPWSTR lpWideCharStr;

			if (!ReadFile(hReadPipe, lpMultiByteStr, dwSize, &dwNumberOfBytesRead, NULL))
				goto FAILED1;
			*(lpMultiByteStr + dwNumberOfBytesRead) = '\0';
			cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, NULL, 0);
			if (!cchWideChar)
				goto FAILED1;
			lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (unsigned int)cchWideChar * sizeof(wchar_t));
			if (!lpWideCharStr)
				goto FAILED1;
			cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, lpWideCharStr, cchWideChar);
			if (!cchWideChar)
				goto FAILED2;
			*lpScreenBuffer = lpWideCharStr;
			dwErrCode = ERROR_SUCCESS;
			break;
		FAILED1:
			dwErrCode = GetLastError();
			break;
		FAILED2:
			dwErrCode = GetLastError();
			HeapFree(hHeap, 0, lpWideCharStr);
		} while (0);
		HeapFree(hHeap, 0, lpMultiByteStr);
		break;
	FAILED3:
		dwErrCode = FATAL_ERROR_FAILED_TO_CREATE_PROCESS;
		break;
	SUCCESS:
		dwErrCode = ERROR_SUCCESS;
		break;
	FAILED4:
		dwErrCode = GetLastError();
	} while (0);
	CloseHandle(hReadPipe);
	CloseHandle(si.hStdOutput);
	return dwErrCode;

FAILED5:
	return GetLastError();
}

/***********************************************************************
 *      GetNextSection
 */
static LPWSTR GetNextSection(LPWSTR lpSection)
{
	LPWSTR p;

	p = lpSection;
	while ((*p >= L'\t' && *p <= L'\r') || *p == L' ')
		p++;
	while (*p)
	{
		if (*p == L' ' || *p == L'\t')
		{
			while (*(++p) == L' ' || *p == L'\t');
			if (!*p)
				break;
		}
		if (*p == L'\r')
			p++;
		if (*(p++) == L'\n')
			break;
		while (*p && *(p++) != L'\n');
	}
	return p;
}

/***********************************************************************
 *      OutputConsoleW
 */
static BOOL OutputConsoleW(HANDLE hConsoleOutput, LPCWSTR lpBuffer, DWORD dwNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten)
{
	BOOL  bSuccess;
	DWORD dwNumberOfCharsWritten;

	dwNumberOfCharsWritten = 0;
	if (!(bSuccess = !(dwNumberOfCharsToWrite = (DWORD)wcsnlen(lpBuffer, dwNumberOfCharsToWrite))))
	{
		unsigned int uCodePage;
		unsigned int cchMultiByte;
		LPSTR        lpMultiByteStr;

		if ((uCodePage = GetConsoleOutputCP()) &&
			(cchMultiByte = WideCharToMultiByte(uCodePage, 0, lpBuffer, dwNumberOfCharsToWrite, NULL, 0, NULL, NULL)) &&
			(lpMultiByteStr = HeapAlloc(hHeap, 0, cchMultiByte)))
		{
			if (cchMultiByte = WideCharToMultiByte(uCodePage, 0, lpBuffer, dwNumberOfCharsToWrite, lpMultiByteStr, cchMultiByte, NULL, NULL))
			{
				bSuccess = WriteFile(hConsoleOutput, lpMultiByteStr, cchMultiByte, &dwNumberOfCharsWritten, NULL);
				if (lpNumberOfCharsWritten)
					dwNumberOfCharsWritten = MultiByteToWideChar(uCodePage, 0, lpMultiByteStr, min(dwNumberOfCharsWritten, cchMultiByte), NULL, -1);
			}
			HeapFree(hHeap, 0, lpMultiByteStr);
		}
	}
	if (lpNumberOfCharsWritten)
		*lpNumberOfCharsWritten = dwNumberOfCharsWritten;
	return bSuccess;
}

/***********************************************************************
 *      OutputConsoleA
 */
static BOOL OutputConsoleA(HANDLE hConsoleOutput, LPCSTR lpBuffer, DWORD dwNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten)
{
	BOOL  bSuccess;
	DWORD dwNumberOfCharsWritten;

	dwNumberOfCharsWritten = 0;
	if (!(bSuccess = !(dwNumberOfCharsToWrite = (DWORD)strnlen(lpBuffer, dwNumberOfCharsToWrite))))
	{
		bSuccess = WriteFile(hConsoleOutput, lpBuffer, dwNumberOfCharsToWrite, &dwNumberOfCharsWritten, NULL);
		if (dwNumberOfCharsWritten > dwNumberOfCharsToWrite)
			dwNumberOfCharsWritten = dwNumberOfCharsToWrite;
	}
	if (lpNumberOfCharsWritten)
		*lpNumberOfCharsWritten = dwNumberOfCharsWritten;
	return bSuccess;
}

/***********************************************************************
 *      wmain
 */
#define wmain inline_wmain
static __forceinline DWORD wmain(int argc, wchar_t *argv[])
{
	DWORD dwExitCode;

	do
	{
		BOOL    OutputFromChild;
		PWSTR   lpFile = NULL;
		PWSTR   lpEditBin = NULL;
		LPWSTR  lpUsageBuffer = NULL;
		LPWSTR  lpUsage;

		hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOutput == INVALID_HANDLE_VALUE)
		{
			dwExitCode = GetLastError();
			break;
		}
		OutputFromChild = FALSE;
		do
		{
			wchar_t  lpCommandLine[16];
			size_t   nEditBinLength;
			wchar_t  lpFileName[MAX_PATH + 1];
			BOOL     fWaitForFileUnlock       = FALSE;
			PWSTR    lpTimeDateStamp          = NULL;
			PWSTR    lpDelayImport            = NULL;
			PWSTR    lpLinkerVersion          = NULL;
			PWSTR    lpOperatingSystemVersion = NULL;
			BOOL     fRemoveDebugSection      = FALSE;
			PWSTR    lpLastWriteTime          = NULL;
			int      i;
			DWORD    dwLength;
			HANDLE   hFile;
			FILETIME ftLastWriteTime;

			lpCommandLine[ 0] = L'e';
			lpCommandLine[ 1] = L'd';
			lpCommandLine[ 2] = L'i';
			lpCommandLine[ 3] = L't';
			lpCommandLine[ 4] = L'b';
			lpCommandLine[ 5] = L'i';
			lpCommandLine[ 6] = L'n';
			lpCommandLine[ 7] = L'.';
			lpCommandLine[ 8] = L'e';
			lpCommandLine[ 9] = L'x';
			lpCommandLine[10] = L'e';
			lpCommandLine[11] = L'\0';
			dwExitCode = Execute(lpCommandLine, &lpUsageBuffer, NULL);
			if (lpUsageBuffer)
			{
				lpUsage = GetNextSection(lpUsageBuffer);
				OutputConsoleW(hStdOutput, lpUsageBuffer, lpUsage - lpUsageBuffer, NULL);
			}
			if (dwExitCode != ERROR_SUCCESS)
				break;
			if (argc < 2)
			{
				dwExitCode = SHOW_USAGES;
				break;
			}
			for (i = 1; i < argc; i++)
			{
				PWSTR p;

				p = argv[i];
				if (*p == L'/' || *p == L'-')
				{
					p++;
					if (*p != L'?')
					{
						if (!_wcsnicmp(p, L"waitforfileunlock", 18))
							fWaitForFileUnlock = TRUE;
						else if (!_wcsnicmp(p, L"timedatestamp:", 14))
							lpTimeDateStamp = p + 14;
						else if (!_wcsnicmp(p, L"linkerversion:", 14))
							lpLinkerVersion = p + 14;
						else if (!_wcsnicmp(p, L"operatingsystemversion:", 23))
							lpOperatingSystemVersion = p + 23;
						else if (!_wcsnicmp(p, L"delayimport:", 12))
							lpDelayImport = p + 12;
						else if (!_wcsnicmp(p, L"removedebugsection", 19))
							fRemoveDebugSection = TRUE;
						else if (!_wcsnicmp(p, L"lastwritetime:", 14))
							lpLastWriteTime = p + 14;
						else
						{
							size_t Length;

							Length = wcslen(p);
							if (!lpEditBin)
							{
								nEditBinLength = 11;
								lpEditBin = HeapAlloc(hHeap, 0, (nEditBinLength + 2 + Length) * sizeof(wchar_t));
								if (!lpEditBin)
								{
									dwExitCode = GetLastError();
									break;
								}
							}
							else
							{
								LPVOID lpMem;

								lpMem = HeapReAlloc(hHeap, 0, lpEditBin, (nEditBinLength + 2 + Length) * sizeof(wchar_t));
								if (!lpMem)
								{
									dwExitCode = GetLastError();
									break;
								}
								lpEditBin = (LPWSTR)lpMem;
							}
							lpEditBin[nEditBinLength    ] = L' ';
							lpEditBin[nEditBinLength + 1] = L'/';
							wmemcpy(lpEditBin + nEditBinLength + 2, p, Length);
							nEditBinLength += 2 + Length;
						}
					}
					else
						dwExitCode = SHOW_USAGES;
				}
				else if (!lpFile)
					lpFile = p;
			}
			if (dwExitCode != ERROR_SUCCESS)
				break;
			if (!lpFile)
			{
				dwExitCode = ERROR_INVALID_PARAMETER;
				break;
			}
			dwLength = SearchPathW(NULL, lpFile, NULL, _countof(lpFileName), lpFileName, NULL);
			if (!dwLength)
				goto ERROR_INVALID_FILE;
			if (dwLength >= _countof(lpFileName))
			{
				dwExitCode = ERROR_BUFFER_OVERFLOW;
				break;
			}
			hFile = CreateFileW(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				if (!fWaitForFileUnlock)
					goto ERROR_INVALID_FILE;
				do
				{
					DWORD dwError;

					dwError = GetLastError();
					if (dwError != ERROR_SHARING_VIOLATION && dwError != ERROR_LOCK_VIOLATION)
						goto ERROR_INVALID_FILE;
					Sleep(100);
					hFile = CreateFileW(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				} while (hFile == INVALID_HANDLE_VALUE);
			}
			if (!GetFileTime(hFile, NULL, NULL, &ftLastWriteTime))
			{
				dwExitCode = GetLastError();
				CloseHandle(hFile);
				break;
			}
			CloseHandle(hFile);
			if (lpEditBin)
			{
				size_t Length;
				LPVOID lpMem;
				LPWSTR lpScreenBuffer;
				DWORD  dwExitCodeProcess;

				Length = wcslen(lpFileName);
				lpMem = HeapReAlloc(hHeap, 0, lpEditBin, (nEditBinLength + Length + 4) * sizeof(wchar_t));
				if (!lpMem)
				{
					dwExitCode = GetLastError();
					break;
				}
				lpEditBin = (LPWSTR)lpMem;
				lpEditBin[ 0] = L'e';
				lpEditBin[ 1] = L'd';
				lpEditBin[ 2] = L'i';
				lpEditBin[ 3] = L't';
				lpEditBin[ 4] = L'b';
				lpEditBin[ 5] = L'i';
				lpEditBin[ 6] = L'n';
				lpEditBin[ 7] = L'.';
				lpEditBin[ 8] = L'e';
				lpEditBin[ 9] = L'x';
				lpEditBin[10] = L'e';
				lpEditBin[nEditBinLength    ] = L' ';
				lpEditBin[nEditBinLength + 1] = L'"';
				wmemcpy(lpEditBin + nEditBinLength + 2, lpFileName, Length);
				nEditBinLength += Length + 2;
				lpEditBin[nEditBinLength    ] = L'"';
				lpEditBin[nEditBinLength + 1] = L'\0';
				dwExitCode = Execute(lpEditBin, &lpScreenBuffer, &dwExitCodeProcess);
				hFile = CreateFileW(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					SetFileTime(hFile, NULL, NULL, &ftLastWriteTime);
					CloseHandle(hFile);
				}
				if (dwExitCodeProcess != ERROR_SUCCESS)
				{
					LPWSTR lpMessage;

					dwExitCode = dwExitCodeProcess;
					OutputFromChild = TRUE;
					lpMessage = GetNextSection(lpScreenBuffer);
					OutputConsoleW(hStdOutput, lpMessage, -1, NULL);
				}
				HeapFree(hHeap, 0, lpScreenBuffer);
				if (dwExitCode != ERROR_SUCCESS)
					break;
			}
			hFile = CreateFileW(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
			ERROR_INVALID_FILE:
				dwExitCode = FATAL_ERROR_CAN_NOT_OPEN_INPUT_FILE;
				break;
			}
			do
			{
				DWORD  FileSize;
				HANDLE hFileMappingObject;

				if (!lpTimeDateStamp
				 && !lpDelayImport
				 && !lpLinkerVersion
				 && !lpOperatingSystemVersion
				 && !fRemoveDebugSection)
					break;
				FileSize = GetFileSize(hFile, NULL);
				if (FileSize == (DWORD)-1)
				{
					dwExitCode = GetLastError();
					break;
				}
				hFileMappingObject = CreateFileMappingW(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
				if (!hFileMappingObject)
				{
					dwExitCode = GetLastError();
					break;
				}
				do
				{
					PVOID BaseAddress;

					BaseAddress = MapViewOfFile(hFileMappingObject, FILE_MAP_ALL_ACCESS, 0, 0, 0);
					if (!BaseAddress)
					{
						dwExitCode = GetLastError();
						break;
					}
					do
					{
						PIMAGE_DOS_HEADER DosHeader;
						PIMAGE_NT_HEADERS NtHeaders;
						BOOL              PE32Plus;
						BOOLEAN           HasCheckSum;

						DosHeader = (PIMAGE_DOS_HEADER)BaseAddress;
						/* Check whether the header is valid and belongs to a PE executable */
						if (IsBadReadPtr(DosHeader, sizeof(IMAGE_DOS_HEADER)))
						{
							dwExitCode = ERROR_ACCESS_DENIED;
							break;
						}
						if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
						{
							dwExitCode = ERROR_BAD_EXE_FORMAT;
							break;
						}

						NtHeaders = (PIMAGE_NT_HEADERS)((size_t)BaseAddress + (size_t)DosHeader->e_lfanew);
						if (IsBadReadPtr(NtHeaders, offsetof(IMAGE_NT_HEADERS, OptionalHeader)))
						{
							dwExitCode = ERROR_ACCESS_DENIED;
							break;
						}
						/* Check the signature */
						if (NtHeaders->Signature != IMAGE_NT_SIGNATURE)
						{
							dwExitCode = ERROR_BAD_EXE_FORMAT;
							break;
						}
						/* IMAGE_OPTIONAL_HEADER.Magic field contains the value that allows   */
						/* to distinguish between PE32 and PE32+ formats                      */
						if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
						{
							/* PE32 */
							if (NtHeaders->FileHeader.SizeOfOptionalHeader < sizeof(IMAGE_OPTIONAL_HEADER32))
							{
								dwExitCode = ERROR_BAD_EXE_FORMAT;
								break;
							}
							PE32Plus = FALSE;
						}
						else if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
						{
							/* PE32+ */
							if (NtHeaders->FileHeader.SizeOfOptionalHeader < sizeof(IMAGE_OPTIONAL_HEADER64))
							{
								dwExitCode = ERROR_BAD_EXE_FORMAT;
								break;
							}
							PE32Plus = TRUE;
						}
						else
						{
							/* Unknown value -> Report an error */
							dwExitCode = ERROR_BAD_EXE_FORMAT;
							break;
						}
						if (IsBadReadPtr(&NtHeaders->OptionalHeader, NtHeaders->FileHeader.SizeOfOptionalHeader))
						{
							dwExitCode = ERROR_ACCESS_DENIED;
							break;
						}

						assert(offsetof(IMAGE_NT_HEADERS32, OptionalHeader) == offsetof(IMAGE_NT_HEADERS64, OptionalHeader));
						assert(offsetof(IMAGE_OPTIONAL_HEADER32, CheckSum) == offsetof(IMAGE_OPTIONAL_HEADER64, CheckSum));

						HasCheckSum = ValidateCheckSum(BaseAddress, FileSize, NtHeaders);

						if (lpTimeDateStamp)
						{
							dwExitCode = SetTimeDateStamp(BaseAddress, FileSize, NtHeaders, PE32Plus, HasCheckSum, lpTimeDateStamp);
							if (dwExitCode != ERROR_SUCCESS)
								break;
						}
						if (lpDelayImport)
						{
							dwExitCode = SetDelayImport(BaseAddress, FileSize, NtHeaders, PE32Plus, HasCheckSum, lpDelayImport);
							if (dwExitCode != ERROR_SUCCESS)
								break;
						}
						if (lpLinkerVersion)
						{
							dwExitCode = SetLinkerVersion(BaseAddress, FileSize, NtHeaders, HasCheckSum, lpLinkerVersion);
							if (dwExitCode != ERROR_SUCCESS)
								break;
						}
						if (lpOperatingSystemVersion)
						{
							dwExitCode = SetOperatingSystemVersion(BaseAddress, FileSize, NtHeaders, HasCheckSum, lpOperatingSystemVersion);
							if (dwExitCode != ERROR_SUCCESS)
								break;
						}
						if (fRemoveDebugSection)
						{
							dwExitCode = RemoveDebugSection(BaseAddress, FileSize, NtHeaders, PE32Plus, HasCheckSum);
							if (dwExitCode != ERROR_SUCCESS)
								break;
						}
					} while (0);
					UnmapViewOfFile(BaseAddress);
				} while (0);
				CloseHandle(hFileMappingObject);
			} while (0);
			if (dwExitCode == ERROR_SUCCESS && lpLastWriteTime)
				dwExitCode = SetLastWriteTime(hFile, &ftLastWriteTime, lpLastWriteTime);
			CloseHandle(hFile);
		} while (0);
		do
		{
			if (lpEditBin)
				HeapFree(hHeap, 0, lpEditBin);
			if (dwExitCode == ERROR_SUCCESS)
				break;
			if (OutputFromChild)
				break;
			if (dwExitCode == FATAL_ERROR_FAILED_TO_CREATE_PROCESS)
			{
				OutputConsoleW(hStdOutput,
					L"EDITBINEX : fatal error LNK"
					STRING_OF_CODE_FAILED_TO_CREATE_PROCESS
					L": editbin.exe ‚ðŽÀs‚Å‚«‚Ü‚¹‚ñ\r\n",
					51 + LENGTH_OF_CODE_FAILED_TO_CREATE_PROCESS, NULL);
			}
			else if (dwExitCode == SHOW_USAGES)
			{
				if (lpUsageBuffer)
				{
					size_t  nLength;
					wchar_t ch;
					size_t  nIndex;
					size_t  nUsageLength;
					size_t  i;
					wchar_t *p1, *p2;

					nLength = wcslen(lpUsageBuffer);
					while (nLength && (((ch = lpUsageBuffer[nLength - 1]) >= L'\t' && ch <= L'\r') || ch == L' '))
					{
						lpUsageBuffer[nLength - 1] = L'\0';
						nLength--;
					}
					nIndex = lpUsage - lpUsageBuffer;
					nUsageLength = nLength - nIndex;
					if (nUsageLength >= 7)
					{
						size_t nCount;

						nCount = nUsageLength - 7;
						i = 0;
						do
						{
							if ((lpUsage[i    ] == L'E' || lpUsage[i    ] == L'e')
							 && (lpUsage[i + 1] == L'D' || lpUsage[i + 1] == L'd')
							 && (lpUsage[i + 2] == L'I' || lpUsage[i + 2] == L'i')
							 && (lpUsage[i + 3] == L'T' || lpUsage[i + 3] == L't')
							 && (lpUsage[i + 4] == L'B' || lpUsage[i + 4] == L'b')
							 && (lpUsage[i + 5] == L'I' || lpUsage[i + 5] == L'i')
							 && (lpUsage[i + 6] == L'N' || lpUsage[i + 6] == L'n'))
							{
								LPVOID lpMem;

								lpMem = HeapReAlloc(hHeap, 0, lpUsageBuffer, (nLength + 3) * sizeof(wchar_t));
								if (!lpMem)
									break;
								lpUsageBuffer = (wchar_t *)lpMem;
								lpUsage = lpUsageBuffer + nIndex;
								wmemmove(lpUsage + i + 9, lpUsage + i + 7, nCount - i);
								lpUsage[i + 7] = lpUsage[i    ];
								lpUsage[i + 8] = lpUsage[i + 1] - L'D' + L'X';
								nLength += 2;
								i += 8;
								nCount += 2;
							}
						} while (++i <= nCount);
					}
					OutputConsoleW(hStdOutput, lpUsage, nLength - (lpUsage - lpUsageBuffer), NULL);
					p1 = NULL;
					p2 = lpUsageBuffer + nLength - 2;
					nLength = 0;
					for (; p2 >= lpUsage; p2--)
					{
						if (*p2 == L'\n')
						{
							p2++;
							if (*p2 == ' ' || *p2 == L'\t')
							{
								p1 = p2;
								do
									nLength++;
								while (*(++p2) == L' ' || *p2 == L'\t');
							}
							break;
						}
					}
					OutputConsoleW(hStdOutput, L"\r\n", 2, NULL);
					OutputConsoleW(hStdOutput, p1, nLength, NULL);
					OutputConsoleW(hStdOutput, L"/WaitForFileUnlock\r\n", 20, NULL);
					OutputConsoleW(hStdOutput, p1, nLength, NULL);
					OutputConsoleW(hStdOutput, L"/TimeDateStamp:[0x]#\r\n", 22, NULL);
					OutputConsoleW(hStdOutput, p1, nLength, NULL);
					OutputConsoleW(hStdOutput, L"/LinkerVersion:[0x]#[.[0x]#]\r\n", 30, NULL);
					OutputConsoleW(hStdOutput, p1, nLength, NULL);
					OutputConsoleW(hStdOutput, L"/OperatingSystemVersion:[0x]#[.[0x]#]\r\n", 39, NULL);
					OutputConsoleW(hStdOutput, p1, nLength, NULL);
					OutputConsoleW(hStdOutput, L"/DelayImport:[0x]#,[0x]#\r\n", 26, NULL);
					OutputConsoleW(hStdOutput, p1, nLength, NULL);
					OutputConsoleW(hStdOutput, L"/RemoveDebugSection\r\n", 21, NULL);
					OutputConsoleW(hStdOutput, p1, nLength, NULL);
					OutputConsoleW(hStdOutput, L"/LastWriteTime:[YYYY-MM-DD][,HH:MM:SS]\r\n", 40, NULL);
				}
			}
			else if (dwExitCode == FATAL_ERROR_CAN_NOT_OPEN_INPUT_FILE)
			{
				OutputConsoleW(hStdOutput,
					L"EDITBINEX : fatal error LNK"
					STRING_OF_CODE_CAN_NOT_OPEN_INPUT_FILE
					L": “ü—ÍÌ§²Ù \"",
					37 + LENGTH_OF_CODE_CAN_NOT_OPEN_INPUT_FILE, NULL);
				OutputConsoleW(hStdOutput, lpFile, -1, NULL);
				OutputConsoleW(hStdOutput, L"\" ‚ðŠJ‚¯‚Ü‚¹‚ñ\r\n", 10, NULL);
			}
			else
			{
				DWORD   n;
				wchar_t buf[11];
				wchar_t *end, *p;
				DWORD   dwMsgLength;
				wchar_t *lpMsgBuf;

				OutputConsoleW(hStdOutput, L"EDITBINEX : ERROR ", 18, NULL);
				n = dwExitCode;
				p = end = buf + _countof(buf) - 1;
				*p = L'\0';
				do
				{
					*(--p) = L'0' + ((unsigned short)n % 10);
					n /= 10;
				} while (n);
				OutputConsoleW(hStdOutput, p, end - p, NULL);
				OutputConsoleW(hStdOutput, L": ", 2, NULL);
				dwMsgLength = FormatMessageW(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					dwExitCode,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPWSTR)&lpMsgBuf,
					0,
					NULL);
				if (dwMsgLength)
				{
					size_t i;

					switch (dwExitCode)
					{
					case ERROR_BAD_EXE_FORMAT:
						i = (size_t)dwMsgLength - 1;
						do
						{
							if (lpMsgBuf[i] == L'1')
							{
								if (lpMsgBuf[--i] == L'%')
								{
									size_t nAddLength;
									size_t nNewLength;
									HLOCAL hNewBuf;

									nAddLength = wcslen(lpFile);
									nNewLength = (size_t)dwMsgLength + nAddLength;
									hNewBuf = LocalAlloc(LMEM_FIXED, (nNewLength + 1) * sizeof(wchar_t));
									if (hNewBuf)
									{
										const wchar_t *src;
										wchar_t       *dest;

										src = lpMsgBuf;
										dest = (wchar_t *)hNewBuf;
										wmemcpy(dest, src, i);
										src += i + 2;
										dest += i;
										*(dest++) = L'"';
										wmemcpy(dest, lpFile, nAddLength);
										dest += nAddLength;
										*(dest++) = L'"';
										wmemcpy(dest, src, wcslen(src) + 1);
										LocalFree(lpMsgBuf);
										lpMsgBuf = (wchar_t *)hNewBuf;
										dwMsgLength = (DWORD)nNewLength;
									}
								}
								if (!i)
									break;
							}
						} while (--i);
						break;
					}
					OutputConsoleW(hStdOutput, lpMsgBuf, dwMsgLength, NULL);
					LocalFree(lpMsgBuf);
				}
			}
		} while (0);
		if (lpUsageBuffer)
			HeapFree(hHeap, 0, lpUsageBuffer);
	} while (0);

	return dwExitCode;
}

/***********************************************************************
 *      mainCRTStartup
 */
EXTERN_C int __cdecl mainCRTStartup(void)
{
	DWORD dwExitCode;

	if (hHeap = GetProcessHeap())
	{
		wchar_t **argv;
		int     argc;

		if (argv = ParseArgumentW(hHeap, GetCommandLineW(), &argc))
		{
			dwExitCode = wmain(argc, argv);
			HeapFree(hHeap, 0, argv);
		}
		else
			dwExitCode = ERROR_NOT_ENOUGH_MEMORY;
	}
	else
		dwExitCode = GetLastError();
	return dwExitCode;
}
