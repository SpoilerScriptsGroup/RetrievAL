#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"

typedef struct
{
	bcb6_std_map    distinctionMap;
	bcb6_std_string filePath;
	unsigned long   sPos;
	unsigned long   ePos;
} TSSGScriptStruct;

EXTERN_C long __stdcall TSSGScriptStruct_GetTitleTextWidth(TSSGScriptStruct *SSGScriptStruct, HDC hDC);

EXTERN_C void(__cdecl * const TSSGScriptStruct_GetDistinction)(bcb6_std_string *str, TSSGScriptStruct *SSGScriptStruct, const char *ID);

#define lpszTitle (LPCSTR)0x006030FF
#define TSSGScriptStruct_GetTitle(str, SSGScriptStruct) TSSGScriptStruct_GetDistinction(str, SSGScriptStruct, lpszTitle)

