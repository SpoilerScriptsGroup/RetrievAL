#pragma once

#include <windows.h>

#pragma pack(push, 1)
typedef struct
{
	BYTE          padding1[87];
	const BOOLEAN Visible;
	BYTE          padding2[432];
} TWinControl;
#pragma pack(pop)

EXTERN_C HWND __fastcall TWinControl_GetHandle(LPVOID WinControl);
