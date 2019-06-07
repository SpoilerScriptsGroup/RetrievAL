#pragma once

#include <windows.h>
#include "bcb6_std_string.h"

typedef struct
{
	LPVOID          *VTable;
	BYTE            padding1[748];
	LPVOID          Panel_T;
	LPVOID          REdit;
	LPVOID          Panel_B;
	LPVOID          UserModeCmbBox;
	bcb6_std_string helpPath;

} TGuideForm;

#define GUIDE_IS_CLEAR    0x01
#define GUIDE_IS_NOT_LINE 0x02

EXTERN_C void __cdecl TGuideForm_Guide(TGuideForm *this, const char *Mes, int Flags);

