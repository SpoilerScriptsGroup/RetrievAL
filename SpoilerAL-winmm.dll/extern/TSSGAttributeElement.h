#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"
#include "TStringDivision.h"

typedef LPVOID(__cdecl * const LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL)(LPCVOID elem1, LPCVOID elem2);
#define TSSGAttributeElement_IsEqual(elem1, elem2) (*(*(LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL **)(elem1) + 1))((LPCVOID)(elem1), (LPCVOID)(elem2))

typedef enum AttrType {
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
} AtType;// for debugger

typedef struct
{
	LPVOID *VTable;
	AtType type;
} TSSGAttributeElement;

typedef struct {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement base;
	};
	bcb6_std_string offsetCode;
	bcb6_std_string fileName;
	unsigned long   offsetNum;
	DWORD           padding;
} TReplaceAttribute;

typedef struct {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement base;
	};
	unsigned long status;
	unsigned long checkType;
	unsigned long adjustVal;
	int           elemOrder;// define here for convenience' sake.
} TAdjustmentAttribute;

typedef struct {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TAdjustmentAttribute super;
	};
	bcb6_std_map heapMap;
} THeapAdjustmentAttribute, TScopeAttribute;

EXTERN_C void(__cdecl * const TReplaceAttribute_Setting)(TReplaceAttribute *, TStringDivision *, const char *);

EXTERN_C TSSGAttributeElement * __cdecl new_TSSGAttributeElement();

#define TSSGAttributeElement_GetType(/*IN TSSGAttributeElement * */attribute) \
	(attribute)->type

#define TAdjustmentAttribute_Setting(Attr, SSGC, Code) \
	((void(__cdecl *)(void *, void *, const char *))(Attr)->VTable[3])(Attr, SSGCtrl, Code)
