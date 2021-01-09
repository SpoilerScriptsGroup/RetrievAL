#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include "intrinsic.h"
#include "verbose.h"
#include "plugin.h"
#include "crc32\crc32.h"
#include "HintWindow\HintWindow.h"
#include "ToolTip\ToolTip.h"
#include "OptimizeAllocator.h"

#pragma intrinsic(__rdtsc)

#ifndef _DEBUG
#define DISABLE_CRT   1
#define ENABLE_ASMLIB 1
#endif

#if DISABLE_CRT
#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:msvcrt.lib")
#pragma comment(linker, "/nodefaultlib:oldnames.lib")
#endif

static LPCSTR ExportNames[] = {
	#define NAMED(name, suffix, ordinal, index) #name,
	#define UNNAMED(name, suffix, ordinal, index) MAKEINTRESOURCEA(ordinal),
	#include "export.h"
	NULL,
};
#if 0
extern FARPROC ExportAddresses[_countof(ExportNames) - 1];
#else
static FARPROC ExportAddresses[_countof(ExportNames) - 1];
#endif

HANDLE         hHeap  = NULL;
HANDLE         pHeap  = NULL;
static HMODULE hWinMM = NULL;

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

	static BOOL __cdecl Attach();
	static __inline void Detach();

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
#if DISABLE_CRT
		__isa_available_init();
#endif
		init_verbose(hInstance);
		verbose(VRB_INFO, "_DllMainCRTStartup - DLL_PROCESS_ATTACH");
		return Attach();
	case DLL_PROCESS_DETACH:
		verbose(VRB_INFO, "_DllMainCRTStartup - DLL_PROCESS_DETACH");
		Detach();
		break;
	}
	return TRUE;
}

/***********************************************************************
 *      Attach
 */
static BOOL __cdecl Attach()
{
	EXTERN_C BOOL __cdecl LoadComCtl32();

	EXTERN_C wchar_t lpMenuProfileName[MAX_PATH];
	EXTERN_C HMODULE hDwmAPI;
	EXTERN_C HMODULE hMsImg32;

	static __inline void InitializeExportFunctions();
	static __inline BOOL VerifyEntryModule(const wchar_t *lpModuleName, const wchar_t *lpProfileName);
	static BOOL __cdecl ModifyCodeSection();
	static __inline BOOL ModifyResourceSection();

	wchar_t lpModuleName[MAX_PATH];
	size_t  nLength;
	wchar_t c, *p;
	HMODULE hEntryModule;
	wchar_t lpProfileName[MAX_PATH];
	DWORD   dwErrCode;
	wchar_t lpBuffer[MAX_PATH];

	if (!(nLength = GetSystemDirectoryW(lpModuleName, _countof(lpModuleName))))
		goto LAST_ERROR;
	if (nLength >= _countof(lpModuleName) - 9)
		goto BUFFER_OVERFLOW;
	if ((c = lpModuleName[nLength - 1]) != L'\\' && c != L'/' && c != L':')
		if (nLength < _countof(lpModuleName) - 10)
			lpModuleName[nLength++] = L'\\';
		else
			goto BUFFER_OVERFLOW;
	lpModuleName[nLength    ] = L'w';
	lpModuleName[nLength + 1] = L'i';
	lpModuleName[nLength + 2] = L'n';
	lpModuleName[nLength + 3] = L'm';
	lpModuleName[nLength + 4] = L'm';
	lpModuleName[nLength + 5] = L'.';
	lpModuleName[nLength + 6] = L'd';
	lpModuleName[nLength + 7] = L'l';
	lpModuleName[nLength + 8] = L'l';
	lpModuleName[nLength + 9] = L'\0';
	if (!(hWinMM = LoadLibraryW(lpModuleName)))
		goto LAST_ERROR;
	InitializeExportFunctions();
	if (!(hEntryModule = GetModuleHandleW(L"SpoilerAL.exe")))
		goto LAST_ERROR;
	if (!(nLength = GetModuleFileNameW(hEntryModule, lpModuleName, _countof(lpModuleName))))
		goto LAST_ERROR;
	__movsw(lpProfileName, lpModuleName, nLength + 1);
	if (!PathRenameExtensionW(lpProfileName, L".ini"))
		goto LAST_ERROR;
	p = lpModuleName + nLength;
	do
		if ((c = *(--p)) == L'\\' || c == L'/' || c == L':')
		{
			p++;
			break;
		}
	while (p != lpModuleName);
	nLength = p - lpModuleName;
	if (VerifyEntryModule(lpModuleName, lpProfileName))
	{
		#define lpDirectoryPath lpModuleName

		verbose(VRB_INFO, "_DllMainCRTStartup - begin Attach");
		lpDirectoryPath[nLength] = L'\0';
		if (nLength <= _countof(lpMenuProfileName) - 9)
		{
			__movsw(lpMenuProfileName, lpDirectoryPath, nLength);
			lpMenuProfileName[nLength    ] = L'm';
			lpMenuProfileName[nLength + 1] = L'e';
			lpMenuProfileName[nLength + 2] = L'n';
			lpMenuProfileName[nLength + 3] = L'u';
			lpMenuProfileName[nLength + 4] = L'.';
			lpMenuProfileName[nLength + 5] = L'i';
			lpMenuProfileName[nLength + 6] = L'n';
			lpMenuProfileName[nLength + 7] = L'i';
			lpMenuProfileName[nLength + 8] = L'\0';
		}
		if (!(hHeap = GetProcessHeap()))
			goto LAST_ERROR;
		if (!(pHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0, 0)))
			goto LAST_ERROR;
		if (!SetThreadLocale(MAKELCID(MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN), SORT_JAPANESE_XJIS)))
			goto LAST_ERROR;
		LoadComCtl32();
		hDwmAPI = LoadLibraryW(L"dwmapi.dll");
