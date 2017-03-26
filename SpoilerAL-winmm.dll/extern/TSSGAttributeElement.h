#pragma once

#include <windows.h>

typedef LPVOID(__cdecl *LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL)(LPCVOID elem1, LPCVOID elem2);
#define TSSGAttributeElement_IsEqual(elem1, elem2) (*(*(LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL **)(elem1) + 1))((LPCVOID)(elem1), (LPCVOID)(elem2))

typedef struct
{
	LPVOID *VTable;
	DWORD  type;
} TSSGAttributeElement;

EXTERN_C TSSGAttributeElement * __cdecl new_TSSGAttributeElement();

#define TSSGAttributeElement_GetType(/*IN TSSGAttributeElement * */attribute) \
	(attribute)->type

