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
	WORD            fixed;
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

EXTERN_C void __fastcall delete_TSSGSubject(TSSGSubject *SSGS);

EXTERN_C long __stdcall TSSGSubject_GetSubjectNameTextWidth(TSSGSubject *this, HDC hDC, struct _TSSGCtrl *SSGCtrl);

#define TSSGSubject_GetAttribute(/*IN TSSGSubject * */this) \
	((this)->attribute)

EXTERN_C void(__cdecl * const TSSGSubject_GetSubjectName)(bcb6_std_string *Result, TSSGSubject *this, struct _TSSGCtrl *SSGC);

typedef void(__cdecl * const LPFN_TSSGSUBJECT_GETLOCKNAME)(bcb6_std_string *Result, TSSGSubject *this);
#define TSSGSubject_GetLockName(Result, SSGS) ((LPFN_TSSGSUBJECT_GETLOCKNAME)(SSGS)->VTable[6])(Result, SSGS)

typedef unsigned long(__cdecl * const LPFN_TSSGSUBJECT_GETSIZE)(TSSGSubject *this);
#define TSSGSubject_GetSize(SSGS) ((LPFN_TSSGSUBJECT_GETSIZE)(SSGS)->VTable[7])(SSGS)

typedef void(__cdecl * const LPFN_TSSGSUBJECT_GETADDRESSSTR)(bcb6_std_string *Result, TSSGSubject *this);
#define TSSGSubject_GetAddressStr(Result, SSGS) ((LPFN_TSSGSUBJECT_GETADDRESSSTR)(SSGS)->VTable[8])(Result, SSGS)

typedef void(__cdecl * const LPFN_TSSARGLONGINDEXSUBJECT_GETINDEXFILENAME)(bcb6_std_string *Result, TSSGSubject *this);
#define TSSArgLongIndexSubject_GetIndexFileName(Result, SSGS) ((LPFN_TSSARGLONGINDEXSUBJECT_GETINDEXFILENAME)(SSGS)->VTable[11])(Result, SSGS)
#define TSSArgBoolVectorSubject_GetIndexFileName TSSArgLongIndexSubject_GetIndexFileName
