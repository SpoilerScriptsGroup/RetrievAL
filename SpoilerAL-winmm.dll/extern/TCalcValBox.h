#pragma once

#include <windows.h>
#include "TSSArg.h"

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

#define typename TCalcValBox
#include "bcb6_std_vector_template.h"
