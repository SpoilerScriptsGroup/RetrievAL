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

__inline void TSSGSubject_SetCode_stdstr(TSSGSubject *this, const bcb6_std_string *Val) { this->code.sstIndex = SubjectStringTable_insert(Val); }
__inline void TSSGSubject_SetName_stdstr(TSSGSubject *this, const bcb6_std_string *Val) { this->name.sstIndex = SubjectStringTable_insert(Val); }
__inline void TSSGSubject_SetSubjectName_stdstr(TSSGSubject *this, const bcb6_std_string *Val) { this->subjectName.sstIndex = SubjectStringTable_insert(Val); }
__inline void TSSGSubject_SetCode(TSSGSubject *this, const char *Val) { LPCSTR s[2] = { Val, Val + strlen(Val) }; this->code.sstIndex = SubjectStringTable_insert((bcb6_std_string *)s); }
__inline void TSSGSubject_SetName(TSSGSubject *this, const char *Val) { LPCSTR s[2] = { Val, Val + strlen(Val) }; this->name.sstIndex = SubjectStringTable_insert((bcb6_std_string *)s); }
__inline void TSSGSubject_SetSubjectName(TSSGSubject *this, const char *Val) { LPCSTR s[2] = { Val, Val + strlen(Val) }; this->subjectName.sstIndex = SubjectStringTable_insert((bcb6_std_string *)s); }
#define TSSGSubject_SetAttribute(/*IN TSSGSubject * */this, /*const bcb6_std_vector * */Val) ((this)->attribute = Val)
__inline void TSSGSubject_SetCanUnknown(TSSGSubject *this, BOOLEAN CanUnknown) { if (CanUnknown) this->status |= ssCAN_UNKNOWN; else this->status &= ~ssCAN_UNKNOWN; }

#define _TSSGSubject_VTable             0x00617C20
#define _TSSSplit_VTable                0x0063FED8
#define _TSSBundleFloatCalc_VTable      0x0063FF10
#define _TSSFloatCalc_VTable            0x0063FF48
#define _TSSCopy_VTable                 0x0063FF80
#define _TSSTrace_VTable                0x0063FFB8
#define _TSSBitList_VTable              0x0063FFF0
#define _TSSArgBoolVectorSubject_VTable 0x0064002C
#define _TSSDoubleToggle_VTable         0x00640068
#define _TSSBundleCalc_VTable           0x006400A4
#define _TSSBundleToggle_VTable         0x00640118
#define _TSSDoubleList_VTable           0x0064018C
#define _TSSList_VTable                 0x006401C8
#define _TSSString_VTable               0x00640204
#define _TSSToggle_VTable               0x0064023C
#define _TSSCalc_VTable                 0x00640274
#define _TSSDir_VTable                  0x006403C0
#define TSSGSubject_VTable              ((LPVOID *)_TSSGSubject_VTable            )
#define TSSSplit_VTable                 ((LPVOID *)_TSSSplit_VTable               )
#define TSSBundleFloatCalc_VTable       ((LPVOID *)_TSSBundleFloatCalc_VTable     )
#define TSSFloatCalc_VTable             ((LPVOID *)_TSSFloatCalc_VTable           )
#define TSSCopy_VTable                  ((LPVOID *)_TSSCopy_VTable                )
#define TSSTrace_VTable                 ((LPVOID *)_TSSTrace_VTable               )
#define TSSBitList_VTable               ((LPVOID *)_TSSBitList_VTable             )
#define TSSArgBoolVectorSubject_VTable  ((LPVOID *)_TSSArgBoolVectorSubject_VTable)
#define TSSDoubleToggle_VTable          ((LPVOID *)_TSSDoubleToggle_VTable        )
#define TSSBundleCalc_VTable            ((LPVOID *)_TSSBundleCalc_VTable          )
#define TSSBundleToggle_VTable          ((LPVOID *)_TSSBundleToggle_VTable        )
#define TSSDoubleList_VTable            ((LPVOID *)_TSSDoubleList_VTable          )
#define TSSList_VTable                  ((LPVOID *)_TSSList_VTable                )
#define TSSString_VTable                ((LPVOID *)_TSSString_VTable              )
#define TSSToggle_VTable                ((LPVOID *)_TSSToggle_VTable              )
#define TSSCalc_VTable                  ((LPVOID *)_TSSCalc_VTable                )
#define TSSDir_VTable                   ((LPVOID *)_TSSDir_VTable                 )

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
#define TSSArgLongIndexSubject_GetIndexFileName(Result, this) ((void(__cdecl *)(bcb6_std_string *, TSSGSubject *))(this)->VTable[11])(Result, this)
#define TSSArgBoolVectorSubject_GetIndexFileName TSSArgLongIndexSubject_GetIndexFileName
