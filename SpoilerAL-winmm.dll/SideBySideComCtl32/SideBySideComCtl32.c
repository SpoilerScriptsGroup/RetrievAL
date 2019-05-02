#include <windows.h>
#include <tlhelp32.h>

#if defined _M_IX86
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

extern HMODULE hHeap;

HMODULE hComCtl32 = NULL;

/***********************************************************************
 *      LoadComCtl32
 */
BOOL __cdecl LoadComCtl32()
{
	static __inline void ReplaceImportAddressTable(HMODULE hModule);
	static __inline void ReplaceExportAddressTable(HMODULE hModule);

	wchar_t lpModuleName[MAX_PATH];
	UINT    uLength;
	HMODULE hModule;

	if (hComCtl32)
		return TRUE;
	uLength = GetSystemDirectoryW(lpModuleName, _countof(lpModuleName));
	if (uLength == 0 || uLength >= _countof(lpModuleName))
		return FALSE;
	if (lpModuleName[uLength - 1] != L'\\')
		lpModuleName[uLength++] = L'\\';
	if (uLength >= _countof(lpModuleName) - 13)
		return FALSE;
	lpModuleName[uLength     ] = L'c';
	lpModuleName[uLength +  1] = L'o';
	lpModuleName[uLength +  2] = L'm';
	lpModuleName[uLength +  3] = L'c';
	lpModuleName[uLength +  4] = L't';
	lpModuleName[uLength +  5] = L'l';
	lpModuleName[uLength +  6] = L'3';
	lpModuleName[uLength +  7] = L'2';
	lpModuleName[uLength +  8] = L'.';
	lpModuleName[uLength +  9] = L'd';
	lpModuleName[uLength + 10] = L'l';
	lpModuleName[uLength + 11] = L'l';
	lpModuleName[uLength + 12] = L'\0';
	hModule = GetModuleHandleW(lpModuleName);
	if (!hModule)
		return FALSE;
	hComCtl32 = LoadLibraryW(L"comctl32.dll");
	if (!hComCtl32)
		return FALSE;
	if (hComCtl32 != hModule)
	{
		ReplaceImportAddressTable(hModule);
		ReplaceExportAddressTable(hModule);
		return TRUE;
	}
	else
	{
		FreeLibrary(hComCtl32);
		hComCtl32 = NULL;
		return FALSE;
	}
}

/***********************************************************************
 *      ReplaceImportAddressTable
 */
