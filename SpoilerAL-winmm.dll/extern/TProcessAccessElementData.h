#pragma once

#include "bcb6_std_vector.h"
#include "TProcessAccessElementBase.h"

typedef struct _TProcessAccessElementData {
	LPVOID              *VTable;
	AccessElementType    type;
	unsigned long        compareType;
	unsigned long        valueOption;
	bcb6_std_vector_byte data;
} TProcessAccessElementData;

#define TProcessAccessElementData_GetData(AElem)\
	(AElem)->data

