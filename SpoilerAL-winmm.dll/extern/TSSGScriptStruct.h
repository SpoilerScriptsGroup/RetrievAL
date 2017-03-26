#pragma once

#include <windows.h>
#include "bcb6_std_string.h"

typedef struct
{
	LPVOID *VTable;
	BYTE   padding[60];
} TSSGScriptStruct;

EXTERN_C long __stdcall TSSGScriptStruct_GetTitleTextWidth(TSSGScriptStruct *SSGScriptStruct, HDC hDC);

typedef void(__cdecl *LPFN_TSSGSCRIPTSTRUCT_GETDISTINCTION)(bcb6_std_string *str, TSSGScriptStruct *SSGScriptStruct, const char *ID);
#define TSSGScriptStruct_GetDistinction ((LPFN_TSSGSCRIPTSTRUCT_GETDISTINCTION)0x00446158)

#define lpszTitle (LPCSTR)0x006030FF
#define TSSGScriptStruct_GetTitle(str, SSGScriptStruct) TSSGScriptStruct_GetDistinction(str, SSGScriptStruct, lpszTitle)

