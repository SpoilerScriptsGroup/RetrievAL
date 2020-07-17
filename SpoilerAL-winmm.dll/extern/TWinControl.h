#pragma once

#include "TControl.h"

#define DEFINE_TWINCONTROL                      \
    DEFINE_TCONTROL;                            \
    BYTE          padding_TWinControl1[168]

#pragma pack(push, 1)
typedef struct _TWinControl
{
	DEFINE_TWINCONTROL;
} TWinControl;
#pragma pack(pop)

EXTERN_C HWND __fastcall TWinControl_GetHandle(LPCVOID WinControl);
