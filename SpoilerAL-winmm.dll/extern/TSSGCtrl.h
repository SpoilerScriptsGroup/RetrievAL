#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TSSGAttributeSelector.h"
#include "TStringDivision.h"
#include "TSSGSubject.h"
#include "TProcessCtrl.h"

typedef struct _TSSGCtrl
{
	LPVOID                *VTable;
	BYTE                  padding1[28];
	TSSGAttributeSelector attributeSelector;
	LPVOID                adjustmentListner;
	LPVOID                ssgActionListner;
	unsigned long         lockTimerInterval;
	BYTE                  padding2[340];
	TProcessCtrl          processCtrl;
	TStringDivision       strD;
} TSSGCtrl;

#define TSSGCtrl_GetAttributeSelector(/*IN TSSGCtrl * */_this) \
	(&(_this)->attributeSelector)

#define TSSGCtrl_GetSSGActionListner(/*IN TSSGCtrl * */_this) \
	(_this)->ssgActionListner

EXTERN_C LPVOID(__cdecl *TSSGCtrl_GetAttribute)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, unsigned long Type);
EXTERN_C unsigned long(__cdecl *TSSGCtrl_AddressAttributeFilter)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, DWORD *Address, unsigned long Mode);
EXTERN_C unsigned long(__cdecl *TSSGCtrl_CheckIO_FEP)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, unsigned long Val, BOOLEAN IsInput);
EXTERN_C double(__cdecl *TSSGCtrl_CheckIO_FEPDouble)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, double Val, BOOLEAN IsInput);
EXTERN_C unsigned long(__cdecl *TSSGCtrl_GetDirLevel)(TSSGCtrl *_this, struct _TSSGSubject *SSGS);

EXTERN_C BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);
EXTERN_C HANDLE __stdcall TSSGCtrl_OpenProcess(LPVOID processCtrl, DWORD Mode, LPCSTR addressStr);
EXTERN_C HANDLE __stdcall TSSGCtrl_Open(TSSGCtrl *_this, struct _TSSGSubject *SSGS, DWORD Mode, LPCSTR addressStr);

EXTERN_C bcb6_std_vector *(__cdecl *TSSGCtrl_GetSSGDataFile)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, bcb6_std_string FName, bcb6_std_string DefaultExt, bcb6_std_string *CurrentDir);

EXTERN_C bcb6_std_string * __cdecl TSSGCtrl_GetNameString(bcb6_std_string *Result, TSSGCtrl *_this, struct _TSSGSubject *SSGS, const bcb6_std_string *NameStr);

EXTERN_C bcb6_std_string *(__cdecl *TSSGCtrl_GetSimpleByteCode)(bcb6_std_string *Result, TSSGCtrl *_this, struct _TSSGSubject *SSGS, bcb6_std_string CodeStr);
EXTERN_C BOOLEAN(__cdecl *TSSGCtrl_IsChildRWProhibit)(TSSGCtrl *_this, struct _TSSGSubject *SSGS);
EXTERN_C BOOLEAN(__cdecl *TSSGCtrl_OneRead)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);
EXTERN_C BOOLEAN(__cdecl *TSSGCtrl_OneWrite)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);