#if !defined(_WIN32_WINNT) || _WIN32_WINNT < _WIN32_WINNT_NT4
		hMsImg32 = LoadLibraryW(L"msimg32.dll");
#endif
		CreateHintWindow();
#if USE_TOOLTIP
		verbose(VRB_INFO, "_DllMainCRTStartup - begin CreateToolTip");
		CreateToolTip();
		verbose(VRB_INFO, "_DllMainCRTStartup - end CreateToolTip");
#endif
		if (*lpProfileName)
			PluginInitialize(lpDirectoryPath, lpProfileName);
		if (!ModifyCodeSection())
			goto LAST_ERROR;
		if (!ModifyResourceSection())
			goto LAST_ERROR;
		srand((unsigned int)__rdtsc());
		verbose(VRB_INFO, "_DllMainCRTStartup - end Attach");

		#undef lpDirectoryPath
	}
	return TRUE;

BUFFER_OVERFLOW:
	dwErrCode = ERROR_BUFFER_OVERFLOW;
	goto FAILED;

LAST_ERROR:
	dwErrCode = GetLastError();

FAILED:
	if (FormatMessageW(
			FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dwErrCode,
			LANGIDFROMLCID(GetThreadLocale()),
			lpBuffer,
			_countof(lpBuffer),
			NULL))
		MessageBoxW(NULL, lpBuffer, NULL, MB_ICONHAND | MB_TOPMOST);
	return FALSE;
}

/***********************************************************************
 *      InitializeExportFunctions
 */
static __inline void InitializeExportFunctions()
{
	size_t i;

	i = 0;
	do
		ExportAddresses[i] = GetProcAddress(hWinMM, ExportNames[i]);
	while (++i != _countof(ExportAddresses));
}

/***********************************************************************
 *      VerifyEntryModule
 */
static __inline BOOL VerifyEntryModule(const wchar_t *lpModuleName, const wchar_t *lpProfileName)
{
	BOOL   bMatched;
	HANDLE hFile;

	bMatched = FALSE;
	hFile = CreateFileW(
		lpModuleName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwSize;

		do	/* do { ... } while (0); */
		{
			wchar_t lpBuffer[16];

			if (*lpProfileName && GetPrivateProfileStringW(L"MainModule", L"Size", L"", lpBuffer, _countof(lpBuffer), lpProfileName))
			{
				wchar_t *endptr;

				errno = 0;
				dwSize = wcstoul(lpBuffer, &endptr, 0);
				if (!*endptr && !errno)
					break;
			}
			dwSize = 0x0027CC00;
		} while (0);
		if (GetFileSize(hFile, NULL) == dwSize)
		{
			DWORD dwCRC32;

			do	/* do { ... } while (0); */
			{
				wchar_t lpBuffer[16];

				if (*lpProfileName && GetPrivateProfileStringW(L"MainModule", L"CRC32", L"", lpBuffer, _countof(lpBuffer), lpProfileName))
				{
					wchar_t *endptr;

					errno = 0;
					dwCRC32 = wcstoul(lpBuffer, &endptr, 0);
					if (!*endptr && !errno)
						break;
				}
				dwCRC32 = 0x2EC74F3D;
			} while (0);
			if (CRC32FromFileHandle(hFile) == dwCRC32)
				bMatched = TRUE;
		}
		CloseHandle(hFile);
	}
	return bMatched;
}

/***********************************************************************
 *      ModifyCodeSection
 */
static BOOL __cdecl ModifyCodeSection()
{
	EXTERN_C void __cdecl Attach_FixSingleInstance();
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
	EXTERN_C void __cdecl Attach_FixClearChild();
	EXTERN_C void __cdecl FixMaskBytes();
	EXTERN_C void __cdecl Attach_FixTitleSelectForm();
	EXTERN_C void __cdecl Attach_HintWindow();

	DWORD dwProtect;

	if (!VirtualProtect((LPVOID)0x00401000, 0x00201000, PAGE_READWRITE, &dwProtect))
		return FALSE;
	Attach_FixSingleInstance();
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
	Attach_FixTitleSelectForm();
	Attach_HintWindow();
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
	*(LPDWORD)0x00664928 = BSWAP24('rap') | (0x08 << 24);

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
	EXTERN_C HMODULE hDwmAPI;
	EXTERN_C HMODULE hMsImg32;

	if (hWinMM)
	{
		if (pHeap)
		{
			PluginFinalize();
			DestroyHintWindow();
#if USE_TOOLTIP
			DestroyToolTip();
#endif
#if !defined(_WIN32_WINNT) || _WIN32_WINNT < _WIN32_WINNT_NT4
			if (hMsImg32)
				FreeLibrary(hMsImg32);
#endif
			if (hDwmAPI)
				FreeLibrary(hDwmAPI);
			if (hComCtl32)
				FreeLibrary(hComCtl32);
			HeapDestroy(pHeap);
		}
		FreeLibrary(hWinMM);
	}
}

/***********************************************************************
 *      Export functions
 */
#define NAMED(name, suffix, ordinal, index)                    \
__declspec(naked) void __cdecl _exp_##name()                   \
{                                                              \
    __asm   jmp     dword ptr [ExportAddresses + (index) * 4]  \
}
#define UNNAMED NAMED
#include "export.h"

