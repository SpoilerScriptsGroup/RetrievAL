#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <wincon.h>
#include <stdlib.h>
#include "crt\ParseArgument.h"
#include "crt\_wcslen.h"
#include "crt\wmemcpy.h"
#include "crt\wmemmove.h"
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

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:libcmtd.lib")

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
DWORD Execute(LPWSTR lpCommandLine, LPWSTR *lpScreenBuffer, LPDWORD lpExitCode)
{
	DWORD dwErrCode;

	dwErrCode = ERROR_SUCCESS;
	*lpScreenBuffer = NULL;
	if (lpExitCode != NULL)
	{
		*lpExitCode = ERROR_SUCCESS;
	}
	do
	{
		STARTUPINFOW        si;
		HANDLE              hReadPipe;
		SECURITY_ATTRIBUTES PipeAttributes;

		PipeAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
		PipeAttributes.lpSecurityDescriptor = NULL;
		PipeAttributes.bInheritHandle = TRUE;
		if (CreatePipe(&hReadPipe, &si.hStdOutput, &PipeAttributes, 0) == FALSE)
		{
			dwErrCode = GetLastError();
			break;
		}
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
			if (CreateProcessW(NULL, lpCommandLine, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi) == FALSE)
			{
				dwErrCode = FATAL_ERROR_FAILED_TO_CREATE_PROCESS;
				break;
			}
			CloseHandle(pi.hThread);
			WaitForSingleObject(pi.hProcess, INFINITE);
			if (lpExitCode != NULL)
			{
				GetExitCodeProcess(pi.hProcess, lpExitCode);
			}
			CloseHandle(pi.hProcess);
			dwSize = GetFileSize(hReadPipe, NULL);
			if (dwSize == 0)
			{
				break;
			}
			if (dwSize == 0xFFFFFFFF)
			{
				dwErrCode = GetLastError();
				break;
			}
			lpMultiByteStr = HeapAlloc(hHeap, 0, (dwSize + 1) * sizeof(wchar_t));
			if (lpMultiByteStr == NULL)
			{
				dwErrCode = GetLastError();
				break;
			}
			do
			{
				DWORD  dwNumberOfBytesRead;
				int    cchWideChar;
				LPWSTR lpWideCharStr;

				if (ReadFile(hReadPipe, lpMultiByteStr, dwSize, &dwNumberOfBytesRead, NULL) == FALSE)
				{
					dwErrCode = GetLastError();
					break;
				}
				*(lpMultiByteStr + dwNumberOfBytesRead) = '\0';
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, NULL, 0);
				if (cchWideChar == 0)
				{
					dwErrCode = GetLastError();
					break;
				}
				lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (unsigned int)cchWideChar * sizeof(wchar_t));
				if (lpWideCharStr == NULL)
				{
					dwErrCode = GetLastError();
					break;
				}
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, lpWideCharStr, cchWideChar);
				if (cchWideChar == 0)
				{
					dwErrCode = GetLastError();
					HeapFree(hHeap, 0, lpWideCharStr);
					break;
				}
				*lpScreenBuffer = lpWideCharStr;
			}
			while (0);
			HeapFree(hHeap, 0, lpMultiByteStr);
		}
		while (0);
		CloseHandle(hReadPipe);
		CloseHandle(si.hStdOutput);
	}
	while (0);
	return dwErrCode;
}

/***********************************************************************
 *      GetNextSection
 */
LPWSTR GetNextSection(LPWSTR lpSection)
{
	LPWSTR p;

	p = lpSection;
	while ((*p >= L'\t' && *p <= L'\r') || *p == L' ')
	{
		p++;
	}
	while (*p != L'\0')
	{
		if (*p == L' ' || *p == L'\t')
		{
			while (*(++p) == L' ' || *p == L'\t');
			if (*p == L'\0')
			{
				break;
			}
		}
		if (*p == L'\r')
		{
			p++;
		}
		if (*(p++) == L'\n')
		{
			break;
		}
		while (*p != L'\0' && *(p++) != L'\n');
	}
	return p;
}

/***********************************************************************
 *      WriteConsole
 */
#undef WriteConsole
void WriteConsole(const wchar_t *lpWideCharStr, int cchWideChar)
{
	UINT CodePage;
	int  cchMultiByte;

	CodePage = GetConsoleOutputCP();
	cchMultiByte = WideCharToMultiByte(CodePage, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL);
	if (cchMultiByte > 0)
	{
		LPSTR lpMultiByteStr;

		lpMultiByteStr = HeapAlloc(hHeap, 0, (unsigned int)cchMultiByte * sizeof(char));
		if (lpMultiByteStr != NULL)
		{
			cchMultiByte = WideCharToMultiByte(CodePage, 0, lpWideCharStr, cchWideChar, lpMultiByteStr, cchMultiByte, NULL, NULL);
			if (cchMultiByte > 0)
			{
				DWORD dwNumberOfBytesWritten;

				WriteFile(hStdOutput, lpMultiByteStr, cchMultiByte, &dwNumberOfBytesWritten, NULL);
			}
			HeapFree(hHeap, 0, lpMultiByteStr);
		}
	}
}

