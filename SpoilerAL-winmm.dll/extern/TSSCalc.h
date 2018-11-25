#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSCalc
{
	TSSGSubject     super;
	bcb6_std_string lockStr;
	BYTE            size;
	BYTE            padding3[3];
	long            min;
	long            max;
	BOOLEAN         isUnsigned;
	BOOLEAN         isBigEndian;
	BYTE            padding4[2];
	bcb6_std_string addressStr;
	bcb6_std_string nowValHeadStr;
	bcb6_std_string nowValFootStr;
} TSSCalc;
#pragma pack(pop)

EXTERN_C TSSCalc * __cdecl new_TSSCalc();

