#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"

#pragma pack(push, 1)
typedef struct
{
	LPVOID          *VTable;
	BOOLEAN         isSeted;
	BYTE            type;
	BYTE            padding1[2];
	bcb6_std_vector *attribute;
	BYTE            status;
	BYTE            padding2[7];
	bcb6_std_string name;
	bcb6_std_string code;
	bcb6_std_string subjectName;
	LPVOID          padding3;
	unsigned long   drawType;
	unsigned long   indentType;
	unsigned char   width;
	unsigned char   height;
	BYTE            padding4[2];
	long            indent;
	bcb6_std_string strParam;
} TSSSplit;
#pragma pack(pop)

