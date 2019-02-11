#pragma once

#include "bcb6_std_vector.h"

typedef enum ProcessAccessElementType {
//	atUNKNOWN           =  0,
	atSKIP              =  1,
	atJUMP              =  2,
	atIF                =  3,
	atDATA              =  4,
	atMASK_DATA         =  5,
	atSIGNED_NUM_DATA   =  6,
	atUNSIGNED_NUM_DATA =  7,
	atLOGICAL_OR        =  8,
	atLOGICAL_AND       =  9,
	atSELECT            = 10,
	atLOOP              = 11,
	atFLOAT_NUM_DATA    = 12,
	atNUM_DATA          = 13,
	atBIT_DATA          = 14,
} AccessElementType;

typedef struct _TProcessAccessElementBase {
	LPVOID           *VTable;
	AccessElementType type;
} TProcessAccessElementBase;

#define delete_TProcessAccessElementBase(AElem)\
	((void (__cdecl *)(void*))(AElem)->VTable[0])(AElem)

#define TProcessAccessElementBase_GetType(AElem)\
	(AElem)->type

#define TProcessAccessElementBase_GetSize(AElem, IsTrueMode)\
	((unsigned long (__cdecl *)(void*, BOOLEAN))(AElem)->VTable[1])(AElem, IsTrueMode)

#define TProcessAccessElementBase_SetSize(AElem, Val, IsTrueMode)\
	((void (__cdecl *)(void*, unsigned long, BOOLEAN))(AElem)->VTable[2])(AElem, Val, IsTrueMode)

typedef struct _TProcessAccessElementLoop {
	LPVOID               *VTable;
	AccessElementType     type;
	bcb6_std_vector_dword loopVec;
	bcb6_std_vector_dword surplusVec;
	unsigned long         loopCount;
	DWORD                 paddding;
} TProcessAccessElementLoop;
