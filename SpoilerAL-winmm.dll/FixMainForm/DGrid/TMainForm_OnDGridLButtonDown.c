#include "TMainForm.h"

extern const DWORD F00559DB8;

int TMainForm_DGridLButtonDownRow = -1;

__declspec(naked) void __stdcall TMainForm_OnDGridLButtonDown(TMainForm *this, LPARAM lParam)
{
	/*
	this->DGrid->MouseToCell(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), ACol, TMainForm_DGridLButtonDownRow);
	*/

	__asm
	{
		#define this   (esp + 4)
		#define lParam (esp + 8)

		mov     edx, dword ptr [lParam]
		mov     eax, dword ptr [this]
		mov     ecx, edx
		mov     eax, dword ptr [eax + 932]
		shl     edx, 16
		push    ecx
		sar     edx, 16
		push    esp
		sar     ecx, 16
		push    offset TMainForm_DGridLButtonDownRow
		call    dword ptr [F00559DB8]
		pop     eax
		ret     8

		#undef this
		#undef lParam
	}
}
