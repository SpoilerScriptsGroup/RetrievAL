#pragma once

#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSDir
{
	TSSGSubject     parent;
	bcb6_std_vector childVec;
} TSSDir;
#pragma pack(pop)

EXTERN_C void __cdecl TSSDir_ClearChild(TSSDir *this);
EXTERN_C BOOLEAN(__cdecl *TSSDir_IsSameChildren)(TSSDir *this);
