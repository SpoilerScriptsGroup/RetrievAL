#pragma once

#include <windows.h>

#define Application (*(LPVOID *)0x0064D0AC)

EXTERN_C void __fastcall TApplication_ProcessMessages(LPVOID _Application);

