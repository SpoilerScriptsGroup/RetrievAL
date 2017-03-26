#pragma once

#include <windows.h>

EXTERN_C void __fastcall TCanvas_Lock(LPVOID Canvas);
EXTERN_C void __fastcall TCanvas_Unlock(LPVOID Canvas);
EXTERN_C HDC __fastcall TCanvas_GetHandle(LPVOID Canvas);
EXTERN_C void __fastcall TCanvas_TextOut(LPVOID *Canvas, int X, int Y, LPCSTR Text);
