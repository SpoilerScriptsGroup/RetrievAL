#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "bcb6_std_map.h"
#include "bcb6_std_set.h"

#define DT_NEST      0x00000001
#define DT_ESCAPE    0x00000002
#define ET_TRIM_L    0x00000004
#define ET_TRIM_R    0x00000008
#define ET_TRIM      (ET_TRIM_L | ET_TRIM_R)
#define ET_REPLACE   0x00000010
#define ET_SOME_EDIT (ET_TRIM | ET_REPLACE)

typedef struct
{
	LPVOID          *VTable;
	bcb6_std_string nestStartTag;
	bcb6_std_string nestEndTag;
	bcb6_std_string escapeTag;
	bcb6_std_map    replaceMap;
	bcb6_std_set    trimSet;
	LPVOID          padding1;

} TStringDivision;

EXTERN_C unsigned long __cdecl TStringDivision_Find(
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option);

EXTERN_C unsigned long __cdecl TStringDivision_FindBack(
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option);

EXTERN_C unsigned long __cdecl TStringDivision_FindByVector(
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_vector *Token,
	unsigned long   *FindElementIndex,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Half(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	unsigned long   Index,
	unsigned long   Option);

EXTERN_C unsigned long TStringDivision_List(
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	bcb6_std_vector *List,
	unsigned long   Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Editing(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	unsigned long   Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Remove(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	char            *Dest,
	unsigned long   Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_RemoveByMap(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_map    *ReplaceMap,
	unsigned long   Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Trim(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_set    *TrimSet,
	unsigned long   Option);

EXTERN_C void(__cdecl *TStringDivision_ToString)(bcb6_std_string *Result, unsigned long Src, const char *Format);
EXTERN_C void(__cdecl *TStringDivision_ToStringDouble)(bcb6_std_string *Result, double Src, const char *Format);

EXTERN_C BOOLEAN __cdecl TStringDivision_WCList(
	TStringDivision *_this,
	bcb6_std_string *Val1,
	bcb6_std_string *Val2,
	bcb6_std_vector *List1,
	bcb6_std_vector *List2);

