#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"

#pragma pack(push, 1)
typedef struct _TSSGSubject
{
	LPVOID          *VTable;
	BOOLEAN         isSeted;
	BYTE            type;
	BYTE            padding1[2];
	bcb6_std_vector *attribute;
	BYTE            status;
	BOOLEAN         isFEP;
	BOOLEAN         evaluateAtRead;
	BYTE            padding2;
	DWORD           propertyIndex;
	bcb6_std_string name;
	bcb6_std_string code;
	bcb6_std_string subjectName;
	const BYTE      *address;
} TSSGSubject;
#pragma pack(pop)

EXTERN_C long __stdcall TSSGSubject_GetSubjectNameTextWidth(TSSGSubject *_this, HDC hDC, struct _TSSGCtrl *SSGCtrl);

#define TSSGSubject_GetAttribute(/*IN TSSGSubject * */_this) \
	((_this)->attribute)

EXTERN_C void(__cdecl *TSSGSubject_GetSubjectName)(bcb6_std_string *Result, TSSGSubject *_this, struct _TSSGCtrl *SSGC);

typedef const bcb6_std_string *(__fastcall *LPFN_TSSGSUBJECT_GETLOCKNAME)(TSSGSubject *_this);
#define TSSGSubject_GetLockName(SSGS) ((LPFN_TSSGSUBJECT_GETLOCKNAME)(SSGS)->VTable[6])(SSGS)

typedef unsigned long(__cdecl *LPFN_TSSGSUBJECT_GETSIZE)(TSSGSubject *_this);
#define TSSGSubject_GetSize(SSGS) ((LPFN_TSSGSUBJECT_GETSIZE)(SSGS)->VTable[7])(SSGS)

typedef const bcb6_std_string *(__fastcall *LPFN_TSSGSUBJECT_GETADDRESSSTR)(TSSGSubject *_this);
#define TSSGSubject_GetAddressStr(SSGS) ((LPFN_TSSGSUBJECT_GETADDRESSSTR)(SSGS)->VTable[8])(SSGS)
