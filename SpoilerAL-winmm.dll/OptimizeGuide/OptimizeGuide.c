#include <windows.h>
#include "TGuideForm.h"

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90

EXTERN_C void __cdecl OptimizeGuide()
{
	// TGuideForm::Guide
	*(LPBYTE )0x0048CF94 = JMP_REL32;
	*(LPDWORD)0x0048CF95 = (DWORD)TGuideForm_Guide - (0x0048CF95 + sizeof(DWORD));
	*(LPBYTE )0x0048CF99 = NOP;
}
