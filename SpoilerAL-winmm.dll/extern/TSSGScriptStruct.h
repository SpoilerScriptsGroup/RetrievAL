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

EXTERN_C long __stdcall TSSGScriptStruct_GetTitleTextWidth(const TSSGScriptStruct *SSGScriptStruct, HDC hDC);

EXTERN_C bcb6_std_string *(__cdecl * const TSSGScriptStruct_GetDistinction)(bcb6_std_string *Result, const TSSGScriptStruct *SSGScriptStruct, const char *ID);

#define TSSGScriptStruct_GetTitle(Result, SSGScriptStruct) TSSGScriptStruct_GetDistinction(Result, SSGScriptStruct, /* "title" */(LPCSTR)0x006030FF)

