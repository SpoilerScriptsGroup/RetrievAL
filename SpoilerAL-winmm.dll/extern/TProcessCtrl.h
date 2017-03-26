#pragma once

#include <windows.h>
#include "tlhelp32fix.h"
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "THeapListData.h"
#include "TStringDivision.h"

typedef struct
{
	TStringDivision strD;
	bcb6_std_string attachedProcessName;
	bcb6_std_vector processNameVec;
	PROCESSENTRY32A entry;
	bcb6_std_vector heapList;
	bcb6_std_vector moduleList;
	bcb6_std_vector threadList;
} TProcessCtrl;

typedef THeapListData *(__cdecl *LPFN_TPROCESSCTRL_GETHEAPLIST)(LPVOID processCtrl, unsigned long ListNo);
#define TProcessCtrl_GetHeapList ((LPFN_TPROCESSCTRL_GETHEAPLIST)0x004A5A94)

EXTERN_C LPMODULEENTRY32A __stdcall TProcessCtrl_GetModuleFromName(TProcessCtrl *_this, LPCSTR Name);

typedef HANDLE(__cdecl *LPFN_TPROCESSCTRL_OPEN)(TProcessCtrl *_this, DWORD Mode);
#define TProcessCtrl_Open ((LPFN_TPROCESSCTRL_OPEN)0x004A618C)

typedef BOOLEAN(__cdecl *LPFN_TPROCESSCTRL_ONEREAD)(HANDLE ProcessHandle, DWORD Address, void *Data, DWORD Size);
#define TProcessCtrl_OneRead ((LPFN_TPROCESSCTRL_ONEREAD)0x004A6FF4)

EXTERN_C void __cdecl TProcessCtrl_LoadHeapList(TProcessCtrl *_this);
EXTERN_C unsigned long __cdecl TProcessCtrl_FindProcess(LPVOID _this, bcb6_std_string *ProcessName, PROCESSENTRY32A *Entry);

