#pragma once

#include <windows.h>

EXTERN_C HMENU __fastcall TMenuItem_GetHandle(LPCVOID MenuItem);
EXTERN_C void __stdcall TMenuItem_SetCaption(LPVOID MenuItem, LPCSTR lpText);