static __inline void ReplaceImportAddressTable(HMODULE hModule)
{
	HANDLE         hSnapshot;
	MODULEENTRY32W entry;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return;
	entry.dwSize = sizeof(entry);
	if (!Module32FirstW(hSnapshot, &entry))
		goto FINALLY;
	do
	{
		PIMAGE_DOS_HEADER        lpDosHeader;
		PIMAGE_NT_HEADERS        lpNtHeader;
		PIMAGE_IMPORT_DESCRIPTOR lpDescriptor;

		if (entry.modBaseAddr == (LPBYTE)hModule ||
			entry.modBaseAddr == (LPBYTE)hComCtl32)
			continue;
		lpDosHeader = (PIMAGE_DOS_HEADER)entry.modBaseAddr;
		if (lpDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
			continue;
		lpNtHeader = (PIMAGE_NT_HEADERS)(entry.modBaseAddr + lpDosHeader->e_lfanew);
		if (lpNtHeader->Signature != IMAGE_NT_SIGNATURE)
			continue;
		if (!lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress ||
			!lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
			continue;
		lpDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(entry.modBaseAddr +
			lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		if (!lpDescriptor->Name)
			continue;
		do
		{
			LPCSTR            lpModuleName;
			PIMAGE_THUNK_DATA lpNameTable, lpName, lpAddressTable, lpAddress;
			size_t            nSize;
			DWORD             dwProtect;

			lpModuleName = (LPCSTR)(entry.modBaseAddr + lpDescriptor->Name);
			if (lpModuleName[ 0] != 'C' && lpModuleName[ 0] != 'c' ||
				lpModuleName[ 1] != 'O' && lpModuleName[ 1] != 'o' ||
				lpModuleName[ 2] != 'M' && lpModuleName[ 2] != 'm' ||
				lpModuleName[ 3] != 'C' && lpModuleName[ 3] != 'c' ||
				lpModuleName[ 4] != 'T' && lpModuleName[ 4] != 't' ||
				lpModuleName[ 5] != 'L' && lpModuleName[ 5] != 'l' ||
				lpModuleName[ 6] != '3'                            ||
				lpModuleName[ 7] != '2'                            ||
				lpModuleName[ 8] != '.'                            ||
				lpModuleName[ 9] != 'D' && lpModuleName[ 9] != 'd' ||
				lpModuleName[10] != 'L' && lpModuleName[10] != 'l' ||
				lpModuleName[11] != 'L' && lpModuleName[11] != 'l' ||
				lpModuleName[12] != '\0')
				continue;
			lpName = lpNameTable = (PIMAGE_THUNK_DATA)(entry.modBaseAddr + lpDescriptor->OriginalFirstThunk);
			while (lpName->u1.AddressOfData)
				lpName++;
			if (!(nSize = (size_t)lpName - (size_t)lpNameTable))
				break;
			lpName = lpNameTable;
			lpAddress = lpAddressTable = (PIMAGE_THUNK_DATA)(entry.modBaseAddr + lpDescriptor->FirstThunk);
			if (!VirtualProtect(lpAddressTable, nSize, PAGE_READWRITE, &dwProtect))
				break;
			do
			{
				LPCSTR  lpProcName;
				FARPROC lpProcAddress;

				lpProcName = IMAGE_SNAP_BY_ORDINAL(lpName->u1.AddressOfData) ?
					(LPCSTR)IMAGE_ORDINAL(lpName->u1.AddressOfData) :
					((PIMAGE_IMPORT_BY_NAME)(entry.modBaseAddr + lpName->u1.AddressOfData))->Name;
				lpProcAddress = GetProcAddress(hComCtl32, lpProcName);
				if (lpProcAddress)
					lpAddress->u1.Function = (ULONG_PTR)lpProcAddress;
				lpName++;
				lpAddress++;
			} while (lpName->u1.AddressOfData);
			VirtualProtect(lpAddressTable, nSize, dwProtect, &dwProtect);
			break;
		} while ((++lpDescriptor)->Characteristics);
	} while (Module32NextW(hSnapshot, &entry));
FINALLY:
	CloseHandle(hSnapshot);
}

/***********************************************************************
 *      ReplaceExportAddressTable
 */
static __inline void ReplaceExportAddressTable(HMODULE hModule)
{
	#define DOS_HEADER(hModule) ((PIMAGE_DOS_HEADER)(hModule))
	#define NT_HEADERS(hModule) ((PIMAGE_NT_HEADERS)((LPBYTE)(hModule) + DOS_HEADER(hModule)->e_lfanew))

	PIMAGE_DATA_DIRECTORY   lpDataDirectory;
	PIMAGE_EXPORT_DIRECTORY lpExportDirectory;
	LPDWORD                 lpFunctions;
	LPDWORD                 lpNames;
	LPWORD                  lpNameOrdinals;
	DWORD                   dwProtect;
	DWORD                   dwFunctionIndex;

#ifndef _M_IX86
	#error "x86 architecture not defined"
#endif

	lpDataDirectory = NT_HEADERS(hModule)->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT;
	if (!lpDataDirectory->VirtualAddress || lpDataDirectory->Size < sizeof(IMAGE_EXPORT_DIRECTORY))
		return;
	lpExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)hModule + lpDataDirectory->VirtualAddress);
	if (!lpExportDirectory->NumberOfFunctions)
		return;
	lpFunctions = (LPDWORD)((LPBYTE)hModule + lpExportDirectory->AddressOfFunctions);
	if (!VirtualProtect(lpFunctions, lpExportDirectory->NumberOfFunctions * sizeof(DWORD), PAGE_READWRITE, &dwProtect))
		return;
	lpNames = (LPDWORD)((LPBYTE)hModule + lpExportDirectory->AddressOfNames);
	lpNameOrdinals = (LPWORD)((LPBYTE)hModule + lpExportDirectory->AddressOfNameOrdinals);
	dwFunctionIndex = 0;
	do
	{
		LPCSTR  lpProcName;
		FARPROC lpProcAddress;

		lpProcName = MAKEINTRESOURCEA(lpExportDirectory->Base + dwFunctionIndex);
		if (lpExportDirectory->NumberOfNames)
		{
			DWORD dwNameIndex;

			dwNameIndex = 0;
			do
				if (lpNameOrdinals[dwNameIndex] == dwFunctionIndex)
				{
					lpProcName = (LPCSTR)hModule + lpNames[dwNameIndex];
					break;
				}
			while (++dwNameIndex < lpExportDirectory->NumberOfNames);
		}
		lpProcAddress = GetProcAddress(hComCtl32, lpProcName);
		if (lpProcAddress)
#ifdef _M_IX86
			lpFunctions[dwFunctionIndex] = (DWORD)lpProcAddress - (DWORD)hModule;
#endif
	} while (++dwFunctionIndex < lpExportDirectory->NumberOfFunctions);
	VirtualProtect(lpFunctions, lpExportDirectory->NumberOfFunctions * sizeof(DWORD), dwProtect, &dwProtect);

	#undef DOS_HEADER
	#undef NT_HEADERS
}

