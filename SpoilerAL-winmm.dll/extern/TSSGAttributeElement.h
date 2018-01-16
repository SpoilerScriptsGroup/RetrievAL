#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"
#include "TStringDivision.h"

typedef LPVOID(__cdecl * const LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL)(LPCVOID elem1, LPCVOID elem2);
#define TSSGAttributeElement_IsEqual(elem1, elem2) (*(*(LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL **)(elem1) + 1))((LPCVOID)(elem1), (LPCVOID)(elem2))

typedef struct
{
	LPVOID *VTable;
	DWORD  type;
} TSSGAttributeElement;

typedef struct {
	union {
		struct {
			LPVOID *VTable;
			DWORD  type;
		};
		TSSGAttributeElement base;
	};
	bcb6_std_string offsetCode;
	bcb6_std_string fileName;
	unsigned long   offsetNum;
} TReplaceAttribute;

typedef struct {
	union {
		struct {
			LPVOID *VTable;
			DWORD  type;
		};
		TSSGAttributeElement base;
	};
	unsigned long status;
	unsigned long checkType;
	unsigned long adjustVal;
} TAdjustmentAttribute;

typedef struct {
	union {
		struct {
			LPVOID *VTable;
			DWORD  type;
		};
		TAdjustmentAttribute super;
	};
	DWORD        padding;
	bcb6_std_map heapMap;
} THeapAdjustmentAttribute, TScopeAttribute;

enum AttrTypes {
	atUNKNOWN,
	atDIR_LEVEL    = 1,
	atADJUST       = 2,
	atREPLACE      = 4,
	atSIZE         = 8,
	atALL          = 15,
	atFUNNEL       = 16,
	atIO_FEP       = 32,
	atE_WITH       = 64,
	atENABLED      = 128,
	atADJUST_CHECK = 256,
	atCHILD_RW     = 512,
	atCAUTION      = 1024,
	atVARIABLE     = 0x0800,
	atDEFINE       = 0x1000,
	atERRORSKIP    = 0x2000,
	atSCOPE        = 0x4000,
};

EXTERN_C void(__cdecl * const TReplaceAttribute_Setting)(TReplaceAttribute *, TStringDivision *, const char *);

EXTERN_C TSSGAttributeElement * __cdecl new_TSSGAttributeElement();

#define TSSGAttributeElement_GetType(/*IN TSSGAttributeElement * */attribute) \
	(attribute)->type

#define TAdjustmentAttribute_Setting(Attr, SSGC, Code) \
	((void(__cdecl *)(void *, void *, const char *))(Attr)->VTable[3])(Attr, SSGCtrl, Code)
