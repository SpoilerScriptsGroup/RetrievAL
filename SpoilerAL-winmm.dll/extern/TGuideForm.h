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

EXTERN_C void __cdecl TGuideForm_Guide(TGuideForm *this, const char *Mes, BOOLEAN IsClear);