/***********************************************************************
 *      _wmain
 */
__forceinline DWORD __cdecl _wmain(int argc, wchar_t *argv[])
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
			if (lpUsageBuffer != NULL)
			{
				lpUsage = GetNextSection(lpUsageBuffer);
				WriteConsole(lpUsageBuffer, lpUsage - lpUsageBuffer);
			}
			if (dwExitCode != ERROR_SUCCESS)
			{
				break;
			}
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
						if (_wcsnicmp(p, L"waitforfileunlock", 18) == 0)
						{
							fWaitForFileUnlock = TRUE;
						}
						else if (_wcsnicmp(p, L"timedatestamp:", 14) == 0)
						{
							lpTimeDateStamp = p + 14;
						}
						else if (_wcsnicmp(p, L"linkerversion:", 14) == 0)
						{
							lpLinkerVersion = p + 14;
						}
						else if (_wcsnicmp(p, L"operatingsystemversion:", 23) == 0)
						{
							lpOperatingSystemVersion = p + 23;
						}
						else if (_wcsnicmp(p, L"delayimport:", 12) == 0)
						{
							lpDelayImport = p + 12;
						}
						else if (_wcsnicmp(p, L"removedebugsection", 19) == 0)
						{
							fRemoveDebugSection = TRUE;
						}
						else if (_wcsnicmp(p, L"lastwritetime:", 14) == 0)
						{
							lpLastWriteTime = p + 14;
						}
						else
						{
							size_t Length;

							Length = _wcslen(p);
							if (lpEditBin == NULL)
							{
								nEditBinLength = 11;
								lpEditBin = HeapAlloc(hHeap, 0, (nEditBinLength + 2 + Length) * sizeof(wchar_t));
								if (lpEditBin == NULL)
								{
									dwExitCode = GetLastError();
									break;
								}
							}
							else
							{
								LPVOID lpMem;

								lpMem = HeapReAlloc(hHeap, 0, lpEditBin, (nEditBinLength + 2 + Length) * sizeof(wchar_t));
								if (lpMem == NULL)
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
					{
						dwExitCode = SHOW_USAGES;
					}
				}
				else if (lpFile == NULL)
				{
					lpFile = p;
				}
			}
			if (dwExitCode != ERROR_SUCCESS)
			{
				break;
			}
			if (lpFile == NULL)
			{
				dwExitCode = ERROR_INVALID_PARAMETER;
				break;
			}
			dwLength = SearchPathW(NULL, lpFile, NULL, _countof(lpFileName), lpFileName, NULL);
			if (dwLength == 0)
			{
				goto ERROR_INVALID_FILE;
			}
			if (dwLength >= _countof(lpFileName))
			{
				dwExitCode = ERROR_BUFFER_OVERFLOW;
				break;
			}
			hFile = CreateFileW(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				if (fWaitForFileUnlock == FALSE)
				{
					goto ERROR_INVALID_FILE;
				}
				do
				{
					DWORD dwError;

					dwError = GetLastError();
					if (dwError != ERROR_SHARING_VIOLATION && dwError != ERROR_LOCK_VIOLATION)
					{
						goto ERROR_INVALID_FILE;
					}
					Sleep(100);
					hFile = CreateFileW(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				}
				while (hFile == INVALID_HANDLE_VALUE);
			}
			if (GetFileTime(hFile, NULL, NULL, &ftLastWriteTime) == FALSE)
			{
				dwExitCode = GetLastError();
				CloseHandle(hFile);
				break;
			}
			CloseHandle(hFile);
			if (lpEditBin != NULL)
			{
				size_t Length;
				LPVOID lpMem;
				LPWSTR lpScreenBuffer;
				DWORD  dwExitCodeProcess;

				Length = _wcslen(lpFileName);
				lpMem = HeapReAlloc(hHeap, 0, lpEditBin, (nEditBinLength + Length + 4) * sizeof(wchar_t));
				if (lpMem == NULL)
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
				lpEditBin[nEditBinLength + 1] = L'\"';
				wmemcpy(lpEditBin + nEditBinLength + 2, lpFileName, Length);
				nEditBinLength += Length + 2;
				lpEditBin[nEditBinLength    ] = L'\"';
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
					WriteConsole(lpMessage, -1);
				}
				HeapFree(hHeap, 0, lpScreenBuffer);
				if (dwExitCode != ERROR_SUCCESS)
				{
					break;
				}
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
				DWORD  SizeOfImage;
				HANDLE hFileMappingObject;

				if (lpTimeDateStamp == NULL
				 && lpDelayImport == NULL
				 && lpLinkerVersion == NULL
				 && lpOperatingSystemVersion == NULL
				 && fRemoveDebugSection == FALSE)
				{
					break;
				}
				SizeOfImage = GetFileSize(hFile, NULL);
				if (SizeOfImage == (DWORD)-1)
				{
					dwExitCode = GetLastError();
					break;
				}
				hFileMappingObject = CreateFileMappingW(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
				if (hFileMappingObject == NULL)
				{
					dwExitCode = GetLastError();
					break;
				}
				do
				{
					PVOID BaseAddress;

					BaseAddress = MapViewOfFile(hFileMappingObject, FILE_MAP_ALL_ACCESS, 0, 0, 0);
					if (BaseAddress == NULL)
					{
						dwExitCode = GetLastError();
						break;
					}
					do
					{
						PIMAGE_DOS_HEADER DosHeader;
						PIMAGE_NT_HEADERS NtHeaders;
						BOOL              PE32Plus;

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

						if (lpTimeDateStamp != NULL)
						{
							dwExitCode = SetTimeDateStamp(BaseAddress, SizeOfImage, NtHeaders, PE32Plus, lpTimeDateStamp);
							if (dwExitCode != ERROR_SUCCESS)
							{
								break;
							}
						}
						if (lpDelayImport != NULL)
						{
							dwExitCode = SetDelayImport(BaseAddress, SizeOfImage, NtHeaders, PE32Plus, lpDelayImport);
							if (dwExitCode != ERROR_SUCCESS)
							{
								break;
							}
						}
						if (lpLinkerVersion != NULL)
						{
							dwExitCode = SetLinkerVersion(BaseAddress, SizeOfImage, NtHeaders, lpLinkerVersion);
							if (dwExitCode != ERROR_SUCCESS)
							{
								break;
							}
						}
						if (lpOperatingSystemVersion != NULL)
						{
							dwExitCode = SetOperatingSystemVersion(BaseAddress, SizeOfImage, NtHeaders, lpOperatingSystemVersion);
							if (dwExitCode != ERROR_SUCCESS)
							{
								break;
							}
						}
						if (fRemoveDebugSection != FALSE)
						{
							dwExitCode = RemoveDebugSection(BaseAddress, SizeOfImage, NtHeaders, PE32Plus);
							if (dwExitCode != ERROR_SUCCESS)
							{
								break;
							}
						}
					}
					while (0);
					UnmapViewOfFile(BaseAddress);
				}
				while (0);
				CloseHandle(hFileMappingObject);
			}
			while (0);
			if (dwExitCode == ERROR_SUCCESS && lpLastWriteTime != NULL)
			{
				dwExitCode = SetLastWriteTime(hFile, &ftLastWriteTime, lpLastWriteTime);
			}
			CloseHandle(hFile);
		}
		while (0);
		do
		{
			if (lpEditBin != NULL)
			{
				HeapFree(hHeap, 0, lpEditBin);
			}
			if (dwExitCode == ERROR_SUCCESS)
			{
				break;
			}
			if (OutputFromChild != FALSE)
			{
				break;
			}
			if (dwExitCode == FATAL_ERROR_FAILED_TO_CREATE_PROCESS)
			{
				WriteConsole(
					L"EDITBINEX : fatal error LNK"
					STRING_OF_CODE_FAILED_TO_CREATE_PROCESS
					L": editbin.exe ‚ðŽÀs‚Å‚«‚Ü‚¹‚ñ\r\n",
					51 + LENGTH_OF_CODE_FAILED_TO_CREATE_PROCESS);
			}
			else if (dwExitCode == SHOW_USAGES)
			{
				if (lpUsageBuffer != NULL)
				{
					size_t  nLength;
					wchar_t ch;
					size_t  nIndex;
					size_t  nUsageLength;
					size_t  i;
					wchar_t *p1, *p2;

					nLength = _wcslen(lpUsageBuffer);
					while (nLength != 0 && (((ch = lpUsageBuffer[nLength - 1]) >= L'\t' && ch <= L'\r') || ch == L' '))
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
								if (lpMem == NULL)
								{
									break;
								}
								lpUsageBuffer = (wchar_t *)lpMem;
								lpUsage = lpUsageBuffer + nIndex;
								wmemmove(lpUsage + i + 9, lpUsage + i + 7, nCount - i);
								lpUsage[i + 7] = lpUsage[i    ];
								lpUsage[i + 8] = lpUsage[i + 1] - L'D' + L'X';
								nLength += 2;
								i += 8;
								nCount += 2;
							}
						}
						while (++i <= nCount);
					}
					WriteConsole(lpUsage, nLength - (lpUsage - lpUsageBuffer));
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
								{
									nLength++;
								}
								while (*(++p2) == L' ' || *p2 == L'\t');
							}
							break;
						}
					}
					WriteConsole(L"\r\n", 2);
					WriteConsole(p1, nLength);
					WriteConsole(L"/WaitForFileUnlock\r\n", 20);
					WriteConsole(p1, nLength);
					WriteConsole(L"/TimeDateStamp:[0x]#\r\n", 22);
					WriteConsole(p1, nLength);
					WriteConsole(L"/LinkerVersion:[0x]#[.[0x]#]\r\n", 30);
					WriteConsole(p1, nLength);
					WriteConsole(L"/OperatingSystemVersion:[0x]#[.[0x]#]\r\n", 39);
					WriteConsole(p1, nLength);
					WriteConsole(L"/DelayImport:[0x]#,[0x]#\r\n", 26);
					WriteConsole(p1, nLength);
					WriteConsole(L"/RemoveDebugSection\r\n", 21);
					WriteConsole(p1, nLength);
					WriteConsole(L"/LastWriteTime:[YYYY-MM-DD][,HH:MM:SS]\r\n", 40);
				}
			}
			else if (dwExitCode == FATAL_ERROR_CAN_NOT_OPEN_INPUT_FILE)
			{
				WriteConsole(
					L"EDITBINEX : fatal error LNK"
					STRING_OF_CODE_CAN_NOT_OPEN_INPUT_FILE
					L": “ü—ÍÌ§²Ù \"",
					37 + LENGTH_OF_CODE_CAN_NOT_OPEN_INPUT_FILE);
				WriteConsole(lpFile, -1);
				WriteConsole(L"\" ‚ðŠJ‚¯‚Ü‚¹‚ñ\r\n", 10);
			}
			else
			{
				DWORD   n;
				wchar_t buf[11];
				wchar_t *end, *p;
				DWORD   dwMsgLength;
				wchar_t *lpMsgBuf;

				WriteConsole(L"EDITBINEX : ERROR ", 18);
				n = dwExitCode;
				p = end = buf + _countof(buf) - 1;
				*p = L'\0';
				do
				{
					*(--p) = L'0' + ((unsigned short)n % 10);
					n /= 10;
				}
				while (n != 0);
				WriteConsole(p, end - p);
				WriteConsole(L": ", 2);
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
				if (dwMsgLength != 0)
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

									nAddLength = _wcslen(lpFile);
									nNewLength = (size_t)dwMsgLength + nAddLength;
									hNewBuf = LocalAlloc(LMEM_FIXED, (nNewLength + 1) * sizeof(wchar_t));
									if (hNewBuf != NULL)
									{
										const wchar_t *src;
										wchar_t       *dest;

										src = lpMsgBuf;
										dest = (wchar_t *)hNewBuf;
										wmemcpy(dest, src, i);
										src += i + 2;
										dest += i;
										*(dest++) = L'\"';
										wmemcpy(dest, lpFile, nAddLength);
										dest += nAddLength;
										*(dest++) = L'\"';
										wmemcpy(dest, src, _wcslen(src) + 1);
										LocalFree(lpMsgBuf);
										lpMsgBuf = (wchar_t *)hNewBuf;
										dwMsgLength = (DWORD)nNewLength;
									}
								}
								if (i == 0)
								{
									break;
								}
							}
						}
						while (--i != 0);
						break;
					}
					WriteConsole(lpMsgBuf, dwMsgLength);
					LocalFree(lpMsgBuf);
				}
			}
		}
		while (0);
		if (lpUsageBuffer != NULL)
		{
			HeapFree(hHeap, 0, lpUsageBuffer);
		}
	}
	while (0);

	return dwExitCode;
}

/***********************************************************************
 *      mainCRTStartup
 */
EXTERN_C void __cdecl mainCRTStartup(void)
{
	DWORD dwExitCode;

	do
	{
		PWSTR *argv;
		int   argc;

		hHeap = GetProcessHeap();
		if (hHeap == NULL)
		{
			dwExitCode = GetLastError();
			break;
		}
		argv = ParseArgumentW(hHeap, GetCommandLineW(), &argc);
		if (argv == NULL)
		{
			dwExitCode = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}
		dwExitCode = _wmain(argc, argv);
		HeapFree(hHeap, 0, argv);
	}
	while (0);

	ExitProcess(dwExitCode);
}
