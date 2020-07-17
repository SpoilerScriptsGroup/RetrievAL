#pragma once

#include <windows.h>

#define DEFINE_TCONTROL                         \
    LPVOID        *VTable;                      \
    BYTE          padding_TControl1[68];        \
    const int     Width;                        \
    const int     Height;                       \
    BYTE          padding_TControl2[7];         \
    const BOOLEAN Visible;                      \
    BYTE          padding_TControl3[40];        \
    LPCSTR        Hint;                         \
    BYTE          padding_TControl4[220]

#pragma pack(push, 1)
typedef struct _TControl
{
	DEFINE_TCONTROL;
} TControl;
#pragma pack(pop)

#define TControl_GetWidth(this) ((TControl *)(this))->Width
#define TControl_GetHeight(this) ((TControl *)(this))->Height
#define TControl_GetVisible(this) ((TControl *)(this))->Visible
