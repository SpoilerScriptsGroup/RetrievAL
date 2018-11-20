#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "TCalcValBox.h"
#define typename TCalcValBox
#include "bcb6_std_vector_template.h"

typedef struct {
	RECT hitRect;       // ボタン矩形
	RECT exclusionRect; // 除外矩形
	BYTE status;        // 押／上トグル形式のボタンは、状態を記録
} TCalcButton;

#define typename TCalcButton
#include "bcb6_std_vector_template.h"

typedef struct
{
	LPVOID                      *VTable;
	BYTE                        padding1[380];
	int                         mouseX;
	int                         mouseY;
	LPVOID                      naturalVal;
	LPVOID                      pushGpc;
	LPVOID                      freeGpc;
	LPVOID                      sprite;
	LPVOID                      drawImage;
	LPVOID                      padding2;
	bcb6_std_vector_TCalcButton btnVec;
	bcb6_std_vector_TCalcValBox valBox;
	BOOLEAN                     isPush;
	long                        mouseNowBtn;
	LPVOID                      calcButtonPushFunc;
	LPVOID                      padding3;
} TCalcImage;

EXTERN_C void(__cdecl * const TCalcImage_SetStatus)(TCalcImage *this, long Index, BYTE Status);
EXTERN_C BYTE(__cdecl * const TCalcImage_GetStatus)(TCalcImage *this, long Index);

void __fastcall TCalcImage_DrawBtn(TCalcImage *this, long No, BOOLEAN IsPress);

EXTERN_C void __fastcall _TCalcImage_FOnMouseUp(TCalcImage *this, LPVOID Sender, int Button, int Y, int X, WORD Shift);
#define TCalcImage_FOnMouseUp(this, Sender, Button, Shift, X, Y) _TCalcImage_FOnMouseUp(this, Sender, Button, Y, X, Shift)

EXTERN_C void __fastcall _TCalcImage_FOnMouseDown(TCalcImage *this, LPVOID Sender, int Button, int Y, int X, WORD Shift);
#define TCalcImage_FOnMouseDown(this, Sender, Button, Shift, X, Y) _TCalcImage_FOnMouseDown(this, Sender, Button, Y, X, Shift)
