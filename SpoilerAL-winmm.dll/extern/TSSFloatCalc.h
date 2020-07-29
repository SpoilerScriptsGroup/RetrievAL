#pragma once

#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSFloatCalc
{
	TSSGSubject     super;
	bcb6_std_string lockStr;
	BYTE            size;
	BYTE            padFloatCalc[7];
	double          min;
	double          max;	
	bool            isBigEndian;
	BYTE            padFloatCalc2[7];
	bcb6_std_string addressStr;
	bcb6_std_string nowValHeadStr;
	bcb6_std_string nowValFootStr;
} TSSFloatCalc;
#pragma pack(pop)

