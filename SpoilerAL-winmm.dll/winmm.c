#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <mmsystem.h>
#include "verbose.h"
#include "plugin.h"
#include "intrinsic.h"

#ifndef _DEBUG
#define DISABLE_CRT   1
#define ENABLE_ASMLIB 1
#endif

#if DISABLE_CRT
#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:msvcrt.lib")
#if ENABLE_ASMLIB
#pragma comment(lib, "crt\\asmlib\\libacof32o.lib")
#endif
#endif

#include "crc32\crc32.h"
#include "ToolTip\ToolTip.h"
#include "OptimizeAllocator.h"

HANDLE  hHeap  = NULL;
HANDLE  pHeap  = NULL;
HMODULE hWinMM = NULL;

#if DISABLE_CRT
#define DllMain _DllMainCRTStartup
#endif

/***********************************************************************
 *      DllMain
 */
EXTERN_C BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#if DISABLE_CRT
	EXTERN_C void __cdecl __isa_available_init();
#endif

	static __inline BOOL Attach();
	static __inline void Detach();

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
#if DISABLE_CRT
		__isa_available_init();
#endif
		init_verbose(hInstance);
		verbose(VERBOSE_INFO, "_DllMainCRTStartup - DLL_PROCESS_ATTACH");
		return Attach();
	case DLL_PROCESS_DETACH:
		verbose(VERBOSE_INFO, "_DllMainCRTStartup - DLL_PROCESS_DETACH");
		Detach();
		break;
	}
	return TRUE;
}

/***********************************************************************
 *      Attach
 */
