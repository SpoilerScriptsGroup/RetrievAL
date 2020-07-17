#pragma once

#include <windows.h>
#include "TWinControl.h"

#pragma pack(push, 1)
typedef struct _TApplication
{
	BYTE              padding1[96];
	const TWinControl *Control;
	BYTE              padding2[16];
	unsigned int      HintHidePause;
	unsigned int      HintPause;
	BOOLEAN           HintShortCuts;
	BYTE              padding3[3];
	unsigned int      HintShortPause;
	BYTE              padding4[164];
} TApplication;
#pragma pack(pop)

#define Application (*(LPVOID *)0x0064D0AC)

EXTERN_C void __fastcall TApplication_ProcessMessages(LPVOID _Application);
EXTERN_C void __fastcall TApplication_ActivateHint(TApplication *this, LPPOINT CursorPos);

