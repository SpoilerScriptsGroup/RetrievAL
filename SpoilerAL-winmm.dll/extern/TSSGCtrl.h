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

typedef LPVOID(__cdecl *LPFN_TSSGCTRL_GETATTRIBUTE)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, unsigned long Type);
#define TSSGCtrl_GetAttribute ((LPFN_TSSGCTRL_GETATTRIBUTE)0x005038E8)

typedef unsigned long(__cdecl *LPFN_TSSGCTRL_ADDRESSATTRIBUTEFILTER)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, DWORD *Address, unsigned long Mode);
#define TSSGCtrl_AddressAttributeFilter ((LPFN_TSSGCTRL_ADDRESSATTRIBUTEFILTER)0x00504130)

typedef unsigned long(__cdecl *LPFN_TSSGCTRL_CHECKIO_FEP)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, unsigned long Val, BOOLEAN IsInput);
#define TSSGCtrl_CheckIO_FEP ((LPFN_TSSGCTRL_CHECKIO_FEP)0x00510F4C)

typedef double(__cdecl *LPFN_TSSGCTRL_CHECKIO_FEPDOUBLE)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, double Val, BOOLEAN IsInput);
#define TSSGCtrl_CheckIO_FEPDouble ((LPFN_TSSGCTRL_CHECKIO_FEPDOUBLE)0x00511094)

typedef unsigned long(__cdecl *LPFN_TSSGCTRL_GETDIRLEVEL)(TSSGCtrl *_this, struct _TSSGSubject *SSGS);
#define TSSGCtrl_GetDirLevel ((LPFN_TSSGCTRL_GETDIRLEVEL)0x00503FA4)

EXTERN_C BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);
EXTERN_C HANDLE __stdcall TSSGCtrl_OpenProcess(LPVOID processCtrl, DWORD Mode, LPCSTR addressStr);
EXTERN_C HANDLE __stdcall TSSGCtrl_Open(TSSGCtrl *_this, struct _TSSGSubject *SSGS, DWORD Mode, LPCSTR addressStr);

typedef bcb6_std_vector *(__cdecl *LPFN_TSSGCTRL_GETSSGDATAFILE)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, bcb6_std_string FName, bcb6_std_string DefaultExt, bcb6_std_string *CurrentDir);
#define TSSGCtrl_GetSSGDataFile ((LPFN_TSSGCTRL_GETSSGDATAFILE)0x004EDFB8)

EXTERN_C bcb6_std_string * __cdecl TSSGCtrl_GetNameString(bcb6_std_string *Result, TSSGCtrl *_this, struct _TSSGSubject *SSGS, const bcb6_std_string *NameStr);

typedef bcb6_std_string *(__cdecl *LPFN_TSSGCTRL_GETSIMPLEBYTECODE)(bcb6_std_string *Result, TSSGCtrl *_this, struct _TSSGSubject *SSGS, bcb6_std_string CodeStr);
#define TSSGCtrl_GetSimpleByteCode ((LPFN_TSSGCTRL_GETSIMPLEBYTECODE)0x00506BAC)

typedef BOOLEAN(__cdecl *LPFN_TSSGCTRL_ONEREAD)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);
#define TSSGCtrl_OneRead ((LPFN_TSSGCTRL_ONEREAD)0x0051C4D8)

typedef BOOLEAN(__cdecl *LPFN_TSSGCTRL_ONEWRITE)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);
#define TSSGCtrl_OneWrite ((LPFN_TSSGCTRL_ONEWRITE)0x0051C540)

