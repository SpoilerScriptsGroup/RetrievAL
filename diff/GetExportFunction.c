#include <windows.h>
#include "intrinsic.h"

extern HANDLE hHeap;

EXTERN_C FARPROC __stdcall GetExportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpProcName)
{
	do	/* do { ... } while (0); */
	{
		typedef struct {
			DWORD Base;
			DWORD NumberOfFunctions;
			DWORD NumberOfNames;
			DWORD AddressOfFunctions;
			DWORD AddressOfNames;
			DWORD AddressOfNameOrdinals;
		} PARTIAL_EXPORT_DIRECTORY;

		LPCBYTE                  lpAddress;
		IMAGE_DOS_HEADER         DosHeader;
		DWORD                    NtSignature;
		WORD                     OptionalHdrMagic;
		IMAGE_DATA_DIRECTORY     DataDirectory;
		PARTIAL_EXPORT_DIRECTORY ExportDirectory;

		lpAddress = (LPCBYTE)hModule;
		if (!ReadProcessMemory(hProcess, lpAddress, &DosHeader, sizeof(DosHeader), NULL))
			break;
		if (DosHeader.e_magic != IMAGE_DOS_SIGNATURE)
			break;
		lpAddress += DosHeader.e_lfanew;
		if (!ReadProcessMemory(hProcess, lpAddress, &NtSignature, sizeof(NtSignature), NULL))
			break;
		if (NtSignature != IMAGE_NT_SIGNATURE)
			break;
		lpAddress += offsetof(IMAGE_NT_HEADERS, OptionalHeader);
		if (!ReadProcessMemory(hProcess, lpAddress, &OptionalHdrMagic, sizeof(OptionalHdrMagic), NULL))
			break;
		if (OptionalHdrMagic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
			lpAddress += offsetof(IMAGE_OPTIONAL_HEADER32, DataDirectory) + IMAGE_DIRECTORY_ENTRY_EXPORT * sizeof(IMAGE_DATA_DIRECTORY);
#ifdef _WIN64
		else if (OptionalHdrMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
			lpAddress += offsetof(IMAGE_OPTIONAL_HEADER64, DataDirectory) + IMAGE_DIRECTORY_ENTRY_EXPORT * sizeof(IMAGE_DATA_DIRECTORY);
#endif
		else
			break;
		if (!ReadProcessMemory(hProcess, lpAddress, &DataDirectory, sizeof(DataDirectory), NULL))
			break;
		if (DataDirectory.VirtualAddress == 0 || DataDirectory.Size < sizeof(IMAGE_EXPORT_DIRECTORY))
			break;
		lpAddress = (LPCBYTE)hModule + DataDirectory.VirtualAddress + offsetof(IMAGE_EXPORT_DIRECTORY, Base);
		if (!ReadProcessMemory(hProcess, lpAddress, &ExportDirectory, sizeof(ExportDirectory), NULL))
			break;
		if (!IS_INTRESOURCE(lpProcName))
		{
			SYSTEM_INFO SystemInfo;
			DWORD       dwPageSize;
			size_t      nProcNameSize;
			DWORD       dwForward;
			size_t      nPageRemainMask;
			size_t      nSizeOfNames;
			LPVOID      lpBuffer;
			LPDWORD     lpdwRelativeNameArray;
			LPDWORD     lpdwExternalAddressOfNames;

			if (ExportDirectory.NumberOfNames == 0)
				break;
			GetSystemInfo(&SystemInfo);
			dwPageSize = SystemInfo.dwPageSize;
			if (dwPageSize == 0)
				break;
			nProcNameSize = strlen(lpProcName) + 1;
			if (nProcNameSize > dwPageSize)
				break;
			_BitScanForward(&dwForward, dwPageSize);
			nPageRemainMask = (size_t)1 << dwForward;
			nPageRemainMask = nPageRemainMask == dwPageSize ?
				nPageRemainMask - 1 :
				0;
			nSizeOfNames = ExportDirectory.NumberOfNames * sizeof(DWORD);
			lpBuffer = HeapAlloc(hHeap, 0, dwPageSize + nSizeOfNames);
			if (lpBuffer == NULL)
				break;
			lpdwRelativeNameArray = (LPDWORD)((LPBYTE)lpBuffer + dwPageSize);
			lpdwExternalAddressOfNames = (LPDWORD)((LPBYTE)hModule + ExportDirectory.AddressOfNames);
			if (ReadProcessMemory(hProcess, lpdwExternalAddressOfNames, lpdwRelativeNameArray, nSizeOfNames, NULL))
			{
				size_t  nBufferedPage;
				LPDWORD lpdwRelativeName;
				LPDWORD lpdwEndOfRelativeName;
				LPSTR   lpPageBuffer;

				nBufferedPage = (size_t)NULL;
				lpdwRelativeName = lpdwRelativeNameArray;
				lpdwEndOfRelativeName = (LPDWORD)((LPBYTE)lpdwRelativeNameArray + nSizeOfNames);
				lpPageBuffer = (LPSTR)lpBuffer;
				do
				{
					size_t nNameAddress;
					size_t nNameInPage;
					size_t nPage;
					size_t nNextPage;
					size_t nSize;
					LPCSTR lpszComparand1;
					LPCSTR lpszComparand2;
					size_t nCompareLength;

					nNameAddress = (size_t)hModule + *lpdwRelativeName;
					if (nPageRemainMask)
						nNameInPage = nNameAddress & nPageRemainMask;
					else
						nNameInPage = nNameAddress % dwPageSize;
					nPage = nNameAddress - nNameInPage;
					nNextPage = nPage + dwPageSize;
					if (nBufferedPage < nPage || nBufferedPage >= nNextPage)
					{
						if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nPage), lpPageBuffer, dwPageSize, NULL))
							break;
					}
					nSize = dwPageSize - nNameInPage;
					if (nSize >= nProcNameSize)
					{
						lpszComparand1 = lpProcName;
						lpszComparand2 = lpPageBuffer + nNameInPage;
						nCompareLength = nProcNameSize;
					}
					else
					{
						if (memcmp(lpProcName, lpPageBuffer + nNameInPage, nSize) != 0)
							continue;
						if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nNextPage), lpPageBuffer, dwPageSize, NULL))
							break;
						lpszComparand1 = lpProcName + nSize;
						lpszComparand2 = lpPageBuffer;
						nCompareLength = nProcNameSize - nSize;
					}
					if (memcmp(lpszComparand1, lpszComparand2, nCompareLength) == 0)
					{
						LPWORD  lpwExternalAddressOfNameOrdinals;
						WORD    wFunctionIndex;
						LPDWORD lpdwExternalAddressOfFunctions;
						DWORD   dwRelativeFunction;

						lpwExternalAddressOfNameOrdinals = (LPWORD)((LPBYTE)hModule + ExportDirectory.AddressOfNameOrdinals);
						if (!ReadProcessMemory(hProcess, lpwExternalAddressOfNameOrdinals + (lpdwRelativeName - lpdwRelativeNameArray), &wFunctionIndex, sizeof(wFunctionIndex), NULL))
							break;
						lpdwExternalAddressOfFunctions = (LPDWORD)((LPBYTE)hModule + ExportDirectory.AddressOfFunctions);
						if (!ReadProcessMemory(hProcess, lpdwExternalAddressOfFunctions + wFunctionIndex, &dwRelativeFunction, sizeof(dwRelativeFunction), NULL))
							break;
						HeapFree(hHeap, 0, lpBuffer);
						return (FARPROC)((LPBYTE)hModule + dwRelativeFunction);
					}
				} while (++lpdwRelativeName != lpdwEndOfRelativeName);
			}
			HeapFree(hHeap, 0, lpBuffer);
		}
		else
		{
			DWORD   dwFunctionIndex;
			LPDWORD lpdwExternalAddressOfFunctions;
			DWORD   dwRelativeFunction;

			if ((DWORD)lpProcName < ExportDirectory.Base)
				break;
			dwFunctionIndex = (DWORD)lpProcName - ExportDirectory.Base;
			if (dwFunctionIndex >= ExportDirectory.NumberOfFunctions)
				break;
			lpdwExternalAddressOfFunctions = (LPDWORD)((LPBYTE)hModule + ExportDirectory.AddressOfFunctions);
			if (!ReadProcessMemory(hProcess, lpdwExternalAddressOfFunctions + dwFunctionIndex, &dwRelativeFunction, sizeof(dwRelativeFunction), NULL))
				break;
			return (FARPROC)((LPBYTE)hModule + dwRelativeFunction);
		}
	} while (0);
	return NULL;
}