static __inline BOOL Attach()
{
	EXTERN_C char    lpMenuProfileName[MAX_PATH];
	EXTERN_C HMODULE hMsImg32;

	EXTERN_C void __cdecl Attach_Parsing();
	EXTERN_C void __cdecl Attach_AddressNamingAdditionalType();
	EXTERN_C void __cdecl Attach_AddressNamingFromFloat();
	EXTERN_C void __cdecl FixAdjustByString();
	EXTERN_C void __cdecl Attach_EnumReadSSG();
	EXTERN_C void __cdecl Attach_FixGetModuleFromName();
	EXTERN_C void __cdecl Attach_AddEntryModule();
	EXTERN_C void __cdecl Attach_FixToggleByteArray();
	EXTERN_C void __cdecl Attach_OnSSGCtrlCleared();
	EXTERN_C void __cdecl Attach_LoadFromFile();
#if ENABLE_ASMLIB
	EXTERN_C void __cdecl OptimizeCRT();
#endif
	EXTERN_C void __cdecl OptimizeStringDivision();
	EXTERN_C void __cdecl Attach_FixLoopByteArray();
	EXTERN_C void __cdecl Attach_FixGetSSGDataFile();
	EXTERN_C void __cdecl Attach_FixTraceAndCopy();
	EXTERN_C void __cdecl Attach_FixAdjustByValue();
	EXTERN_C void __cdecl Attach_FixMainForm();
	EXTERN_C void __cdecl Attach_FixRepeat();
	EXTERN_C void __cdecl Attach_FixRemoveSpace();
	EXTERN_C void __cdecl Attach_StringSubject();
	EXTERN_C void __cdecl Attach_FixByteArrayFind();
	EXTERN_C void __cdecl Attach_ErrorSkip();
	EXTERN_C void __cdecl Attach_ProcessMonitor();
	EXTERN_C void __cdecl Attach_AddressNamingFmt();
	EXTERN_C void __cdecl Attach_FixSplitElement();
	EXTERN_C void __cdecl Attach_NocachedMemoryList();
	EXTERN_C void __cdecl Attach_SubjectProperty();
	EXTERN_C void __cdecl Attach_RepeatIndex();
	EXTERN_C void __cdecl Attach_FormatNameString();
	EXTERN_C void __cdecl Attach_FixListFEP();
	EXTERN_C void __cdecl Attach_ShowErrorMessage();
	EXTERN_C void __cdecl Attach_FixDoubleList();
	EXTERN_C void __cdecl OptimizeGuide();
	EXTERN_C void __cdecl Attach_CommonList();
	EXTERN_C void __cdecl Attach_FixGetDistractionString();
	EXTERN_C void __cdecl Attach_ForceFunnel();
	EXTERN_C void __cdecl Attach_MinMaxParam();
	EXTERN_C void __cdecl Attach_SubjectStringTable();
	EXTERN_C void __cdecl Attach_FixFindName();
	EXTERN_C BOOL __cdecl LoadComCtl32();
	EXTERN_C void __cdecl Attach_FixClearChild();
	EXTERN_C void __cdecl FixMaskBytes();
	EXTERN_C void __cdecl Attach_FixSortTitle();

#if 0
	static __inline BOOL ModifyImportAddressTable(HMODULE hEntryModule);
#else
	static __inline BOOL ModifyImportAddressTable();
#endif
	static __inline BOOL ModifyCodeSection();
	static __inline BOOL ModifyResourceSection();

	HMODULE hEntryModule;
	wchar_t lpFileName[MAX_PATH];
	UINT    uLength;
	char    lpProfileName[MAX_PATH];
	char    lpDirectoryPath[MAX_PATH];
	DWORD   crcTarget;
	DWORD   crc;

	hEntryModule = GetModuleHandleW(NULL);
	if (hEntryModule == NULL)
		return FALSE;
	uLength = GetModuleFileNameW(hEntryModule, lpFileName, _countof(lpFileName));
	if (uLength == 0)
		return FALSE;
	*lpProfileName = '\0';
	*lpDirectoryPath = '\0';
	do
	{
		wchar_t c;

		if ((c = lpFileName[--uLength]) == L'\\' || c == L'/')
		{
			unsigned int cchMultiByte;
			BOOL         bHasException;

			cchMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, lpFileName, ++uLength, NULL, 0, NULL, &bHasException);
			if (!bHasException && cchMultiByte < MAX_PATH - 8)
			{
				WideCharToMultiByte(CP_THREAD_ACP, 0, lpFileName, uLength, lpDirectoryPath, _countof(lpDirectoryPath), NULL, NULL);
				lpDirectoryPath[cchMultiByte] = '\0';
				memcpy(lpMenuProfileName, lpDirectoryPath, cchMultiByte);
				lpMenuProfileName[cchMultiByte    ] = 'm';
				lpMenuProfileName[cchMultiByte + 1] = 'e';
				lpMenuProfileName[cchMultiByte + 2] = 'n';
				lpMenuProfileName[cchMultiByte + 3] = 'u';
				lpMenuProfileName[cchMultiByte + 4] = '.';
				lpMenuProfileName[cchMultiByte + 5] = 'i';
				lpMenuProfileName[cchMultiByte + 6] = 'n';
				lpMenuProfileName[cchMultiByte + 7] = 'i';
				lpMenuProfileName[cchMultiByte + 8] = '\0';
				if (cchMultiByte < MAX_PATH - 13)
				{
					memcpy(lpProfileName, lpDirectoryPath, cchMultiByte);
					lpProfileName[cchMultiByte     ] = 'S';
					lpProfileName[cchMultiByte +  1] = 'p';
					lpProfileName[cchMultiByte +  2] = 'o';
					lpProfileName[cchMultiByte +  3] = 'i';
					lpProfileName[cchMultiByte +  4] = 'l';
					lpProfileName[cchMultiByte +  5] = 'e';
					lpProfileName[cchMultiByte +  6] = 'r';
					lpProfileName[cchMultiByte +  7] = 'A';
					lpProfileName[cchMultiByte +  8] = 'L';
					lpProfileName[cchMultiByte +  9] = '.';
					lpProfileName[cchMultiByte + 10] = 'i';
					lpProfileName[cchMultiByte + 11] = 'n';
					lpProfileName[cchMultiByte + 12] = 'i';
					lpProfileName[cchMultiByte + 13] = '\0';
				}
			}
			break;
		}
	} while (uLength);
	do	/* do { ... } while (0); */
	{
		if (*lpProfileName)
		{
			char lpBuffer[MAX_PATH];

			GetPrivateProfileStringA("MainModule" , "CRC32", "", lpBuffer, _countof(lpBuffer), lpProfileName);
			if (*lpBuffer)
			{
				ULONG64 ull;
				char    *endptr;

				ull = _strtoui64(lpBuffer, &endptr, 0);
				if (!*endptr && !(ull >> 32))
				{
					crcTarget = (DWORD)ull;
					break;
				}
			}
		}
		crcTarget = 0x2EC74F3D;
	} while (0);

	verbose(VERBOSE_INFO, "_DllMainCRTStartup - begin CRC32FromFileW");
	if (!CRC32FromFileW(lpFileName, &crc))
		return FALSE;
	if (crc != crcTarget)
		return FALSE;
	verbose(VERBOSE_INFO, "_DllMainCRTStartup - end CRC32FromFileW");

	hHeap = GetProcessHeap();
	if (hHeap == NULL)
		return FALSE;
	pHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0, 0);
	if (pHeap == NULL)
		return FALSE;
	if (!SetThreadLocale(MAKELCID(MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN), SORT_JAPANESE_XJIS)))
		return FALSE;
	uLength = GetSystemDirectoryW(lpFileName, _countof(lpFileName));
	if (uLength == 0 || uLength >= _countof(lpFileName))
		return FALSE;
	if (lpFileName[uLength - 1] != L'\\')
		lpFileName[uLength++] = L'\\';
	if (uLength >= _countof(lpFileName) - 10)
		return FALSE;
	lpFileName[uLength    ] = L'w';
	lpFileName[uLength + 1] = L'i';
	lpFileName[uLength + 2] = L'n';
	lpFileName[uLength + 3] = L'm';
	lpFileName[uLength + 4] = L'm';
	lpFileName[uLength + 5] = L'.';
	lpFileName[uLength + 6] = L'd';
	lpFileName[uLength + 7] = L'l';
	lpFileName[uLength + 8] = L'l';
	lpFileName[uLength + 9] = L'\0';
	hWinMM = LoadLibraryW(lpFileName);
	if (hWinMM == NULL)
		return FALSE;

	verbose(VERBOSE_INFO, "_DllMainCRTStartup - begin ModifyImportAddressTable");
