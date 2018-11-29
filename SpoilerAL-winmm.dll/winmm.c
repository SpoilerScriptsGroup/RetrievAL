#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "verbose.h"
#include "plugin.h"

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

FARPROC _imp_NONAME0;
FARPROC _imp_CloseDriver;
FARPROC _imp_DefDriverProc;
FARPROC _imp_DriverCallback;
FARPROC _imp_DrvGetModuleHandle;
FARPROC _imp_GetDriverModuleHandle;
FARPROC _imp_MigrateAllDrivers;
FARPROC _imp_MigrateMidiUser;
FARPROC _imp_MigrateSoundEvents;
FARPROC _imp_NotifyCallbackData;
FARPROC _imp_OpenDriver;
FARPROC _imp_PlaySound;
FARPROC _imp_PlaySoundA;
FARPROC _imp_PlaySoundW;
FARPROC _imp_SendDriverMessage;
FARPROC _imp_WOW32DriverCallback;
FARPROC _imp_WOW32ResolveMultiMediaHandle;
FARPROC _imp_WOWAppExit;
FARPROC _imp_WinmmLogoff;
FARPROC _imp_WinmmLogon;
FARPROC _imp_aux32Message;
FARPROC _imp_auxGetDevCapsA;
FARPROC _imp_auxGetDevCapsW;
FARPROC _imp_auxGetNumDevs;
FARPROC _imp_auxGetVolume;
FARPROC _imp_auxOutMessage;
FARPROC _imp_auxSetVolume;
FARPROC _imp_joy32Message;
FARPROC _imp_joyConfigChanged;
FARPROC _imp_joyGetDevCapsA;
FARPROC _imp_joyGetDevCapsW;
FARPROC _imp_joyGetNumDevs;
FARPROC _imp_joyGetPos;
FARPROC _imp_joyGetPosEx;
FARPROC _imp_joyGetThreshold;
FARPROC _imp_joyReleaseCapture;
FARPROC _imp_joySetCapture;
FARPROC _imp_joySetThreshold;
FARPROC _imp_mci32Message;
FARPROC _imp_mciDriverNotify;
FARPROC _imp_mciDriverYield;
FARPROC _imp_mciExecute;
FARPROC _imp_mciFreeCommandResource;
FARPROC _imp_mciGetCreatorTask;
FARPROC _imp_mciGetDeviceIDA;
FARPROC _imp_mciGetDeviceIDFromElementIDA;
FARPROC _imp_mciGetDeviceIDFromElementIDW;
FARPROC _imp_mciGetDeviceIDW;
FARPROC _imp_mciGetDriverData;
FARPROC _imp_mciGetErrorStringA;
FARPROC _imp_mciGetErrorStringW;
FARPROC _imp_mciGetYieldProc;
FARPROC _imp_mciLoadCommandResource;
FARPROC _imp_mciSendCommandA;
FARPROC _imp_mciSendCommandW;
FARPROC _imp_mciSendStringA;
FARPROC _imp_mciSendStringW;
FARPROC _imp_mciSetDriverData;
FARPROC _imp_mciSetYieldProc;
FARPROC _imp_mid32Message;
FARPROC _imp_midiConnect;
FARPROC _imp_midiDisconnect;
FARPROC _imp_midiInAddBuffer;
FARPROC _imp_midiInClose;
FARPROC _imp_midiInGetDevCapsA;
FARPROC _imp_midiInGetDevCapsW;
FARPROC _imp_midiInGetErrorTextA;
FARPROC _imp_midiInGetErrorTextW;
FARPROC _imp_midiInGetID;
FARPROC _imp_midiInGetNumDevs;
FARPROC _imp_midiInMessage;
FARPROC _imp_midiInOpen;
FARPROC _imp_midiInPrepareHeader;
FARPROC _imp_midiInReset;
FARPROC _imp_midiInStart;
FARPROC _imp_midiInStop;
FARPROC _imp_midiInUnprepareHeader;
FARPROC _imp_midiOutCacheDrumPatches;
FARPROC _imp_midiOutCachePatches;
FARPROC _imp_midiOutClose;
FARPROC _imp_midiOutGetDevCapsA;
FARPROC _imp_midiOutGetDevCapsW;
FARPROC _imp_midiOutGetErrorTextA;
FARPROC _imp_midiOutGetErrorTextW;
FARPROC _imp_midiOutGetID;
FARPROC _imp_midiOutGetNumDevs;
FARPROC _imp_midiOutGetVolume;
FARPROC _imp_midiOutLongMsg;
FARPROC _imp_midiOutMessage;
FARPROC _imp_midiOutOpen;
FARPROC _imp_midiOutPrepareHeader;
FARPROC _imp_midiOutReset;
FARPROC _imp_midiOutSetVolume;
FARPROC _imp_midiOutShortMsg;
FARPROC _imp_midiOutUnprepareHeader;
FARPROC _imp_midiStreamClose;
FARPROC _imp_midiStreamOpen;
FARPROC _imp_midiStreamOut;
FARPROC _imp_midiStreamPause;
FARPROC _imp_midiStreamPosition;
FARPROC _imp_midiStreamProperty;
FARPROC _imp_midiStreamRestart;
FARPROC _imp_midiStreamStop;
FARPROC _imp_mixerClose;
FARPROC _imp_mixerGetControlDetailsA;
FARPROC _imp_mixerGetControlDetailsW;
FARPROC _imp_mixerGetDevCapsA;
FARPROC _imp_mixerGetDevCapsW;
FARPROC _imp_mixerGetID;
FARPROC _imp_mixerGetLineControlsA;
FARPROC _imp_mixerGetLineControlsW;
FARPROC _imp_mixerGetLineInfoA;
FARPROC _imp_mixerGetLineInfoW;
FARPROC _imp_mixerGetNumDevs;
FARPROC _imp_mixerMessage;
FARPROC _imp_mixerOpen;
FARPROC _imp_mixerSetControlDetails;
FARPROC _imp_mmDrvInstall;
FARPROC _imp_mmGetCurrentTask;
FARPROC _imp_mmTaskBlock;
FARPROC _imp_mmTaskCreate;
FARPROC _imp_mmTaskSignal;
FARPROC _imp_mmTaskYield;
FARPROC _imp_mmioAdvance;
FARPROC _imp_mmioAscend;
FARPROC _imp_mmioClose;
FARPROC _imp_mmioCreateChunk;
FARPROC _imp_mmioDescend;
FARPROC _imp_mmioFlush;
FARPROC _imp_mmioGetInfo;
FARPROC _imp_mmioInstallIOProcA;
FARPROC _imp_mmioInstallIOProcW;
FARPROC _imp_mmioOpenA;
FARPROC _imp_mmioOpenW;
FARPROC _imp_mmioRead;
FARPROC _imp_mmioRenameA;
FARPROC _imp_mmioRenameW;
FARPROC _imp_mmioSeek;
FARPROC _imp_mmioSendMessage;
FARPROC _imp_mmioSetBuffer;
FARPROC _imp_mmioSetInfo;
FARPROC _imp_mmioStringToFOURCCA;
FARPROC _imp_mmioStringToFOURCCW;
FARPROC _imp_mmioWrite;
FARPROC _imp_mmsystemGetVersion;
FARPROC _imp_mod32Message;
FARPROC _imp_mxd32Message;
FARPROC _imp_sndPlaySoundA;
FARPROC _imp_sndPlaySoundW;
FARPROC _imp_tid32Message;
FARPROC _imp_timeBeginPeriod;
FARPROC _imp_timeEndPeriod;
FARPROC _imp_timeGetDevCaps;
FARPROC _imp_timeGetSystemTime;
FARPROC _imp_timeGetTime;
FARPROC _imp_timeKillEvent;
FARPROC _imp_timeSetEvent;
FARPROC _imp_waveInAddBuffer;
FARPROC _imp_waveInClose;
FARPROC _imp_waveInGetDevCapsA;
FARPROC _imp_waveInGetDevCapsW;
FARPROC _imp_waveInGetErrorTextA;
FARPROC _imp_waveInGetErrorTextW;
FARPROC _imp_waveInGetID;
FARPROC _imp_waveInGetNumDevs;
FARPROC _imp_waveInGetPosition;
FARPROC _imp_waveInMessage;
FARPROC _imp_waveInOpen;
FARPROC _imp_waveInPrepareHeader;
FARPROC _imp_waveInReset;
FARPROC _imp_waveInStart;
FARPROC _imp_waveInStop;
FARPROC _imp_waveInUnprepareHeader;
FARPROC _imp_waveOutBreakLoop;
FARPROC _imp_waveOutClose;
FARPROC _imp_waveOutGetDevCapsA;
FARPROC _imp_waveOutGetDevCapsW;
FARPROC _imp_waveOutGetErrorTextA;
FARPROC _imp_waveOutGetErrorTextW;
FARPROC _imp_waveOutGetID;
FARPROC _imp_waveOutGetNumDevs;
FARPROC _imp_waveOutGetPitch;
FARPROC _imp_waveOutGetPlaybackRate;
FARPROC _imp_waveOutGetPosition;
FARPROC _imp_waveOutGetVolume;
FARPROC _imp_waveOutMessage;
FARPROC _imp_waveOutOpen;
FARPROC _imp_waveOutPause;
FARPROC _imp_waveOutPrepareHeader;
FARPROC _imp_waveOutReset;
FARPROC _imp_waveOutRestart;
FARPROC _imp_waveOutSetPitch;
FARPROC _imp_waveOutSetPlaybackRate;
FARPROC _imp_waveOutSetVolume;
FARPROC _imp_waveOutUnprepareHeader;
FARPROC _imp_waveOutWrite;
FARPROC _imp_wid32Message;
FARPROC _imp_winmmDbgOut;
FARPROC _imp_winmmSetDebugLevel;
FARPROC _imp_wod32Message;

