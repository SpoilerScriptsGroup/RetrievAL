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

__inline void TSSGSubject_SetCode(TSSGSubject *this, const char *Val) { LPCSTR s[2] = { Val, Val + strlen(Val) }; this->code.index = SubjectStringTable_insert((bcb6_std_string *)s); }
__inline void TSSGSubject_SetName(TSSGSubject *this, const char *Val) { LPCSTR s[2] = { Val, Val + strlen(Val) }; this->name.index = SubjectStringTable_insert((bcb6_std_string *)s); }
__inline void TSSGSubject_SetSubjectName(TSSGSubject *this, const char *Val) { LPCSTR s[2] = { Val, Val + strlen(Val) }; this->subjectName.index = SubjectStringTable_insert((bcb6_std_string *)s); }
#define TSSGSubject_SetAttribute(/*IN TSSGSubject * */this, /*const char * */Val) ((this)->attribute = Val)
__inline void TSSGSubject_SetCanUnknown(TSSGSubject *this, BOOLEAN CanUnknown) { if (CanUnknown) this->status |= ssCAN_UNKNOWN; else this->status &= ~ssCAN_UNKNOWN; }

// virtual
#define TSSGSubject_Setting(this, SSGC) ((void(__cdecl *)(TSSGSubject *, TSSGCtrl *))(this)->VTable[1])(this, SSGC)
#define TSSGSubject_Read(this, SSGC, Arg) ((unsigned long(__cdecl *)(TSSGSubject *, TSSGCtrl *, TSSArg *))(this)->VTable[2])(this, SSGC, Arg)
#define TSSGSubject_Write(this, SSGC, Arg) ((unsigned long(__cdecl *)(TSSGSubject *, TSSGCtrl *, TSSArg *))(this)->VTable[3])(this, SSGC, Arg)
#define TSSGSubject_ToString(Result, this, SSGC) ((bcb6_std_string *(__cdecl *)(bcb6_std_string *, TSSGSubject *, TSSGCtrl *))(this)->VTable[4])(Result, this, SSGC)
#define TSSGSubject_ToByteCode(Result, this, SSGC, Val) ((bcb6_std_string *(__cdecl *)(bcb6_std_string *, TSSGSubject *, TSSGCtrl *, TSSArg *))(this)->VTable[5])(Result, this, SSGC, Val)
#define TSSGSubject_GetLockName(Result, this) ((bcb6_std_string *(__cdecl *)(bcb6_std_string *, TSSGSubject *))(this)->VTable[6])(Result, this)
#define TSSGSubject_GetSize(this) ((unsigned long(__cdecl *)(TSSGSubject *))(this)->VTable[7])(this)
#define TSSGSubject_GetAddressStr(Result, this) ((bcb6_std_string *(__cdecl *)(bcb6_std_string *, TSSGSubject *))(this)->VTable[8])(Result, this)
#define TSSGSubject_GetArgType(this) ((unsigned long(__cdecl *)(TSSGSubject *))(this)->VTable[9])(this)
#define TSSGSubject_IsSameSubject(this, SSGS) ((BOOLEAN(__cdecl *)(TSSGSubject *, TSSGSubject *))(this)->VTable[10])(this, SSGS)
#define TSSArgLongIndexSubject_GetIndexFileName(Result, SSGS) ((void(__cdecl *)(bcb6_std_string *, TSSGSubject *))(SSGS)->VTable[11])(Result, SSGS)
#define TSSArgBoolVectorSubject_GetIndexFileName TSSArgLongIndexSubject_GetIndexFileName
