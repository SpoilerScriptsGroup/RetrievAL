#pragma once

#include <windows.h>

typedef struct _TCanvas
{
	LPVOID *vftable;
	int    FLockCount;
	int    FStartCount;
	struct TFont {
		LPVOID *vftable;
		DWORD  FOnChange[3];
		struct TResource {
			struct TResource *Next;
			int              RefCount;
			unsigned         Handle;
			WORD             HashCode;
			union {
				struct TFontData {
					HFONT Handle;
					int   Height;
					BYTE  TFontPitch;
					BYTE  Style;
				} Font;
			};
		} *FResource;
	}     *FFont;
	LPVOID FPen;
	LPVOID Brush;
	BYTE   padding2[64];
} TCanvas;

EXTERN_C void __fastcall TCanvas_FillRect(LPVOID this, const RECT *rect);
EXTERN_C void __fastcall TCanvas_Lock(LPVOID this);
EXTERN_C void __fastcall TCanvas_Unlock(LPVOID this);
EXTERN_C HDC  __fastcall TCanvas_GetHandle(LPVOID this);
EXTERN_C void __fastcall TCanvas_TextOut(LPVOID *this, int X, int Y, LPCSTR Text);
