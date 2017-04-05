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

EXTERN_C THeapListData *(__cdecl *TProcessCtrl_GetHeapList)(TProcessCtrl *_this, unsigned long ListNo);

EXTERN_C LPMODULEENTRY32A __stdcall TProcessCtrl_GetModuleFromName(TProcessCtrl *_this, LPCSTR Name);

EXTERN_C HANDLE(__cdecl *TProcessCtrl_Open)(TProcessCtrl *_this, DWORD Mode);

EXTERN_C BOOLEAN(__cdecl *TProcessCtrl_OneRead)(HANDLE ProcessHandle, DWORD Address, void *Data, DWORD Size);

EXTERN_C void __cdecl TProcessCtrl_LoadHeapList(TProcessCtrl *_this);
EXTERN_C unsigned long __cdecl TProcessCtrl_FindProcess(LPVOID _this, bcb6_std_string *ProcessName, PROCESSENTRY32A *Entry);

