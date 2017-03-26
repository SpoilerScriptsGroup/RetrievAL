#pragma once

#include <windows.h>
#include "TSSGAttributeElement.h"
#include "bcb6_std_string.h"

typedef struct
{
	LPVOID          *VTable;
	DWORD           type;
	bcb6_std_string inputCode;
	bcb6_std_string outputCode;
} TIO_FEPAttribute;

EXTERN_C TIO_FEPAttribute * __cdecl new_TIO_FEPAttribute();
EXTERN_C void __fastcall delete_TIO_FEPAttribute(TIO_FEPAttribute *attribute);

#define TIO_FEPAttribute_GetInputCode(/*IN TIO_FEPAttribute * */attribute) \
	(&(attribute)->inputCode)

#define TIO_FEPAttribute_GetOutputCode(/*IN TIO_FEPAttribute * */attribute) \
	(&(attribute)->outputCode)
