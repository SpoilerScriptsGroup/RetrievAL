#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "TCalcValBox.h"
#define typename TCalcValBox
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
	bcb6_std_vector             btnVec;
	bcb6_std_vector_TCalcValBox valBox;
	BOOLEAN                     isPush;
	long                        mouseNowBtn;
	LPVOID                      calcButtonPushFunc;
	LPVOID                      padding3;
} TCalcImage;
