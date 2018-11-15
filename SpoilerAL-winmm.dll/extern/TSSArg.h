#pragma once

#include <windows.h>

typedef enum {
	atUNKNOWN,
	atLONG,
	atLONG_INDEX,
	atBOOL,
	atSTRING,
	atBOOL_VECTOR,
	atDOUBLE,
	atNONE,
	atDIR,
	atSPLIT
} ssgCtrl;

#pragma pack(push, 1)
typedef struct _TSSArg
{
	LPVOID       *VTable;
	unsigned long type;
} TSSArg;
#pragma pack(pop)

typedef void(__cdecl * const LPFN_TSSARG_TOSTRING)(bcb6_std_string *ret, TSSArg *this);
#define TSSArg_ToString(ret, Arg) ((LPFN_TSSARG_TOSTRING)(Arg)->VTable[2])(ret, Arg)

