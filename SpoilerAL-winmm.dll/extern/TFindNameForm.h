#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"

#pragma pack(push, 1)
typedef struct _TFindNameForm
{
	LPVOID          *VTable;
	BYTE            padding1[748];
	LPVOID          Panel_T;
	LPVOID          FindEdit;
	LPVOID          FindStopBtn;
	LPVOID          FindLBox;
	LPVOID          FindStartBtn;
	LPVOID          FindRGroup;
	LPVOID          IsWordConvertCBox;
	LCID            findLCID;
	long            findMode;
	BYTE            padding2[4];
	bcb6_std_vector findIndexVec;
	bcb6_std_vector findSubjectVec;
	BOOLEAN         isWordConvert;
	BYTE            padding3[7];
} TFindNameForm;
#pragma pack(pop)

EXTERN_C bcb6_std_string * __cdecl TFindNameForm_CnvString(bcb6_std_string *Result, TFindNameForm *this, const bcb6_std_string *Src);