#if 0
	ModifyImportAddressTable(hEntryModule);
#else
	ModifyImportAddressTable();
#endif
	verbose(VERBOSE_INFO, "_DllMainCRTStartup - end ModifyImportAddressTable");

	LoadComCtl32();

	hMsImg32 = LoadLibraryW(L"msimg32.dll");

#if USE_TOOLTIP
	verbose(VERBOSE_INFO, "_DllMainCRTStartup - begin CreateToolTip");
	CreateToolTip();
	verbose(VERBOSE_INFO, "_DllMainCRTStartup - end CreateToolTip");
#endif

	if (*lpProfileName)
		PluginInitialize(lpDirectoryPath, lpProfileName);

	verbose(VERBOSE_INFO, "_DllMainCRTStartup - begin Attach");

	if (!ModifyCodeSection())
		return FALSE;

	if (!ModifyResourceSection())
		return FALSE;

	verbose(VERBOSE_INFO, "_DllMainCRTStartup - end Attach");

	return TRUE;
}

#if 0
/***********************************************************************
 *      GetImportDescriptor
 */
static __inline PIMAGE_IMPORT_DESCRIPTOR GetImportDescriptor(HMODULE hModule)
{
	PIMAGE_DOS_HEADER lpDosHeader;

	lpDosHeader = (PIMAGE_DOS_HEADER)hModule;
	if (lpDosHeader->e_magic == IMAGE_DOS_SIGNATURE)
	{
		PIMAGE_NT_HEADERS lpNtHeader;

		lpNtHeader = (PIMAGE_NT_HEADERS)((LPBYTE)hModule + lpDosHeader->e_lfanew);
		if (lpNtHeader->Signature == IMAGE_NT_SIGNATURE &&
			lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress != 0 &&
			lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size != 0)
		{
			return (PIMAGE_IMPORT_DESCRIPTOR)(
				(LPBYTE)hModule +
				lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		}
	}
	return NULL;
}

/***********************************************************************
 *      ModifyImportAddressTable
 */
static __inline BOOL ModifyImportAddressTable(HMODULE hEntryModule)
{
	PIMAGE_IMPORT_DESCRIPTOR lpImportDescriptor;

	if (hWinMM == hEntryModule)
		return FALSE;
	lpImportDescriptor = GetImportDescriptor(hEntryModule);
	if (lpImportDescriptor == NULL)
		return FALSE;
	for (; lpImportDescriptor->Name != 0; lpImportDescriptor++)
	{
		LPCSTR            lpFileName;
		PIMAGE_THUNK_DATA lpThunkINT;
		PIMAGE_THUNK_DATA lpThunkIAT;

		lpFileName = (LPCSTR)((LPBYTE)hEntryModule + lpImportDescriptor->Name);
		if ((lpFileName[0] != 'w' && lpFileName[0] != 'W') ||
			(lpFileName[1] != 'i' && lpFileName[1] != 'I') ||
			(lpFileName[2] != 'n' && lpFileName[2] != 'N') ||
			(lpFileName[3] != 'm' && lpFileName[3] != 'M') ||
			(lpFileName[4] != 'm' && lpFileName[4] != 'M') ||
			 lpFileName[5] != '.'                          ||
			(lpFileName[6] != 'd' && lpFileName[6] != 'D') ||
			(lpFileName[7] != 'l' && lpFileName[7] != 'L') ||
			(lpFileName[8] != 'l' && lpFileName[8] != 'L') ||
			 lpFileName[9] != '\0')
			continue;
		lpThunkINT = (PIMAGE_THUNK_DATA)((LPBYTE)hEntryModule + lpImportDescriptor->OriginalFirstThunk);
		lpThunkIAT = (PIMAGE_THUNK_DATA)((LPBYTE)hEntryModule + lpImportDescriptor->FirstThunk);
		for (; lpThunkINT->u1.Function != 0; lpThunkINT++, lpThunkIAT++)
		{
			LPCSTR  lpProcName;
			FARPROC lpFunction;
			DWORD   dwProtect;

			lpProcName = IMAGE_SNAP_BY_ORDINAL(lpThunkINT->u1.AddressOfData) ?
				(LPCSTR)IMAGE_ORDINAL(lpThunkINT->u1.AddressOfData) :
				((PIMAGE_IMPORT_BY_NAME)((LPBYTE)hEntryModule + lpThunkINT->u1.AddressOfData))->Name;
			lpFunction = GetProcAddress(hWinMM, lpProcName);
			if (lpFunction == NULL)
				continue;
			if (!VirtualProtect(&lpThunkIAT->u1.Function, sizeof(ULONG_PTR), PAGE_READWRITE, &dwProtect))
				continue;
			lpThunkIAT->u1.Function = (ULONG_PTR)lpFunction;
			VirtualProtect(&lpThunkIAT->u1.Function, sizeof(ULONG_PTR), dwProtect, &dwProtect);
		}
		return TRUE;
	}
	return FALSE;
}
#else
/***********************************************************************
 *      ModifyImportAddressTable
 */
static __inline BOOL ModifyImportAddressTable()
{
	static const LPCSTR lpProcNames[] = {
		(LPCSTR)0x00656D8D,	// "timeBeginPeriod"
		(LPCSTR)0x00656D9F,	// "timeEndPeriod"
		(LPCSTR)0x00656DAF,	// "timeGetDevCaps"
		(LPCSTR)0x00656DC1,	// "timeGetSystemTime"
		(LPCSTR)0x00656DD5,	// "timeKillEvent"
		(LPCSTR)0x00656DE5,	// "timeSetEvent"
	};

	DWORD   dwProtect;
	FARPROC lpProcAddress;

	if (!VirtualProtect((LPVOID)0x00654000, 0x00003000, PAGE_READWRITE, &dwProtect))
		return FALSE;
	for (size_t i = 0; i < _countof(lpProcNames); i++)
		if (lpProcAddress = GetProcAddress(hWinMM, lpProcNames[i]))
			((FARPROC *)0x00654F20)[i] = lpProcAddress;
		else
			return FALSE;
	return VirtualProtect((LPVOID)0x00654000, 0x00003000, PAGE_READONLY, &dwProtect);
}
#endif

/***********************************************************************
 *      ModifyCodeSection
 */
static __inline BOOL ModifyCodeSection()
{
	DWORD dwProtect;

	if (!VirtualProtect((LPVOID)0x00401000, 0x00201000, PAGE_READWRITE, &dwProtect))
		return FALSE;
	Attach_Parsing();
	Attach_AddressNamingAdditionalType();
	Attach_AddressNamingFromFloat();
	FixAdjustByString();
	Attach_EnumReadSSG();
	Attach_FixGetModuleFromName();
	Attach_AddEntryModule();
	Attach_FixToggleByteArray();
	Attach_OnSSGCtrlCleared();
	Attach_LoadFromFile();
#if ENABLE_ASMLIB
	OptimizeCRT();
#endif
	OptimizeStringDivision();
	Attach_FixLoopByteArray();
	Attach_FixGetSSGDataFile();
	Attach_FixTraceAndCopy();
	Attach_FixAdjustByValue();
	Attach_FixMainForm();
	Attach_FixRepeat();
	Attach_FixRemoveSpace();
	Attach_StringSubject();
	Attach_FixByteArrayFind();
	Attach_ErrorSkip();
	Attach_ProcessMonitor();
	Attach_AddressNamingFmt();
	Attach_FixSplitElement();
	Attach_NocachedMemoryList();
	Attach_SubjectProperty();
	Attach_RepeatIndex();
	Attach_FormatNameString();
	Attach_FixListFEP();
	Attach_ShowErrorMessage();
	Attach_FixDoubleList();
	OptimizeGuide();
	Attach_CommonList();
	Attach_FixGetDistractionString();
	Attach_ForceFunnel();
	Attach_MinMaxParam();
	Attach_SubjectStringTable();
	OptimizeAllocator();
	Attach_FixFindName();
	Attach_FixClearChild();
	FixMaskBytes();
	Attach_FixSortTitle();
	return VirtualProtect((LPVOID)0x00401000, 0x00201000, PAGE_EXECUTE_READ, &dwProtect);
}

/***********************************************************************
 *      ModifyResourceSection
 */
static __inline BOOL ModifyResourceSection()
{
	DWORD dwProtect;

	if (!VirtualProtect((LPVOID)0x0065B000, 0x00015000, PAGE_READWRITE, &dwProtect))
		return FALSE;

	// TCustomizeForm::Panel_B.OKBBtn.Left
	// 192 -> 213
	*(LPBYTE)0x00660DCB = 0xD5;

	// TCustomizeForm::Panel_B.OKBBtn.Width
	// 93 -> 99
	*(LPBYTE)0x00660DDA = 0x63;

	// TCustomizeForm::Panel_B.CancelBBtn.Left
	// 312 -> 319
	*(LPBYTE)0x00660E15 = 0x3F;

	// TGetSearchRangeForm::Panel_L.GetHeapBtn.Caption
	// "Š“¾" -> "Žæ“¾"
	*(LPWORD)0x00664385 = BSWAP16(0xD653);

	// TGuideForm::REdit
	//__movsb((unsigned char *)0x00664900,
	//	"\x06\x09" "MS Gothic"
	//	"\x09" "Font.Size" "\x03\x0A\x00"// 10pt
	//	"\x08" "ReadOnly"  "\x08"// false
	//	"\x08" "WordWrap", 43);
	*(LPDWORD)0x00664900 = BSWAP16(0x0609) | ((DWORD)BSWAP16('MS') << 16);
	*(LPDWORD)0x00664904 = BSWAP32(' Got');
	*(LPDWORD)0x00664908 = BSWAP24('hic') | (0x09 << 24);
	*(LPDWORD)0x0066490C = BSWAP32('Font');
	*(LPDWORD)0x00664910 = BSWAP32('.Siz');
	*(LPDWORD)0x00664914 = 'e' | (BSWAP24(0x030A00) << 8);
	*(LPDWORD)0x00664918 = 0x08 | (BSWAP24('Rea') << 8);
	*(LPDWORD)0x0066491C = BSWAP32('dOnl');
	*(LPDWORD)0x00664920 = 'y' | ((DWORD)BSWAP16(0x0808) << 8) | ((DWORD)'W' << 24);
	*(LPDWORD)0x00664924 = BSWAP32('ordW');
	*(LPDWORD)0x00664928 = BSWAP16('ra');
	*(LPBYTE )0x0066492A =         'p';

	// TMemorySettingForm::Panel_C.CRCBtn.Caption
	// "Š“¾" -> "Žæ“¾"
	*(LPWORD)0x006673D6 = BSWAP16(0xD653);

	// TProcessAddForm::Panel_T.ReLoadBtn.Caption
	// "Š“¾" -> "Žæ“¾"
	*(LPWORD)0x00667A9C = BSWAP16(0xD653);

	return VirtualProtect((LPVOID)0x0065B000, 0x00015000, PAGE_READONLY, &dwProtect);
}

/***********************************************************************
 *      Detach
 */
static __inline void Detach()
{
	EXTERN_C HMODULE hComCtl32;
	EXTERN_C HMODULE hMsImg32;

	PluginFinalize();
#if USE_TOOLTIP
	DestroyToolTip();
#endif
	if (hMsImg32)
		FreeLibrary(hMsImg32);
	if (hComCtl32)
		FreeLibrary(hComCtl32);
	if (hWinMM)
		FreeLibrary(hWinMM);
	if (pHeap)
		HeapDestroy(pHeap);
}

/***********************************************************************
 *      Export fuctions
 */
#pragma warning(push)
#pragma warning(disable:4273)
__declspec(naked) MMRESULT __stdcall timeBeginPeriod(UINT uPeriod)
{ __asm jmp dword ptr gs:[0x00654F20] }
__declspec(naked) MMRESULT __stdcall timeEndPeriod(UINT uPeriod)
{ __asm jmp dword ptr gs:[0x00654F24] }
__declspec(naked) MMRESULT __stdcall timeGetDevCaps(LPTIMECAPS ptc, UINT cbtc)
{ __asm jmp dword ptr gs:[0x00654F28] }
__declspec(naked) MMRESULT __stdcall timeGetSystemTime(LPMMTIME pmmt, UINT cbmmt)
{ __asm jmp dword ptr gs:[0x00654F2C] }
__declspec(naked) MMRESULT __stdcall timeKillEvent(UINT uTimerID)
{ __asm jmp dword ptr gs:[0x00654F30] }
__declspec(naked) MMRESULT __stdcall timeSetEvent(UINT uDelay, UINT uResolution, LPTIMECALLBACK lpTimeProc, DWORD dwUser, UINT fuEvent)
{ __asm jmp dword ptr gs:[0x00654F34] }
#pragma warning(pop)

