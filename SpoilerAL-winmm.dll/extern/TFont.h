#pragma once

#include <windows.h>
#include "TControl.h"

EXTERN_C void __fastcall TFont_Assign(TFont *this, TFont *Source);
EXTERN_C void __fastcall TFont_SetColor(TFont *this, TColor Color);
EXTERN_C int __fastcall TFont_GetHeight(TFont *this);
EXTERN_C void __fastcall TFont_SetHeight(TFont *this, int Height);
EXTERN_C LPCSTR __fastcall TFont_GetName(TFont *this);
EXTERN_C void __fastcall TFont_SetName(TFont *this, LPCSTR Name);
EXTERN_C int __fastcall TFont_GetSize(TFont *this);
EXTERN_C void __fastcall TFont_SetSize(TFont *this, int Size);
EXTERN_C int __fastcall TFont_GetStyle(TFont *this);
EXTERN_C void __fastcall TFont_SetStyle(TFont *this, TFontStyles Style);
EXTERN_C TFontPitch __fastcall TFont_GetPitch(TFont *this);
EXTERN_C void __fastcall TFont_SetPitch(TFont *this, int Pitch);
EXTERN_C TFontCharset __fastcall TFont_GetCharset(TFont *this);
EXTERN_C void __fastcall TFont_SetCharset(TFont *this, TFontCharset Charset);
