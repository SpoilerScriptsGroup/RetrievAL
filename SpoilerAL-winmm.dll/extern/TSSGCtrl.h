#pragma once

#include <windows.h>
#include "bcb6_std_stack.h"
#include "bcb6_std_vector_string.h"
#include "TStringDivision.h"
#include "TProcessCtrl.h"
#include "TSSGAttributeElement.h"
#include "TSSGAttributeSelector.h"
#include "TSSGScriptStruct.h"

typedef struct _TSSGCtrl
{
	struct _TFunctionTimer {
		unsigned long (*nowTimerFunc)(unsigned long);
		MMRESULT timerID;
		TIMECAPS devCaps;
		unsigned long oldTime, nowTime, interval;
	} funcTimer;
	BYTE                   padding1[4];
	TSSGAttributeSelector  attributeSelector;
	LPVOID                 adjustmentListner;
	LPVOID                 ssgActionListner;
	unsigned long          lockTimerInterval;
	BYTE                   padding2[4];
	bcb6_std_string        ssgDir;
	bcb6_std_string        scriptDir;
	bcb6_std_string        exeDir;
	BOOLEAN                canLockWrite;
	BYTE                   padding3[7];
	bcb6_std_map           lockMap;
	bcb6_std_map           operatorMap;
	bcb6_std_map           stringOperatorMap;
	TSSGScriptStruct       script;
	bcb6_std_vector_string memo;
	bcb6_std_vector_string processNameVec;
	BOOLEAN                isMemoWordWrap;
	BYTE                   padding4[7];
	bcb6_std_map           dataFileMap;
	struct _TSSDir         *rootSubject;
	BYTE                   padding5[4];
	TProcessCtrl           processCtrl;
	TStringDivision        strD;
} TSSGCtrl;

#include "TSSDir.h"

#define TSSGCtrl_GetAttributeSelector(/*IN TSSGCtrl * */this) \
	(&(this)->attributeSelector)

#define TSSGCtrl_GetSSGActionListner(/*IN TSSGCtrl * */this) \
	(this)->ssgActionListner

EXTERN_C void(__cdecl * const TSSGCtrl_LockClear)(TSSGCtrl *this);
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG(
	TSSGCtrl *this, bcb6_std_vector_string *SSGFile, bcb6_std_stack *ParentStack, TDialogAdjustmentAttribute *ADJElem, DWORD RepeatIndex, DWORD OuterRepeat);
EXTERN_C BOOLEAN (__cdecl * const TSSGCtrl_IsEnabled)(TSSGCtrl *this, TSSGSubject *SSGS);
EXTERN_C unsigned long __cdecl TSSGCtrl_MakeSubjectType(const bcb6_std_string *Tag);
EXTERN_C TSSGSubject *(__cdecl * const TSSGCtrl_MakeSubjectClass)(const bcb6_std_string *Tag);
EXTERN_C LPVOID(__cdecl * const TSSGCtrl_MakeAdjustmentClass)(const bcb6_std_string *);
EXTERN_C TSSGSubject *(__cdecl * const TSSGCtrl_GetTargetSubject)(TSSGSubject *SSGS);
EXTERN_C void(__cdecl * const TSSGCtrl_SetSSGDataFile)(TSSGCtrl *this, bcb6_std_vector_string *Data, bcb6_std_string *FileName, BOOLEAN IsSSL);
EXTERN_C BOOLEAN (__cdecl * const TSSGCtrl_IsLocked)(TSSGCtrl *this, TSSGSubject *SSGS);
EXTERN_C bcb6_std_vector_string *(__cdecl * const TSSGCtrl_ReadSSRFile)(bcb6_std_vector_string *Result, TSSGCtrl *this, const bcb6_std_string *Code, bcb6_std_vector_dword *Indices, TSSGSubject *SSGS);
EXTERN_C void(__cdecl * const TSSGCtrl_LoopSSRFile)(TSSGCtrl *this, bcb6_std_vector *FormatVec, bcb6_std_vector *Dest, unsigned long LoopVal);
EXTERN_C LPVOID(__cdecl * const TSSGCtrl_GetAttribute)(TSSGCtrl *this, TSSGSubject *SSGS, unsigned long Type);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_GetAddress)(TSSGCtrl *this, TSSGSubject *SSGS, const bcb6_std_string *AddressStr, unsigned long Mode);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_GetDirLevel)(TSSGCtrl *this, TSSGSubject *SSGS);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_AddressAttributeFilter)(TSSGCtrl *this, TSSGSubject *SSGS, DWORD *Address, unsigned long Mode);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_CheckFunnel)(TSSGCtrl *this, TSSGSubject *SSGS, unsigned long Val);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_Funneling)(TSSGCtrl *this, TSSGSubject *SSGS, const bcb6_std_string *FileName, unsigned long Val);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_CheckIO_FEP)(TSSGCtrl *this, TSSGSubject *SSGS, unsigned long Val, BOOLEAN IsInput);
EXTERN_C double(__cdecl * const TSSGCtrl_CheckIO_FEPDouble)(TSSGCtrl *this, TSSGSubject *SSGS, double Val, BOOLEAN IsInput);

EXTERN_C BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);

EXTERN_C bcb6_std_vector_string *(__cdecl * const TSSGCtrl_GetSSGDataFile)(TSSGCtrl *this, TSSGSubject *SSGS, bcb6_std_string FName, bcb6_std_string DefaultExt, bcb6_std_string *CurrentDir);

EXTERN_C bcb6_std_string * __cdecl TSSGCtrl_GetNameString(bcb6_std_string *Result, TSSGCtrl *this, TSSGSubject *SSGS, const bcb6_std_string *NameStr);

EXTERN_C bcb6_std_string *(__cdecl * const TSSGCtrl_GetSimpleByteCode)(bcb6_std_string *Result, TSSGCtrl *this, TSSGSubject *SSGS, bcb6_std_string CodeStr);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_IsChildRWProhibit)(TSSGCtrl *this, TSSGSubject *SSGS);
HANDLE __cdecl TSSGCtrl_Open(TSSGCtrl *this, TSSGSubject *SSGS, DWORD Mode);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_OneRead)(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_OneWrite)(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);

EXTERN_C void(__cdecl * const TSSGCtrl_SetLock)(TSSGCtrl *this, BOOLEAN IsLock, TSSGSubject *Target, void *Value);
