#pragma once

#include <windows.h>
#include "TWinControl.h"

EXTERN_C HMENU __fastcall TMenuItem_GetHandle(TMenuItem *this);
EXTERN_C void __fastcall TMenuItem_SetCaption(TMenuItem *this, LPCSTR lpText);

