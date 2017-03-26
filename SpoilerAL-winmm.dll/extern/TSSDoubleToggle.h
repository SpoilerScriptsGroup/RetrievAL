#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"

#pragma pack(push, 1)
typedef struct _TSSDoubleToggle
{
	LPVOID          *VTable;
	BOOLEAN         isSeted;
	BYTE            type;
	BYTE            padding1[2];
	bcb6_std_vector *attribute;
	BYTE            status;
	BOOLEAN         isFEP;
	BYTE            padding2[6];
	bcb6_std_string name;
	bcb6_std_string code;
	bcb6_std_string subjectName;
	LPVOID          padding3;
	bcb6_std_string lockStr;
	BOOLEAN         isOffset;
	BYTE            padding4[7];
	bcb6_std_vector statusVec;
	bcb6_std_string addressStr;
	bcb6_std_string indexFileName;
	bcb6_std_string dataFileName;
} TSSDoubleToggle;
#pragma pack(pop)

