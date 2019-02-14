#pragma once

#include "bcb6_std_vector.h"
#include "TProcessAccessElementBase.h"

typedef enum ProcessAccessElementCompareType {
	ctEQUOL       = 0,
	ctNOT         = 1,
	ctLOWER       = 2,
	ctUPPER       = 3,
	ctLOWER_EQUOL = 4,
	ctUPPER_EQUOL = 5,
} PAEcmpTp;

typedef enum ProcessAccessElementValueOption {
	voNONE           = 0,
	voREVERSE_ENDIAN = 1,
} PAEvalOp;

typedef struct _TProcessAccessElementDataBase {
	TProcessAccessElementBase super;
	PAEcmpTp                  compareType;
	PAEvalOp                  valueOption;
} TProcessAccessElementDataBase;

typedef struct _TProcessAccessElementData {
	union {
		struct {
			LPVOID *VTable;
			PAEtype type;
		};
		TProcessAccessElementDataBase super;
	};
	bcb6_std_vector_byte data;
} TProcessAccessElementData;

#define TProcessAccessElementData_GetData(PAED) &(PAED)->data

typedef struct _TProcessAccessElementMaskData {
	union {
		struct {
			LPVOID *VTable;
			PAEtype type;
		};// Real super class is TProcessAccessElementData
		TProcessAccessElementDataBase super;
	};
	bcb6_std_vector_byte data;
	bcb6_std_vector_byte mask;
} TProcessAccessElementMaskData;

#define TProcessAccessElementMaskData_GetMask(PAEM) &(PAEM)->mask
