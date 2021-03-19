#pragma once

#include <windows.h>
#include "TControl.h"
#include "TCanvas.h"

EXTERN_C COLORREF __fastcall TBrush_GetColor(TBrush *this);
EXTERN_C void __fastcall TBrush_SetColor(TBrush *this, COLORREF color);
