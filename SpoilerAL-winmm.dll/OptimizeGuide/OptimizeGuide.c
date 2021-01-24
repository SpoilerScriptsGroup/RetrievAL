#include <windows.h>
#include "TMainForm.h"

static __declspec(naked) void __fastcall TMainForm_DGridDrawCell_DrawTreeCell(int ARow, TDrawGrid *DGrid)
{
	static const LPCVOID listener = NULL;
	__asm {// Don't change eax!
		lea    ecx, [eax]TMainForm.ssgActionListner
		cmp    [eax]TMainForm.userMode, 4
		cmovae ecx, listener// quiet mode
		mov    [eax]TMainForm.ssgCtrl.ssgActionListner, ecx
		mov    ecx, [edx]TDrawGrid.Canvas
		ret
	}// Must make ecx into TCanvas*
}

static void __fastcall TMainForm_DGridDrawCell_epilog(
	TMainForm       *const this,
	TCanvas   const *const DestCanvas,
	BYTE/*TGridDrawState*/ State,
	RECT      const *const Rect,
	int              const ARow)
{
	this->ssgCtrl.ssgActionListner = &this->ssgActionListner;
}

#define CALL_REL  (BYTE)0xE8
#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90

EXTERN_C void __cdecl OptimizeGuide()
{
	// TMainForm::DGridDrawCell
	*(LPBYTE )0x00444831 = CALL_REL;
	*(LPDWORD)0x00444832 = (DWORD)TMainForm_DGridDrawCell_DrawTreeCell - (0x00444832 + sizeof(DWORD));
	*(LPBYTE )0x00444836 = NOP;

	*(LPWORD )0x0044483E = BSWAP16(0x595A);// pop ecx; pop edx
	*(LPDWORD)0x00444840 = BSWAP32(0x58585D << 8 | JMP_REL32);
	*(LPDWORD)0x00444844 = (DWORD)TMainForm_DGridDrawCell_epilog - (0x00444844 + sizeof(DWORD));

	// TGuideForm::Guide
	*(LPBYTE )0x0048CF94 = JMP_REL32;
	*(LPDWORD)0x0048CF95 = (DWORD)TGuideForm_Guide - (0x0048CF95 + sizeof(DWORD));
	*(LPBYTE )0x0048CF99 = NOP;
}
