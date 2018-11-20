#pragma once

#include "TWinControl.h"

#define DEFINE_TBUTTONCONTROL                   \
    DEFINE_TWINCONTROL;                         \
    BYTE          padding_TButtonControl1[8]

#define DEFINE_TCUSTOMCHECKBOX                  \
    DEFINE_TBUTTONCONTROL;                      \
    BYTE          padding_TCustomCheckBox1[8]

#define DEFINE_TCHECKBOX                        \
    DEFINE_TCUSTOMCHECKBOX

#pragma pack(push, 1)
typedef struct _TCheckBox
{
	DEFINE_TCHECKBOX;
} TCheckBox;
#pragma pack(pop)

EXTERN_C void __fastcall TCheckBox_SetChecked(LPVOID this, BOOLEAN Value);
EXTERN_C BOOLEAN __fastcall TCheckBox_GetChecked(LPVOID this);
