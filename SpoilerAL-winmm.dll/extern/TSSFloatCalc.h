#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSFloatCalc
{
	TSSGSubject     parent;
	bcb6_std_string lockStr;
	BYTE            size;
	BYTE            padding3[7];
	double          min;
	double          max;
	BOOLEAN         isBigEndian;
	BYTE            padding4[7];
	bcb6_std_string addressStr;
	bcb6_std_string nowValHeadStr;
	bcb6_std_string nowValFootStr;
} TSSFloatCalc;
#pragma pack(pop)

