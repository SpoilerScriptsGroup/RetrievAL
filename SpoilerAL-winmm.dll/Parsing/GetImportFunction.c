#include <windows.h>
#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif
#include <mbstring.h>
#define PSAPI_VERSION 1
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#include "GetPageSize.h"

extern HANDLE hHeap;

EXTERN_C FARPROC * __stdcall GetImportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpModuleName, LPCSTR lpProcName)
{
	char                     lpBaseName[MAX_PATH];
	LPCBYTE                  lpAddress;
	IMAGE_DOS_HEADER         DosHeader;
	DWORD                    NtSignature;
	WORD                     OptionalHdrMagic;
	IMAGE_DATA_DIRECTORY     DataDirectory;
	PIMAGE_IMPORT_DESCRIPTOR lpImportDescriptor;
	IMAGE_IMPORT_DESCRIPTOR  ImportDescriptor;
	DWORD                    dwPageSize;
	size_t                   nModuleNameSize;
	LPSTR                    lpBuffer;

	if (!lpModuleName && IS_INTRESOURCE(lpProcName))
	{
		DWORD cbNeeded;

		if (!GetModuleBaseNameA(hProcess, hModule, lpBaseName, _countof(lpBaseName)))
			goto FAILED;
		if (!EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
			goto FAILED;
		lpModuleName = lpBaseName;
	}
	lpAddress = (LPCBYTE)hModule;
	if (!ReadProcessMemory(hProcess, lpAddress, &DosHeader, sizeof(DosHeader), NULL))
		goto FAILED;
	if (DosHeader.e_magic != IMAGE_DOS_SIGNATURE)
		goto FAILED;
	lpAddress += DosHeader.e_lfanew;
	if (!ReadProcessMemory(hProcess, lpAddress, &NtSignature, sizeof(NtSignature), NULL))
		goto FAILED;
	if (NtSignature != IMAGE_NT_SIGNATURE)
		goto FAILED;
	lpAddress += offsetof(IMAGE_NT_HEADERS, OptionalHeader);
	if (!ReadProcessMemory(hProcess, lpAddress, &OptionalHdrMagic, sizeof(OptionalHdrMagic), NULL))
		goto FAILED;
	if (OptionalHdrMagic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		lpAddress += offsetof(IMAGE_OPTIONAL_HEADER32, DataDirectory) + IMAGE_DIRECTORY_ENTRY_IMPORT * sizeof(IMAGE_DATA_DIRECTORY);
#ifdef _WIN64
	else if (OptionalHdrMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
		lpAddress += offsetof(IMAGE_OPTIONAL_HEADER64, DataDirectory) + IMAGE_DIRECTORY_ENTRY_IMPORT * sizeof(IMAGE_DATA_DIRECTORY);
#endif
	else
		goto FAILED;
	if (!ReadProcessMemory(hProcess, lpAddress, &DataDirectory, sizeof(DataDirectory), NULL))
		goto FAILED;
	if (!DataDirectory.VirtualAddress || DataDirectory.Size < sizeof(IMAGE_IMPORT_DESCRIPTOR))
		goto FAILED;
	lpImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((LPCBYTE)hModule + DataDirectory.VirtualAddress);
	if (!ReadProcessMemory(hProcess, lpImportDescriptor, &ImportDescriptor, sizeof(ImportDescriptor), NULL))
		goto FAILED;
	if (!ImportDescriptor.Characteristics)
		goto FAILED;
	dwPageSize = GetPageSize();
	if (!dwPageSize)
		goto FAILED;
	if (lpModuleName)
	{
		if (*lpModuleName)
		{
			nModuleNameSize = strlen(lpModuleName) + 1;
			if (nModuleNameSize > dwPageSize)
				goto FAILED;
		}
		else
		{
			lpModuleName = NULL;
		}
	}
	if (!IS_INTRESOURCE(lpProcName))
	{
		size_t nProcNameSize;
		size_t nBufferedPage;

		nProcNameSize = strlen(lpProcName) + 1;
		if (nProcNameSize > dwPageSize)
			goto FAILED;
		lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, dwPageSize);
		if (!lpBuffer)
			goto FAILED;
		nBufferedPage = (size_t)NULL;
		do
		{
			size_t            nNameAddress;
			size_t            nNameInPage;
			size_t            nPage;
			size_t            nNextPage;
			size_t            nSize;
			PIMAGE_THUNK_DATA lpThunk;

			if (lpModuleName)
			{
				nNameAddress = (size_t)hModule + ImportDescriptor.Name;
				nNameInPage = nNameAddress & (dwPageSize - 1);
				nPage = nNameAddress - nNameInPage;
				nNextPage = nPage + dwPageSize;
				if (nBufferedPage < nPage || nBufferedPage >= nNextPage)
				{
					if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nPage), lpBuffer, dwPageSize, NULL))
						break;
				}
				nSize = dwPageSize - nNameInPage;
				if (nSize >= nModuleNameSize)
				{
					if (_mbsicmp((LPCBYTE)lpModuleName, (LPCBYTE)lpBuffer + nNameInPage) != 0)
						continue;
				}
				else
				{
					memcpy(lpBuffer, (LPCBYTE)lpBuffer + nNameInPage, nSize);
					if (!ReadProcessMemory(hProcess, (LPCVOID)nNextPage, (LPBYTE)lpBuffer + nSize, nNameInPage, NULL))
						break;
					if (_mbsicmp((LPCBYTE)lpModuleName, (LPCBYTE)lpBuffer) != 0)
					{
						memcpy(lpBuffer, (LPCBYTE)lpBuffer + nSize, nNameInPage);
						if (!ReadProcessMemory(hProcess, (LPCBYTE)nNextPage + nNameInPage, (LPBYTE)lpBuffer + nNameInPage, nSize, NULL))
							break;
						nBufferedPage = nNextPage;
						continue;
					}
					nBufferedPage = (size_t)NULL;
				}
			}
			for (lpThunk = (PIMAGE_THUNK_DATA)((LPBYTE)hModule + ImportDescriptor.OriginalFirstThunk); ; lpThunk++)
			{
				ULONG_PTR AddressOfData;
				LPCSTR    lpszComparand1;
				LPCSTR    lpszComparand2;
				size_t    nCompareLength;

				if (!ReadProcessMemory(hProcess, &lpThunk->u1.AddressOfData, &AddressOfData, sizeof(AddressOfData), NULL))
					goto RELEASE;
				if (!AddressOfData)
					break;
				if ((LONG_PTR)AddressOfData < 0)
					continue;
				nNameAddress = (size_t)((PIMAGE_IMPORT_BY_NAME)((LPBYTE)hModule + AddressOfData))->Name;
				nNameInPage = nNameAddress & (dwPageSize - 1);
				nPage = nNameAddress - nNameInPage;
				nNextPage = nPage + dwPageSize;
				if (nBufferedPage < nPage || nBufferedPage >= nNextPage)
				{
					if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nPage), lpBuffer, dwPageSize, NULL))
						goto RELEASE;
				}
				nSize = dwPageSize - nNameInPage;
				if (nSize >= nProcNameSize)
				{
					lpszComparand1 = lpProcName;
					lpszComparand2 = lpBuffer + nNameInPage;
					nCompareLength = nProcNameSize;
				}
				else
				{
					if (memcmp(lpProcName, lpBuffer + nNameInPage, nSize) != 0)
						continue;
					if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nNextPage), lpBuffer, dwPageSize, NULL))
						goto RELEASE;
					lpszComparand1 = lpProcName + nSize;
					lpszComparand2 = lpBuffer;
					nCompareLength = nProcNameSize - nSize;
				}
				if (memcmp(lpszComparand1, lpszComparand2, nCompareLength) != 0)
					continue;
				HeapFree(hHeap, 0, lpBuffer);
				(LPBYTE)lpThunk += ImportDescriptor.FirstThunk - ImportDescriptor.OriginalFirstThunk;
				return (FARPROC *)&lpThunk->u1.Function;
			}
			if (lpModuleName)
				break;
		} while (
			ReadProcessMemory(hProcess, ++lpImportDescriptor, &ImportDescriptor, sizeof(ImportDescriptor), NULL) &&
			ImportDescriptor.Characteristics);
	}
	else
	{
		size_t nBufferedPage;

		lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, dwPageSize);
		if (!lpBuffer)
			goto FAILED;
		nBufferedPage = (size_t)NULL;
		do
		{
			size_t            nNameAddress;
			size_t            nNameInPage;
			size_t            nPage;
			size_t            nNextPage;
			size_t            nSize;
			PIMAGE_THUNK_DATA lpThunk;

			nNameAddress = (size_t)hModule + ImportDescriptor.Name;
			nNameInPage = nNameAddress & (dwPageSize - 1);
			nPage = nNameAddress - nNameInPage;
			nNextPage = nPage + dwPageSize;
			if (nBufferedPage < nPage || nBufferedPage >= nNextPage)
			{
				if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nPage), lpBuffer, dwPageSize, NULL))
					break;
			}
			nSize = dwPageSize - nNameInPage;
			if (nSize >= nModuleNameSize)
			{
				if (_mbsicmp((LPCBYTE)lpModuleName, (LPCBYTE)lpBuffer + nNameInPage) != 0)
					continue;
			}
			else
			{
				memcpy(lpBuffer, (LPCBYTE)lpBuffer + nNameInPage, nSize);
				if (!ReadProcessMemory(hProcess, (LPCVOID)nNextPage, (LPBYTE)lpBuffer + nSize, nNameInPage, NULL))
					break;
				if (_mbsicmp((LPCBYTE)lpModuleName, (LPCBYTE)lpBuffer) != 0)
				{
					memcpy(lpBuffer, (LPCBYTE)lpBuffer + nSize, nNameInPage);
					if (!ReadProcessMemory(hProcess, (LPCBYTE)nNextPage + nNameInPage, (LPBYTE)lpBuffer + nNameInPage, nSize, NULL))
						break;
					nBufferedPage = nNextPage;
					continue;
				}
			}
			for (lpThunk = (PIMAGE_THUNK_DATA)((LPBYTE)hModule + ImportDescriptor.OriginalFirstThunk); ; lpThunk++)
			{
				ULONG_PTR AddressOfData;

				if (!ReadProcessMemory(hProcess, &lpThunk->u1.AddressOfData, &AddressOfData, sizeof(AddressOfData), NULL))
					goto RELEASE;
				if (!AddressOfData)
					goto RELEASE;
				if ((LONG_PTR)AddressOfData >= 0)
					continue;
				if ((AddressOfData & MAXLONG_PTR) != (ULONG_PTR)lpProcName)
					continue;
				HeapFree(hHeap, 0, lpBuffer);
				(LPBYTE)lpThunk += ImportDescriptor.FirstThunk - ImportDescriptor.OriginalFirstThunk;
				return (FARPROC *)&lpThunk->u1.Function;
			}
		} while (
			ReadProcessMemory(hProcess, ++lpImportDescriptor, &ImportDescriptor, sizeof(ImportDescriptor), NULL) &&
			ImportDescriptor.Characteristics);
	}
RELEASE:
	HeapFree(hHeap, 0, lpBuffer);
FAILED:
	return NULL;
}
