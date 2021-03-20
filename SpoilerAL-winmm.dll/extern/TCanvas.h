#pragma once

#include "TControl.h"
#include "bcb6_std_string.h"

typedef struct CanvasState
{
	bool csHandleValid : 1;
	bool csFontValid   : 1;
	bool csPenValid    : 1;
	bool csBrushValid  : 1;
	__int8 : 0;
} TCanvasState;

typedef TObject TPen, TBrush;

#pragma pack(push, 1)
typedef struct Canvas
{
	LPCVOID         *VTable;
	HDC              FHandle;
	TCanvasState     State;
	__int8 : 8;
	__int16 : 16;
	TFont           *FFont;
	TPen            *FPen;
	TBrush          *FBrush;
	TPoint           FPenPos;
	int              FCopyMode;
	__int32 : 32;
	TNotifyEvent     FOnChange;
	TNotifyEvent     FOnChanging;
	CRITICAL_SECTION FLock;
	int              FLockCount;
	int              FTextFlags;
} TCanvas;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct ControlCanvas
{
	TCanvas   super;
	TControl *FControl;
	HDC       FDeviceContext;
	HWND      FWindowHandle;
	__int32 : 32;
} TControlCanvas;
#pragma pack(pop)

EXTERN_C void __fastcall TCanvas_FillRect(TCanvas *this, const RECT *rect);
EXTERN_C void __fastcall TCanvas_Lock(TCanvas *this);
EXTERN_C int __fastcall TCanvas_TextWidth(TCanvas *this, LPCSTR Text);
EXTERN_C int __fastcall TCanvas_TextWidth_with_length(TCanvas *this, LPCSTR Text, size_t length);
EXTERN_C int __fastcall TCanvas_TextWidth_stdstr(TCanvas *this, const bcb6_std_string *Text);
EXTERN_C void __fastcall TCanvas_Unlock(TCanvas *this);
EXTERN_C void __fastcall TCanvas_SetFont(TCanvas *this, TFont *Font);
EXTERN_C void __fastcall TCanvas_SetPen(TCanvas *this, TPen *Pen);
EXTERN_C void __fastcall TCanvas_SetBrush(TCanvas *this, TBrush *Brush);
EXTERN_C HDC  __fastcall TCanvas_GetHandle(TCanvas *this);
EXTERN_C void __stdcall TCanvas_TextOut(TCanvas *this, int X, int Y, LPCSTR Text);
