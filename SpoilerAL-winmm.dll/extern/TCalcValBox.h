#pragma once

#include <windows.h>

typedef struct
{
	LPVOID edit;
	DWORD  frontTColor;
	DWORD  backTColor;
	BYTE   backXOffset;
	BYTE   backYOffset;
	BYTE   frontXOffset;
	BYTE   frontYOffset;
	DWORD  mode;
} TCalcValBox;

typedef enum {
	atUNKNOWN,
	atLONG,
	atLONG_INDEX,
	atBOOL,
	atSTRING,
	atBOOL_VECTOR,
	atDOUBLE,
	atNONE,
	atDIR,
	atSPLIT
} ssgCtrl;
