#include <windows.h>

#if defined _M_IX86
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

HMODULE hComCtl32 = NULL;

BOOL __cdecl LoadComCtl32()
{
	static __inline BOOL ReplaceImportAddressTable();
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
		ReplaceImportAddressTable();
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

static __inline BOOL ReplaceImportAddressTable()
{
	static const LPCSTR lpProcNames[] = {
		(LPCSTR)0x006559D9,	// "ImageList_Add"
		(LPCSTR)0x006559E9,	// "ImageList_BeginDrag"
		(LPCSTR)0x006559FF,	// "ImageList_Create"
		(LPCSTR)0x00655A13,	// "ImageList_Destroy"
		(LPCSTR)0x00655A27,	// "ImageList_DragEnter"
		(LPCSTR)0x00655A3D,	// "ImageList_DragLeave"
		(LPCSTR)0x00655A53,	// "ImageList_DragMove"
		(LPCSTR)0x00655A69,	// "ImageList_DragShowNolock"
		(LPCSTR)0x00655A85,	// "ImageList_Draw"
		(LPCSTR)0x00655A97,	// "ImageList_DrawEx"
		(LPCSTR)0x00655AAB,	// "ImageList_EndDrag"
		(LPCSTR)0x00655ABF,	// "ImageList_GetBkColor"
		(LPCSTR)0x00655AD7,	// "ImageList_GetDragImage"
		(LPCSTR)0x00655AF1,	// "ImageList_GetIconSize"
		(LPCSTR)0x00655B09,	// "ImageList_GetImageCount"
		(LPCSTR)0x00655B23,	// "ImageList_Read"
		(LPCSTR)0x00655B35,	// "ImageList_Remove"
		(LPCSTR)0x00655B49,	// "ImageList_Replace"
		(LPCSTR)0x00655B5D,	// "ImageList_ReplaceIcon"
		(LPCSTR)0x00655B75,	// "ImageList_SetBkColor"
		(LPCSTR)0x00655B8D,	// "ImageList_SetDragCursorImage"
		(LPCSTR)0x00655BAD,	// "ImageList_SetIconSize"
		(LPCSTR)0x00655BC5,	// "ImageList_Write"
		(LPCSTR)0x00000011,	// MAKEINTRESOURCEA(17)
	};

	DWORD   dwProtect;
	FARPROC lpProcAddress;

	if (!VirtualProtect((LPVOID)0x00654000, 0x00003000, PAGE_READWRITE, &dwProtect))
		return FALSE;
	for (size_t i = 0; i < _countof(lpProcNames); i++)
		if (lpProcAddress = GetProcAddress(hComCtl32, lpProcNames[i]))
			((FARPROC *)0x006545F8)[i] = lpProcAddress;
		else
			return FALSE;
	return VirtualProtect((LPVOID)0x00654000, 0x00003000, PAGE_READONLY, &dwProtect);
}

static __inline void ReplaceExportAddressTable(HMODULE hModule)
{
	PIMAGE_DATA_DIRECTORY   DataDirectory;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory;
	LPDWORD                 AddressOfFunctions;
	LPDWORD                 AddressOfNames;
	LPWORD                  AddressOfNameOrdinals;
	DWORD                   Protect;
	DWORD                   Index;

#ifndef _M_IX86
#error "x86 architecture not defined"
#endif

	#define DOS_HEADER(hModule) ((PIMAGE_DOS_HEADER)hModule)
	#define NT_HEADERS(hModule) ((PIMAGE_NT_HEADERS)((LPCBYTE)hModule + DOS_HEADER(hModule)->e_lfanew))

	DataDirectory = NT_HEADERS(hModule)->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT;
	if (!DataDirectory->VirtualAddress || DataDirectory->Size < sizeof(IMAGE_EXPORT_DIRECTORY))
		return;
	ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((LPCBYTE)hModule + DataDirectory->VirtualAddress);
	AddressOfFunctions    = (LPDWORD)((LPCBYTE)hModule + ExportDirectory->AddressOfFunctions   );
	AddressOfNames        = (LPDWORD)((LPCBYTE)hModule + ExportDirectory->AddressOfNames       );
	AddressOfNameOrdinals = (LPWORD )((LPCBYTE)hModule + ExportDirectory->AddressOfNameOrdinals);
	if (!VirtualProtect(AddressOfFunctions, ExportDirectory->NumberOfFunctions * sizeof(DWORD), PAGE_READWRITE, &Protect))
		return;
	for (Index = 0; Index < ExportDirectory->NumberOfFunctions; Index++)
	{
		FARPROC ProcAddress;

		ProcAddress = GetProcAddress(hComCtl32, MAKEINTRESOURCEA(ExportDirectory->Base + Index));
		if (!ProcAddress)
			continue;
#ifdef _M_IX86
		AddressOfFunctions[Index] = (DWORD)ProcAddress - (DWORD)hModule;
#endif
	}
	for (Index = 0; Index < ExportDirectory->NumberOfNames; Index++)
	{
		FARPROC ProcAddress;

		ProcAddress = GetProcAddress(hComCtl32, (LPCSTR)hModule + AddressOfNames[Index]);
		if (!ProcAddress)
			continue;
#ifdef _M_IX86
		AddressOfFunctions[AddressOfNameOrdinals[Index]] = (DWORD)ProcAddress - (DWORD)hModule;
#endif
	}
	VirtualProtect(AddressOfFunctions, ExportDirectory->NumberOfFunctions * sizeof(DWORD), Protect, &Protect);

	#undef DOS_HEADER
	#undef NT_HEADERS
}
