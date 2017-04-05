#pragma once

#include <windows.h>
#include "bcb6_std_list.h"
#include "bcb6_std_vector.h"
#include "bcb6_std_map.h"

typedef struct
{
	bcb6_std_list   allAtteributeVecList;
	bcb6_std_list   allAtteributeList;
	bcb6_std_map    *attributeSetMap;
	bcb6_std_list   *nowAttributeList;
	bcb6_std_vector *nowAttributeVec;
	bcb6_std_map    *stackElemMap;
} TSSGAttributeSelector;

#define TSSGAttributeSelector_GetNowAtteributeVec(/*IN TSSGAttributeSelector * */_this) \
	(_this)->nowAttributeVec

EXTERN_C bcb6_std_vector *(__cdecl *TSSGAttributeSelector_MakeNowAttributeVec)(TSSGAttributeSelector *_this);
EXTERN_C void(__cdecl *TSSGAttributeSelector_AddElement)(TSSGAttributeSelector *_this, LPVOID element);

EXTERN_C void __cdecl TSSGAttributeSelector_EraseElement(TSSGAttributeSelector *_this, LPVOID element);

EXTERN_C bcb6_std_vector *(__cdecl *TSSGAttributeSelector_PushElement)(TSSGAttributeSelector *_this, LPVOID element);
EXTERN_C void(__cdecl *TSSGAttributeSelector_PopElement)(TSSGAttributeSelector *_this, LPVOID element);

EXTERN_C void __cdecl TSSGAttributeSelector_PopElementByType(TSSGAttributeSelector *_this, unsigned long type);
EXTERN_C void __cdecl TSSGAttributeSelector_EraseElementByType(TSSGAttributeSelector *_this, unsigned long type);

