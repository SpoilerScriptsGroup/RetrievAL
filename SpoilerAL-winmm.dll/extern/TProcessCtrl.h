#pragma once

#include <windows.h>
#include "tlhelp32fix.h"
#include "bcb6_std_vector_string.h"
#include "THeapListData.h"
#include "TStringDivision.h"

typedef struct
{
	TStringDivision               strD;
	bcb6_std_string               attachedProcessName;
	bcb6_std_vector_string        processNameVec;
	PROCESSENTRY32A               entry;
	bcb6_std_vector_THeapListData heapList;
	bcb6_std_vector               moduleList;
	bcb6_std_vector               threadList;
} TProcessCtrl;

EXTERN_C void(__cdecl * const TProcessCtrl_Clear)(TProcessCtrl *this);
EXTERN_C void(__cdecl * const TProcessCtrl_LoadHeapList)(TProcessCtrl *this);
EXTERN_C void(__cdecl * const TProcessCtrl_LoadModuleList)(TProcessCtrl *this);
EXTERN_C THeapListData *(__cdecl * const TProcessCtrl_GetHeapList)(TProcessCtrl *this, unsigned long ListNo);

EXTERN_C BOOLEAN __cdecl TProcessCtrl_AttachByProcessName(TProcessCtrl *this, bcb6_std_string ProcessName);
EXTERN_C BOOLEAN __cdecl TProcessCtrl_Attach(TProcessCtrl *this);
EXTERN_C LPMODULEENTRY32A __fastcall TProcessCtrl_GetModuleFromName(TProcessCtrl *this, LPCSTR Name);

EXTERN_C HANDLE(__cdecl * const TProcessCtrl_Open)(TProcessCtrl *this, DWORD Mode);

EXTERN_C BOOLEAN(__cdecl * const TProcessCtrl_OneRead)(HANDLE ProcessHandle, DWORD Address, void *Data, DWORD Size);

EXTERN_C unsigned long __cdecl TProcessCtrl_FindProcess(LPVOID this, bcb6_std_string *ProcessName, PROCESSENTRY32A *Entry);

