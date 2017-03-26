#pragma once

#include <windows.h>
#include "TSSGAttributeElement.h"
#include "bcb6_std_string.h"

#define TEndWithAttribute_Procedures (LPVOID *)0x0064030C

typedef struct
{
	LPVOID          *VTable;
	DWORD           type;
	bcb6_std_string code;
} TEndWithAttribute;

EXTERN_C TEndWithAttribute * __cdecl new_TEndWithAttribute();
EXTERN_C void __fastcall delete_TEndWithAttribute(TEndWithAttribute *attribute);

#define TEndWithAttribute_GetCode(/*IN TEndWithAttribute * */attribute) \
	(&(attribute)->code)

#define TEndWithAttribute_Setting(/*IN TEndWithAttribute * */attribute, /*LPCSTR */Code) \
	bcb6_std_string_assign_cstr(&(attribute)->code, Code)
