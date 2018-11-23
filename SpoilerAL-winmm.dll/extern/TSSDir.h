#pragma once

#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSDir
{
	TSSGSubject                  super;
	bcb6_std_vector_PTSSGSubject childVec;
} TSSDir;
#pragma pack(pop)

EXTERN_C void __cdecl TSSDir_ClearChild(TSSDir *this);
EXTERN_C BOOLEAN(__cdecl * const TSSDir_IsSameChildren)(TSSDir *this);
