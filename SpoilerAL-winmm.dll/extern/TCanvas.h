#pragma once

#include <windows.h>
#include <stdbool.h>

enum TFontStyle
{
	fsBold      = 0x01,
	fsItalic    = 0x02,
	fsUnderline = 0x04,
	fsStrikeOut = 0x08,
};

typedef struct TFontStylesBase
{
	bool fsBold      : 1;
	bool fsItalic    : 1;
	bool fsUnderline : 1;
	bool fsStrikeOut : 1;
} TFSB;

typedef struct _TCanvas
{
	LPVOID *vftable;
	HDC    FHandle;
	BYTE   State;
	BYTE   padding1[3];
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
					TFSB  Style;
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
