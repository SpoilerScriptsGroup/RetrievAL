#include <windows.h>
#include "PageSize.h"

extern HANDLE hHeap;

EXTERN_C FARPROC __stdcall GetExportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpProcName)
{
	char lpPageBuffer[PAGE_SIZE];

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
		if (!DataDirectory.VirtualAddress || DataDirectory.Size < sizeof(IMAGE_EXPORT_DIRECTORY))
			break;
		lpAddress = (LPCBYTE)hModule + DataDirectory.VirtualAddress + offsetof(IMAGE_EXPORT_DIRECTORY, Base);
		if (!ReadProcessMemory(hProcess, lpAddress, &ExportDirectory, sizeof(ExportDirectory), NULL))
			break;
		if (!IS_INTRESOURCE(lpProcName))
		{
			size_t  nProcNameSize;
			size_t  nSizeOfNames;
			LPDWORD lpdwRelativeNameArray;
			LPDWORD lpdwExternalAddressOfNames;

			if (!ExportDirectory.NumberOfNames)
				break;
			nProcNameSize = strlen(lpProcName) + 1;
			if (nProcNameSize > PAGE_SIZE)
				break;
			nSizeOfNames = ExportDirectory.NumberOfNames * sizeof(DWORD);
			lpdwRelativeNameArray = (LPDWORD)HeapAlloc(hHeap, 0, nSizeOfNames);
			if (!lpdwRelativeNameArray)
				break;
			lpdwExternalAddressOfNames = (LPDWORD)((LPBYTE)hModule + ExportDirectory.AddressOfNames);
			if (ReadProcessMemory(hProcess, lpdwExternalAddressOfNames, lpdwRelativeNameArray, nSizeOfNames, NULL))
			{
				size_t  nBufferedPage;
				LPDWORD lpdwRelativeName;
				LPDWORD lpdwEndOfRelativeName;

				nBufferedPage = (size_t)NULL;
				lpdwRelativeName = lpdwRelativeNameArray;
				lpdwEndOfRelativeName = (LPDWORD)((LPBYTE)lpdwRelativeNameArray + nSizeOfNames);
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
					nNameInPage = nNameAddress & (PAGE_SIZE - 1);
					nPage = nNameAddress - nNameInPage;
					nNextPage = nPage + PAGE_SIZE;
					if (nBufferedPage < nPage || nBufferedPage >= nNextPage)
					{
						if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nPage), lpPageBuffer, PAGE_SIZE, NULL))
							break;
					}
					nSize = PAGE_SIZE - nNameInPage;
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
						if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nNextPage), lpPageBuffer, PAGE_SIZE, NULL))
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
						HeapFree(hHeap, 0, lpdwRelativeNameArray);
						return (FARPROC)((LPBYTE)hModule + dwRelativeFunction);
					}
				} while (++lpdwRelativeName != lpdwEndOfRelativeName);
			}
			HeapFree(hHeap, 0, lpdwRelativeNameArray);
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
