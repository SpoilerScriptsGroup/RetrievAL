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

static HMODULE hModule = NULL;
static HMODULE hSideBySide = NULL;

BOOL __cdecl LoadComCtl32();
void __cdecl FreeComCtl32();

static __inline void ReplaceExportFunctions()
{
	PIMAGE_DATA_DIRECTORY   DataDirectory;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory;
	LPDWORD                 AddressOfFunctions;
	LPDWORD                 AddressOfNames;
	LPWORD                  AddressOfNameOrdinals;
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
	for (Index = 0; Index < ExportDirectory->NumberOfFunctions; Index++)
	{
		FARPROC ProcAddress;
		LPDWORD Function;
		DWORD   Protect;

		ProcAddress = GetProcAddress(hSideBySide, MAKEINTRESOURCEA(ExportDirectory->Base + Index));
		if (!ProcAddress)
			continue;
		Function = AddressOfFunctions + Index;
#ifdef _M_IX86
		if (!VirtualProtect(Function, sizeof(DWORD), PAGE_READWRITE, &Protect))
			continue;
		*Function = (DWORD)ProcAddress - (DWORD)hModule;
		VirtualProtect(Function, sizeof(DWORD), Protect, &Protect);
#endif
	}
	for (Index = 0; Index < ExportDirectory->NumberOfNames; Index++)
	{
		FARPROC ProcAddress;
		LPDWORD Function;
		DWORD   Protect;

		ProcAddress = GetProcAddress(hSideBySide, (LPCSTR)hModule + AddressOfNames[Index]);
		if (!ProcAddress)
			continue;
		Function = AddressOfFunctions + AddressOfNameOrdinals[Index];
#ifdef _M_IX86
		if (!VirtualProtect(Function, sizeof(DWORD), PAGE_READWRITE, &Protect))
			continue;
		*Function = (DWORD)ProcAddress - (DWORD)hModule;
		VirtualProtect(Function, sizeof(DWORD), Protect, &Protect);
#endif
	}

	#undef DOS_HEADER
	#undef NT_HEADERS
}

BOOL __cdecl LoadComCtl32()
{
	if (hSideBySide)
		return TRUE;
	do
	{
		wchar_t lpModuleName[MAX_PATH];
		UINT    uLength;

		uLength = GetSystemDirectoryW(lpModuleName, _countof(lpModuleName));
		if (uLength == 0 || uLength >= _countof(lpModuleName))
			break;
		if (lpModuleName[uLength - 1] != L'\\')
			lpModuleName[uLength++] = L'\\';
		if (uLength >= _countof(lpModuleName) - 13)
			break;
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
		hModule = LoadLibraryExW(lpModuleName, NULL, LOAD_LIBRARY_AS_DATAFILE);
		if (!hModule)
			break;
		hSideBySide = LoadLibraryW(L"comctl32.dll");
		if (!hSideBySide || hSideBySide == hModule)
			break;
		ReplaceExportFunctions();
		return TRUE;
	} while (0);
	FreeComCtl32();
	return FALSE;
}

void __cdecl FreeComCtl32()
{
	if (hSideBySide)
	{
		FreeLibrary(hSideBySide);
		hSideBySide = NULL;
	}
	if (hModule)
	{
		FreeLibrary(hModule);
		hModule = NULL;
	}
}
