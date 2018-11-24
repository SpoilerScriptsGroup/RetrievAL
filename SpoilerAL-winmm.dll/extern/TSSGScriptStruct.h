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

EXTERN_C long __stdcall TSSGScriptStruct_GetTitleTextWidth(const TSSGScriptStruct *this, HDC hDC);

void __cdecl TSSGScriptStruct_SetDistinction(const TSSGScriptStruct *this, const char *ID, const char *Val);

EXTERN_C bcb6_std_string *(__cdecl * const TSSGScriptStruct_GetDistinction)(bcb6_std_string *Result, const TSSGScriptStruct *this, const char *ID);

#define TSSGScriptStruct_GetTitle(Result, this) TSSGScriptStruct_GetDistinction(Result, this, /* "title" */(LPCSTR)0x006030FF)

