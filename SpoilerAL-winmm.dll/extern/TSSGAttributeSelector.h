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

#define TSSGAttributeSelector_GetNowAtteributeVec(/*IN TSSGAttributeSelector * */attributeSelector) \
	(attributeSelector)->nowAttributeVec

typedef bcb6_std_vector *(__cdecl *LPFN_TSSGATTRIBUTESELECTOR_MAKENOWATTRIBUTEVEC)(TSSGAttributeSelector *attributeSelector);
#define TSSGAttributeSelector_MakeNowAttributeVec(attributeSelector) \
	((LPFN_TSSGATTRIBUTESELECTOR_MAKENOWATTRIBUTEVEC)0x004D584C)(attributeSelector)

typedef void(__cdecl *LPFN_TSSGATTRIBUTESELECTOR_ADDELEMENT)(TSSGAttributeSelector *attributeSelector, LPVOID element);
#define TSSGAttributeSelector_AddElement(attributeSelector, element) \
	((LPFN_TSSGATTRIBUTESELECTOR_ADDELEMENT)0x004D3DB8)(attributeSelector, element)

#if 0
typedef void(__cdecl *LPFN_TSSGATTRIBUTESELECTOR_ERASEELEMENT)(TSSGAttributeSelector *attributeSelector, LPVOID element);
#define TSSGAttributeSelector_EraseElement(attributeSelector, element) \
	((LPFN_TSSGATTRIBUTESELECTOR_ERASEELEMENT)0x004D40A0)(attributeSelector, element)
#else
EXTERN_C void __cdecl TSSGAttributeSelector_EraseElement(TSSGAttributeSelector *attributeSelector, LPVOID element);
#endif

typedef bcb6_std_vector *(__cdecl *LPFN_TSSGATTRIBUTESELECTOR_PUSHELEMENT)(TSSGAttributeSelector *attributeSelector, LPVOID element);
#define TSSGAttributeSelector_PushElement(attributeSelector, element) \
	((LPFN_TSSGATTRIBUTESELECTOR_PUSHELEMENT)0x004D3CDC)(attributeSelector, element)

typedef void(__cdecl *LPFN_TSSGATTRIBUTESELECTOR_POPELEMENT)(TSSGAttributeSelector *attributeSelector, LPVOID element);
#define TSSGAttributeSelector_PopElement(attributeSelector, element) \
	((LPFN_TSSGATTRIBUTESELECTOR_POPELEMENT)0x004D3E34)(attributeSelector, element)

EXTERN_C void __cdecl TSSGAttributeSelector_PopElementByType(TSSGAttributeSelector *attributeSelector, unsigned long type);
EXTERN_C void __cdecl TSSGAttributeSelector_EraseElementByType(TSSGAttributeSelector *attributeSelector, unsigned long type);

