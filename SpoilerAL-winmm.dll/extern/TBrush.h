#pragma once

#include <windows.h>
#include "TControl.h"
#include "TCanvas.h"

typedef unsigned char TBrushStyle;

EXTERN_C TBrushStyle __fastcall TBrush_GetStyle(TBrush *this);
EXTERN_C void __fastcall TBrush_SetStyle(TBrush *this, TBrushStyle Style);
EXTERN_C COLORREF __fastcall TBrush_GetColor(TBrush *this);
EXTERN_C void __fastcall TBrush_SetColor(TBrush *this, COLORREF Color);
