#pragma once

#include <windows.h>

typedef struct _TCanvas
{
	BYTE   padding1[20];
	LPVOID Brush;
	BYTE   padding2[64];
} TCanvas;

EXTERN_C void __fastcall TCanvas_FillRect(LPVOID this, const RECT *rect);
EXTERN_C void __fastcall TCanvas_Lock(LPVOID this);
EXTERN_C void __fastcall TCanvas_Unlock(LPVOID this);
EXTERN_C HDC __fastcall TCanvas_GetHandle(LPVOID this);
EXTERN_C void __fastcall TCanvas_TextOut(LPVOID *this, int X, int Y, LPCSTR Text);
