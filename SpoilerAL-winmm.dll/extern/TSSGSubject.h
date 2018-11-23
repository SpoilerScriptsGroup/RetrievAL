#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "SubjectStringTable.h"

typedef enum {
	reNO_ERROR      = 0,
	reACCESS_ERROR     ,
	rePOINTER_ERROR    ,
	reOPTION_ERROR     ,
	reINDEX_ERROR      ,
	reNOT_ENABLED      ,
	reOPEN_ERROR       ,
} re;

typedef enum {
	stNONE         = 0,
	stDIR             ,
	stCALC            ,
	stTOGGLE          ,
	stLIST            ,
	stSTRING          ,
	stBITLIST         ,
	stB_TOGGLE        ,
	stB_CALC          ,
	stB_LIST          ,
	stD_LIST          ,
	stTRACE           ,
	stD_TOGGLE        ,
	stCOPY            ,
	stFLOAT_CALC      ,
	stB_FLOAT_CALC    ,
	stSPLIT           ,
	stREPEAT          ,
} st;

#define ssOPEN        0x00000001
#define ssLOCK        0x00000002
#define ssCAN_UNKNOWN 0x00000004

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

#define typename PTSSGSubject
#define type     TSSGSubject *
#include "bcb6_std_vector_template.h"

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

typedef unsigned long(__cdecl * const LPFN_TSSGSUBJECT_GETARGTYPE)(TSSGSubject *this);
#define TSSGSubject_GetArgType(SSGS) ((LPFN_TSSGSUBJECT_GETARGTYPE)(SSGS)->VTable[9])(SSGS)

__inline void TSSGSubject_SetCode(TSSGSubject *this, const char *Val) { LPCSTR s[2] = { Val, Val + strlen(Val) }; this->code.index = SubjectStringTable_insert((bcb6_std_string *)s); }
__inline void TSSGSubject_SetName(TSSGSubject *this, const char *Val) { LPCSTR s[2] = { Val, Val + strlen(Val) }; this->name.index = SubjectStringTable_insert((bcb6_std_string *)s); }
__inline void TSSGSubject_SetSubjectName(TSSGSubject *this, const char *Val) { LPCSTR s[2] = { Val, Val + strlen(Val) }; this->subjectName.index = SubjectStringTable_insert((bcb6_std_string *)s); }
#define TSSGSubject_SetAttribute(/*IN TSSGSubject * */this, /*const char * */Val) \
	((this)->attribute = Val)
__inline void TSSGSubject_SetCanUnknown(TSSGSubject *this, BOOLEAN CanUnknown) { if (CanUnknown) this->status |= ssCAN_UNKNOWN; else this->status &= ~ssCAN_UNKNOWN; }
__inline void TSSGSubject_Setting(TSSGSubject *this) { if (!this->isSeted) { this->subjectName.index = this->name.index; this->isSeted = TRUE; } }

typedef void(__cdecl * const LPFN_TSSARGLONGINDEXSUBJECT_GETINDEXFILENAME)(bcb6_std_string *Result, TSSGSubject *this);
#define TSSArgLongIndexSubject_GetIndexFileName(Result, SSGS) ((LPFN_TSSARGLONGINDEXSUBJECT_GETINDEXFILENAME)(SSGS)->VTable[11])(Result, SSGS)
#define TSSArgBoolVectorSubject_GetIndexFileName TSSArgLongIndexSubject_GetIndexFileName