extern HANDLE  hHeap;
extern HANDLE  pHeap;
extern char    lpMenuProfileName[MAX_PATH];
extern HMODULE MsImg32Handle;
HMODULE        hSystemModule = NULL;

__inline PIMAGE_IMPORT_DESCRIPTOR GetImportDescriptor(HMODULE hModule)
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

__inline BOOL ReplaceImportAddressTable(HMODULE hEntryModule)
{
	PIMAGE_IMPORT_DESCRIPTOR lpImportDescriptor;

	if (hSystemModule == hEntryModule)
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
			lpFunction = GetProcAddress(hSystemModule, lpProcName);
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

#if DISABLE_CRT
EXTERN_C void __cdecl __isa_available_init();
#endif
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
EXTERN_C void __cdecl Attach_FixIncomeTypo();
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
EXTERN_C void __cdecl FreeComCtl32();
EXTERN_C void __cdecl Attach_FixClearChild();
EXTERN_C void __cdecl FixMaskBytes();
EXTERN_C void __cdecl Attach_FixSortTitle();

#if DISABLE_CRT
EXTERN_C BOOL WINAPI _DllMainCRTStartup(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
#else
BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
#endif
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			HMODULE hEntryModule;
			wchar_t lpFileName[MAX_PATH];
			UINT    uLength;
			char    lpDirectoryPath[MAX_PATH];
			char    lpProfileName[MAX_PATH];
			char    lpStringBuffer[MAX_PATH];
			DWORD   crcTarget;
			DWORD   crc;
			DWORD   dwProtect;

			init_verbose(hInstance);
			verbose(VERBOSE_INFO, "_DllMainCRTStartup - DLL_PROCESS_ATTACH");

#if DISABLE_CRT
			__isa_available_init();
#endif
			if (!SetThreadLocale(MAKELCID(MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN), SORT_JAPANESE_XJIS)))
				return FALSE;
			hHeap = GetProcessHeap();
			pHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0, 0);
			if (hHeap == NULL || pHeap == NULL)
				return FALSE;
			hEntryModule = GetModuleHandleW(NULL);
			if (hEntryModule == NULL)
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
			hSystemModule = LoadLibraryW(lpFileName);
			if (hSystemModule == NULL)
				return FALSE;

			verbose(VERBOSE_INFO, "_DllMainCRTStartup - begin ReplaceImportAddressTable");
			ReplaceImportAddressTable(hEntryModule);
			verbose(VERBOSE_INFO, "_DllMainCRTStartup - end ReplaceImportAddressTable");

			_imp_NONAME0                      = GetProcAddress(hSystemModule, MAKEINTRESOURCEA(2)           );
			_imp_CloseDriver                  = GetProcAddress(hSystemModule, "CloseDriver"                 );
			_imp_DefDriverProc                = GetProcAddress(hSystemModule, "DefDriverProc"               );
			_imp_DriverCallback               = GetProcAddress(hSystemModule, "DriverCallback"              );
			_imp_DrvGetModuleHandle           = GetProcAddress(hSystemModule, "DrvGetModuleHandle"          );
			_imp_GetDriverModuleHandle        = GetProcAddress(hSystemModule, "GetDriverModuleHandle"       );
			_imp_MigrateAllDrivers            = GetProcAddress(hSystemModule, "MigrateAllDrivers"           );
			_imp_MigrateMidiUser              = GetProcAddress(hSystemModule, "MigrateMidiUser"             );
			_imp_MigrateSoundEvents           = GetProcAddress(hSystemModule, "MigrateSoundEvents"          );
			_imp_NotifyCallbackData           = GetProcAddress(hSystemModule, "NotifyCallbackData"          );
			_imp_OpenDriver                   = GetProcAddress(hSystemModule, "OpenDriver"                  );
			_imp_PlaySound                    = GetProcAddress(hSystemModule, "PlaySound"                   );
			_imp_PlaySoundA                   = GetProcAddress(hSystemModule, "PlaySoundA"                  );
			_imp_PlaySoundW                   = GetProcAddress(hSystemModule, "PlaySoundW"                  );
			_imp_SendDriverMessage            = GetProcAddress(hSystemModule, "SendDriverMessage"           );
			_imp_WOW32DriverCallback          = GetProcAddress(hSystemModule, "WOW32DriverCallback"         );
			_imp_WOW32ResolveMultiMediaHandle = GetProcAddress(hSystemModule, "WOW32ResolveMultiMediaHandle");
			_imp_WOWAppExit                   = GetProcAddress(hSystemModule, "WOWAppExit"                  );
			_imp_WinmmLogoff                  = GetProcAddress(hSystemModule, "WinmmLogoff"                 );
			_imp_WinmmLogon                   = GetProcAddress(hSystemModule, "WinmmLogon"                  );
			_imp_aux32Message                 = GetProcAddress(hSystemModule, "aux32Message"                );
			_imp_auxGetDevCapsA               = GetProcAddress(hSystemModule, "auxGetDevCapsA"              );
			_imp_auxGetDevCapsW               = GetProcAddress(hSystemModule, "auxGetDevCapsW"              );
			_imp_auxGetNumDevs                = GetProcAddress(hSystemModule, "auxGetNumDevs"               );
			_imp_auxGetVolume                 = GetProcAddress(hSystemModule, "auxGetVolume"                );
			_imp_auxOutMessage                = GetProcAddress(hSystemModule, "auxOutMessage"               );
			_imp_auxSetVolume                 = GetProcAddress(hSystemModule, "auxSetVolume"                );
			_imp_joy32Message                 = GetProcAddress(hSystemModule, "joy32Message"                );
			_imp_joyConfigChanged             = GetProcAddress(hSystemModule, "joyConfigChanged"            );
			_imp_joyGetDevCapsA               = GetProcAddress(hSystemModule, "joyGetDevCapsA"              );
			_imp_joyGetDevCapsW               = GetProcAddress(hSystemModule, "joyGetDevCapsW"              );
			_imp_joyGetNumDevs                = GetProcAddress(hSystemModule, "joyGetNumDevs"               );
			_imp_joyGetPos                    = GetProcAddress(hSystemModule, "joyGetPos"                   );
			_imp_joyGetPosEx                  = GetProcAddress(hSystemModule, "joyGetPosEx"                 );
			_imp_joyGetThreshold              = GetProcAddress(hSystemModule, "joyGetThreshold"             );
			_imp_joyReleaseCapture            = GetProcAddress(hSystemModule, "joyReleaseCapture"           );
			_imp_joySetCapture                = GetProcAddress(hSystemModule, "joySetCapture"               );
			_imp_joySetThreshold              = GetProcAddress(hSystemModule, "joySetThreshold"             );
			_imp_mci32Message                 = GetProcAddress(hSystemModule, "mci32Message"                );
			_imp_mciDriverNotify              = GetProcAddress(hSystemModule, "mciDriverNotify"             );
			_imp_mciDriverYield               = GetProcAddress(hSystemModule, "mciDriverYield"              );
			_imp_mciExecute                   = GetProcAddress(hSystemModule, "mciExecute"                  );
			_imp_mciFreeCommandResource       = GetProcAddress(hSystemModule, "mciFreeCommandResource"      );
			_imp_mciGetCreatorTask            = GetProcAddress(hSystemModule, "mciGetCreatorTask"           );
			_imp_mciGetDeviceIDA              = GetProcAddress(hSystemModule, "mciGetDeviceIDA"             );
			_imp_mciGetDeviceIDFromElementIDA = GetProcAddress(hSystemModule, "mciGetDeviceIDFromElementIDA");
			_imp_mciGetDeviceIDFromElementIDW = GetProcAddress(hSystemModule, "mciGetDeviceIDFromElementIDW");
			_imp_mciGetDeviceIDW              = GetProcAddress(hSystemModule, "mciGetDeviceIDW"             );
			_imp_mciGetDriverData             = GetProcAddress(hSystemModule, "mciGetDriverData"            );
			_imp_mciGetErrorStringA           = GetProcAddress(hSystemModule, "mciGetErrorStringA"          );
			_imp_mciGetErrorStringW           = GetProcAddress(hSystemModule, "mciGetErrorStringW"          );
			_imp_mciGetYieldProc              = GetProcAddress(hSystemModule, "mciGetYieldProc"             );
			_imp_mciLoadCommandResource       = GetProcAddress(hSystemModule, "mciLoadCommandResource"      );
			_imp_mciSendCommandA              = GetProcAddress(hSystemModule, "mciSendCommandA"             );
			_imp_mciSendCommandW              = GetProcAddress(hSystemModule, "mciSendCommandW"             );
			_imp_mciSendStringA               = GetProcAddress(hSystemModule, "mciSendStringA"              );
			_imp_mciSendStringW               = GetProcAddress(hSystemModule, "mciSendStringW"              );
			_imp_mciSetDriverData             = GetProcAddress(hSystemModule, "mciSetDriverData"            );
			_imp_mciSetYieldProc              = GetProcAddress(hSystemModule, "mciSetYieldProc"             );
			_imp_mid32Message                 = GetProcAddress(hSystemModule, "mid32Message"                );
			_imp_midiConnect                  = GetProcAddress(hSystemModule, "midiConnect"                 );
			_imp_midiDisconnect               = GetProcAddress(hSystemModule, "midiDisconnect"              );
			_imp_midiInAddBuffer              = GetProcAddress(hSystemModule, "midiInAddBuffer"             );
			_imp_midiInClose                  = GetProcAddress(hSystemModule, "midiInClose"                 );
			_imp_midiInGetDevCapsA            = GetProcAddress(hSystemModule, "midiInGetDevCapsA"           );
			_imp_midiInGetDevCapsW            = GetProcAddress(hSystemModule, "midiInGetDevCapsW"           );
			_imp_midiInGetErrorTextA          = GetProcAddress(hSystemModule, "midiInGetErrorTextA"         );
			_imp_midiInGetErrorTextW          = GetProcAddress(hSystemModule, "midiInGetErrorTextW"         );
			_imp_midiInGetID                  = GetProcAddress(hSystemModule, "midiInGetID"                 );
			_imp_midiInGetNumDevs             = GetProcAddress(hSystemModule, "midiInGetNumDevs"            );
			_imp_midiInMessage                = GetProcAddress(hSystemModule, "midiInMessage"               );
			_imp_midiInOpen                   = GetProcAddress(hSystemModule, "midiInOpen"                  );
			_imp_midiInPrepareHeader          = GetProcAddress(hSystemModule, "midiInPrepareHeader"         );
			_imp_midiInReset                  = GetProcAddress(hSystemModule, "midiInReset"                 );
			_imp_midiInStart                  = GetProcAddress(hSystemModule, "midiInStart"                 );
			_imp_midiInStop                   = GetProcAddress(hSystemModule, "midiInStop"                  );
			_imp_midiInUnprepareHeader        = GetProcAddress(hSystemModule, "midiInUnprepareHeader"       );
			_imp_midiOutCacheDrumPatches      = GetProcAddress(hSystemModule, "midiOutCacheDrumPatches"     );
			_imp_midiOutCachePatches          = GetProcAddress(hSystemModule, "midiOutCachePatches"         );
			_imp_midiOutClose                 = GetProcAddress(hSystemModule, "midiOutClose"                );
			_imp_midiOutGetDevCapsA           = GetProcAddress(hSystemModule, "midiOutGetDevCapsA"          );
			_imp_midiOutGetDevCapsW           = GetProcAddress(hSystemModule, "midiOutGetDevCapsW"          );
			_imp_midiOutGetErrorTextA         = GetProcAddress(hSystemModule, "midiOutGetErrorTextA"        );
			_imp_midiOutGetErrorTextW         = GetProcAddress(hSystemModule, "midiOutGetErrorTextW"        );
			_imp_midiOutGetID                 = GetProcAddress(hSystemModule, "midiOutGetID"                );
			_imp_midiOutGetNumDevs            = GetProcAddress(hSystemModule, "midiOutGetNumDevs"           );
			_imp_midiOutGetVolume             = GetProcAddress(hSystemModule, "midiOutGetVolume"            );
			_imp_midiOutLongMsg               = GetProcAddress(hSystemModule, "midiOutLongMsg"              );
			_imp_midiOutMessage               = GetProcAddress(hSystemModule, "midiOutMessage"              );
			_imp_midiOutOpen                  = GetProcAddress(hSystemModule, "midiOutOpen"                 );
			_imp_midiOutPrepareHeader         = GetProcAddress(hSystemModule, "midiOutPrepareHeader"        );
			_imp_midiOutReset                 = GetProcAddress(hSystemModule, "midiOutReset"                );
			_imp_midiOutSetVolume             = GetProcAddress(hSystemModule, "midiOutSetVolume"            );
			_imp_midiOutShortMsg              = GetProcAddress(hSystemModule, "midiOutShortMsg"             );
			_imp_midiOutUnprepareHeader       = GetProcAddress(hSystemModule, "midiOutUnprepareHeader"      );
			_imp_midiStreamClose              = GetProcAddress(hSystemModule, "midiStreamClose"             );
			_imp_midiStreamOpen               = GetProcAddress(hSystemModule, "midiStreamOpen"              );
			_imp_midiStreamOut                = GetProcAddress(hSystemModule, "midiStreamOut"               );
			_imp_midiStreamPause              = GetProcAddress(hSystemModule, "midiStreamPause"             );
			_imp_midiStreamPosition           = GetProcAddress(hSystemModule, "midiStreamPosition"          );
			_imp_midiStreamProperty           = GetProcAddress(hSystemModule, "midiStreamProperty"          );
			_imp_midiStreamRestart            = GetProcAddress(hSystemModule, "midiStreamRestart"           );
			_imp_midiStreamStop               = GetProcAddress(hSystemModule, "midiStreamStop"              );
			_imp_mixerClose                   = GetProcAddress(hSystemModule, "mixerClose"                  );
			_imp_mixerGetControlDetailsA      = GetProcAddress(hSystemModule, "mixerGetControlDetailsA"     );
			_imp_mixerGetControlDetailsW      = GetProcAddress(hSystemModule, "mixerGetControlDetailsW"     );
			_imp_mixerGetDevCapsA             = GetProcAddress(hSystemModule, "mixerGetDevCapsA"            );
			_imp_mixerGetDevCapsW             = GetProcAddress(hSystemModule, "mixerGetDevCapsW"            );
			_imp_mixerGetID                   = GetProcAddress(hSystemModule, "mixerGetID"                  );
			_imp_mixerGetLineControlsA        = GetProcAddress(hSystemModule, "mixerGetLineControlsA"       );
			_imp_mixerGetLineControlsW        = GetProcAddress(hSystemModule, "mixerGetLineControlsW"       );
			_imp_mixerGetLineInfoA            = GetProcAddress(hSystemModule, "mixerGetLineInfoA"           );
			_imp_mixerGetLineInfoW            = GetProcAddress(hSystemModule, "mixerGetLineInfoW"           );
			_imp_mixerGetNumDevs              = GetProcAddress(hSystemModule, "mixerGetNumDevs"             );
			_imp_mixerMessage                 = GetProcAddress(hSystemModule, "mixerMessage"                );
			_imp_mixerOpen                    = GetProcAddress(hSystemModule, "mixerOpen"                   );
			_imp_mixerSetControlDetails       = GetProcAddress(hSystemModule, "mixerSetControlDetails"      );
			_imp_mmDrvInstall                 = GetProcAddress(hSystemModule, "mmDrvInstall"                );
			_imp_mmGetCurrentTask             = GetProcAddress(hSystemModule, "mmGetCurrentTask"            );
			_imp_mmTaskBlock                  = GetProcAddress(hSystemModule, "mmTaskBlock"                 );
			_imp_mmTaskCreate                 = GetProcAddress(hSystemModule, "mmTaskCreate"                );
			_imp_mmTaskSignal                 = GetProcAddress(hSystemModule, "mmTaskSignal"                );
			_imp_mmTaskYield                  = GetProcAddress(hSystemModule, "mmTaskYield"                 );
			_imp_mmioAdvance                  = GetProcAddress(hSystemModule, "mmioAdvance"                 );
			_imp_mmioAscend                   = GetProcAddress(hSystemModule, "mmioAscend"                  );
			_imp_mmioClose                    = GetProcAddress(hSystemModule, "mmioClose"                   );
			_imp_mmioCreateChunk              = GetProcAddress(hSystemModule, "mmioCreateChunk"             );
			_imp_mmioDescend                  = GetProcAddress(hSystemModule, "mmioDescend"                 );
			_imp_mmioFlush                    = GetProcAddress(hSystemModule, "mmioFlush"                   );
			_imp_mmioGetInfo                  = GetProcAddress(hSystemModule, "mmioGetInfo"                 );
			_imp_mmioInstallIOProcA           = GetProcAddress(hSystemModule, "mmioInstallIOProcA"          );
			_imp_mmioInstallIOProcW           = GetProcAddress(hSystemModule, "mmioInstallIOProcW"          );
			_imp_mmioOpenA                    = GetProcAddress(hSystemModule, "mmioOpenA"                   );
			_imp_mmioOpenW                    = GetProcAddress(hSystemModule, "mmioOpenW"                   );
			_imp_mmioRead                     = GetProcAddress(hSystemModule, "mmioRead"                    );
			_imp_mmioRenameA                  = GetProcAddress(hSystemModule, "mmioRenameA"                 );
			_imp_mmioRenameW                  = GetProcAddress(hSystemModule, "mmioRenameW"                 );
			_imp_mmioSeek                     = GetProcAddress(hSystemModule, "mmioSeek"                    );
			_imp_mmioSendMessage              = GetProcAddress(hSystemModule, "mmioSendMessage"             );
			_imp_mmioSetBuffer                = GetProcAddress(hSystemModule, "mmioSetBuffer"               );
			_imp_mmioSetInfo                  = GetProcAddress(hSystemModule, "mmioSetInfo"                 );
			_imp_mmioStringToFOURCCA          = GetProcAddress(hSystemModule, "mmioStringToFOURCCA"         );
			_imp_mmioStringToFOURCCW          = GetProcAddress(hSystemModule, "mmioStringToFOURCCW"         );
			_imp_mmioWrite                    = GetProcAddress(hSystemModule, "mmioWrite"                   );
			_imp_mmsystemGetVersion           = GetProcAddress(hSystemModule, "mmsystemGetVersion"          );
			_imp_mod32Message                 = GetProcAddress(hSystemModule, "mod32Message"                );
			_imp_mxd32Message                 = GetProcAddress(hSystemModule, "mxd32Message"                );
			_imp_sndPlaySoundA                = GetProcAddress(hSystemModule, "sndPlaySoundA"               );
			_imp_sndPlaySoundW                = GetProcAddress(hSystemModule, "sndPlaySoundW"               );
			_imp_tid32Message                 = GetProcAddress(hSystemModule, "tid32Message"                );
			_imp_timeBeginPeriod              = GetProcAddress(hSystemModule, "timeBeginPeriod"             );
			_imp_timeEndPeriod                = GetProcAddress(hSystemModule, "timeEndPeriod"               );
			_imp_timeGetDevCaps               = GetProcAddress(hSystemModule, "timeGetDevCaps"              );
			_imp_timeGetSystemTime            = GetProcAddress(hSystemModule, "timeGetSystemTime"           );
			_imp_timeGetTime                  = GetProcAddress(hSystemModule, "timeGetTime"                 );
			_imp_timeKillEvent                = GetProcAddress(hSystemModule, "timeKillEvent"               );
			_imp_timeSetEvent                 = GetProcAddress(hSystemModule, "timeSetEvent"                );
			_imp_waveInAddBuffer              = GetProcAddress(hSystemModule, "waveInAddBuffer"             );
			_imp_waveInClose                  = GetProcAddress(hSystemModule, "waveInClose"                 );
			_imp_waveInGetDevCapsA            = GetProcAddress(hSystemModule, "waveInGetDevCapsA"           );
			_imp_waveInGetDevCapsW            = GetProcAddress(hSystemModule, "waveInGetDevCapsW"           );
			_imp_waveInGetErrorTextA          = GetProcAddress(hSystemModule, "waveInGetErrorTextA"         );
			_imp_waveInGetErrorTextW          = GetProcAddress(hSystemModule, "waveInGetErrorTextW"         );
			_imp_waveInGetID                  = GetProcAddress(hSystemModule, "waveInGetID"                 );
			_imp_waveInGetNumDevs             = GetProcAddress(hSystemModule, "waveInGetNumDevs"            );
			_imp_waveInGetPosition            = GetProcAddress(hSystemModule, "waveInGetPosition"           );
			_imp_waveInMessage                = GetProcAddress(hSystemModule, "waveInMessage"               );
			_imp_waveInOpen                   = GetProcAddress(hSystemModule, "waveInOpen"                  );
			_imp_waveInPrepareHeader          = GetProcAddress(hSystemModule, "waveInPrepareHeader"         );
			_imp_waveInReset                  = GetProcAddress(hSystemModule, "waveInReset"                 );
			_imp_waveInStart                  = GetProcAddress(hSystemModule, "waveInStart"                 );
			_imp_waveInStop                   = GetProcAddress(hSystemModule, "waveInStop"                  );
			_imp_waveInUnprepareHeader        = GetProcAddress(hSystemModule, "waveInUnprepareHeader"       );
			_imp_waveOutBreakLoop             = GetProcAddress(hSystemModule, "waveOutBreakLoop"            );
			_imp_waveOutClose                 = GetProcAddress(hSystemModule, "waveOutClose"                );
			_imp_waveOutGetDevCapsA           = GetProcAddress(hSystemModule, "waveOutGetDevCapsA"          );
			_imp_waveOutGetDevCapsW           = GetProcAddress(hSystemModule, "waveOutGetDevCapsW"          );
			_imp_waveOutGetErrorTextA         = GetProcAddress(hSystemModule, "waveOutGetErrorTextA"        );
			_imp_waveOutGetErrorTextW         = GetProcAddress(hSystemModule, "waveOutGetErrorTextW"        );
			_imp_waveOutGetID                 = GetProcAddress(hSystemModule, "waveOutGetID"                );
			_imp_waveOutGetNumDevs            = GetProcAddress(hSystemModule, "waveOutGetNumDevs"           );
			_imp_waveOutGetPitch              = GetProcAddress(hSystemModule, "waveOutGetPitch"             );
			_imp_waveOutGetPlaybackRate       = GetProcAddress(hSystemModule, "waveOutGetPlaybackRate"      );
			_imp_waveOutGetPosition           = GetProcAddress(hSystemModule, "waveOutGetPosition"          );
			_imp_waveOutGetVolume             = GetProcAddress(hSystemModule, "waveOutGetVolume"            );
			_imp_waveOutMessage               = GetProcAddress(hSystemModule, "waveOutMessage"              );
			_imp_waveOutOpen                  = GetProcAddress(hSystemModule, "waveOutOpen"                 );
			_imp_waveOutPause                 = GetProcAddress(hSystemModule, "waveOutPause"                );
			_imp_waveOutPrepareHeader         = GetProcAddress(hSystemModule, "waveOutPrepareHeader"        );
			_imp_waveOutReset                 = GetProcAddress(hSystemModule, "waveOutReset"                );
			_imp_waveOutRestart               = GetProcAddress(hSystemModule, "waveOutRestart"              );
			_imp_waveOutSetPitch              = GetProcAddress(hSystemModule, "waveOutSetPitch"             );
			_imp_waveOutSetPlaybackRate       = GetProcAddress(hSystemModule, "waveOutSetPlaybackRate"      );
			_imp_waveOutSetVolume             = GetProcAddress(hSystemModule, "waveOutSetVolume"            );
			_imp_waveOutUnprepareHeader       = GetProcAddress(hSystemModule, "waveOutUnprepareHeader"      );
			_imp_waveOutWrite                 = GetProcAddress(hSystemModule, "waveOutWrite"                );
			_imp_wid32Message                 = GetProcAddress(hSystemModule, "wid32Message"                );
			_imp_winmmDbgOut                  = GetProcAddress(hSystemModule, "winmmDbgOut"                 );
			_imp_winmmSetDebugLevel           = GetProcAddress(hSystemModule, "winmmSetDebugLevel"          );
			_imp_wod32Message                 = GetProcAddress(hSystemModule, "wod32Message"                );

			uLength = GetModuleFileNameW(hEntryModule, lpFileName, _countof(lpFileName));
			if (uLength == 0)
				break;
			*lpProfileName = '\0';
			while (uLength--)
			{
				wchar_t c;

				if ((c = lpFileName[uLength]) == L'\\' || c == L'/')
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
			}
			do	/* do { ... } while (0); */
			{
				if (*lpProfileName)
				{
					GetPrivateProfileStringA("MainModule" , "CRC32", "", lpStringBuffer, _countof(lpStringBuffer), lpProfileName);
					if (*lpStringBuffer)
					{
						ULONG64 ull;
						char    *endptr;

						ull = _strtoui64(lpStringBuffer, &endptr, 0);
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
				break;
			if (crc != crcTarget)
				break;
			verbose(VERBOSE_INFO, "_DllMainCRTStartup - end CRC32FromFileW");

			LoadComCtl32();

			MsImg32Handle = LoadLibraryW(L"msimg32.dll");

#if USE_TOOLTIP
			verbose(VERBOSE_INFO, "_DllMainCRTStartup - begin CreateToolTip");
			CreateToolTip();
			verbose(VERBOSE_INFO, "_DllMainCRTStartup - end CreateToolTip");
#endif

			if (*lpProfileName)
				PluginInitialize(lpDirectoryPath, lpProfileName);

			verbose(VERBOSE_INFO, "_DllMainCRTStartup - begin Attach");
			if (!VirtualProtect((LPVOID)0x00401000, 0x00201000, PAGE_READWRITE, &dwProtect))
				break;
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
			Attach_FixIncomeTypo();
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
			VirtualProtect((LPVOID)0x00401000, 0x00201000, dwProtect, &dwProtect);
			if (!VirtualProtect((LPVOID)0x0065B000, 0x00015000, PAGE_READWRITE, &dwProtect))
				break;
			__movsb(0x00664900, "\x06\x09" "MS Gothic"
					"\x09" "Font.Size" "\x03\x0A\x00"// 10pt
					"\x08" "ReadOnly"  "\x08"// false
					"\x08" "WordWrap", 43);
			VirtualProtect((LPVOID)0x0065B000, 0x00015000, dwProtect, &dwProtect);
			verbose(VERBOSE_INFO, "_DllMainCRTStartup - end Attach");
		}
		break;
	case DLL_PROCESS_DETACH:
		verbose(VERBOSE_INFO, "_DllMainCRTStartup - DLL_PROCESS_DETACH");
		PluginFinalize();
#if USE_TOOLTIP
		DestroyToolTip();
#endif
		if (MsImg32Handle)
			FreeLibrary(MsImg32Handle);
		FreeComCtl32();
		FreeLibrary(hSystemModule);
		HeapDestroy(pHeap);
		break;
	}
	return TRUE;
}
