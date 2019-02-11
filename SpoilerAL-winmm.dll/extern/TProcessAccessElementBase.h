#pragma once

#include "bcb6_std_vector.h"

typedef struct _TProcessAccessElementBase {
	LPVOID       *VTable;
	unsigned long type;
} TProcessAccessElementBase;

#define delete_TProcessAccessElementBase(AElem)\
	((void (__cdecl *)(TProcessAccessElementBase*))(AElem)->VTable[0])(AElem)

#define TProcessAccessElementBase_GetType(AElem)\
	(AElem)->type

#define TProcessAccessElementBase_GetSize(AElem, IsTrueMode)\
	((unsigned long (__cdecl *)(TProcessAccessElementBase*, BOOLEAN))(AElem)->VTable[1])(AElem, IsTrueMode)

#define TProcessAccessElementBase_SetSize(AElem, Val, IsTrueMode)\
	((void (__cdecl *)(TProcessAccessElementBase*, unsigned long, BOOLEAN))(AElem)->VTable[2])(AElem, Val, IsTrueMode)

typedef struct _TProcessAccessElementLoop {
	LPVOID               *VTable;
	unsigned long         type;
	bcb6_std_vector_dword loopVec;
	bcb6_std_vector_dword surplusVec;
	unsigned long         loopCount;
	DWORD                 paddding;
} TProcessAccessElementLoop;
