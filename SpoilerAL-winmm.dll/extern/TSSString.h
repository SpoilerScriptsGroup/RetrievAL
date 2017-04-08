#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSString
{
	TSSGSubject     parent;
	bcb6_std_string lockStr;
	unsigned long   size;
	BOOLEAN         isUnicode;
	BYTE            padding3[3];
	bcb6_std_string addressStr;
	bcb6_std_string endWord;
	bcb6_std_string nowValHeadStr;
} TSSString;
#pragma pack(pop)

