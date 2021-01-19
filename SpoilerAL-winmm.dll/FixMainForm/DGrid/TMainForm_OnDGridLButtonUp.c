#include <windows.h>
#include "TMainForm.h"

extern const DWORD F00439F10;
extern const DWORD _TMainForm_M_TitleSelectClick;
extern const DWORD F00559D94;
extern const DWORD F00559DB8;
extern const DWORD F005806A0;
extern const DWORD _TWinControl_GetHandle;

extern BOOLEAN TMainForm_DGridLButtonDblClk;
extern int TMainForm_DGridLButtonDownRow;

__declspec(naked) void __stdcall TMainForm_OnDGridLButtonUp(TMainForm *this, LPARAM lParam)
{
	/*
	int ACol, ARow;

	if (TMainForm_DGridLButtonDblClk)
		return;
	if ((unsigned int)TMainForm_DGridLButtonDownRow > this->treeSubjectVec.size())
		return;
	this->DGrid->MouseToCell(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), ACol, ARow);
	if (ARow != TMainForm_DGridLButtonDownRow)
		return;
	if (ARow != 0)
	{
		int        PrevRow;
		RECT       PrevRect, Rect;
		SCROLLINFO si;

		PrevRow = this->nowSelectSubjectIndex + 1;
		this->nowSelectSubjectIndex = ARow - 1;
		PrevRect = this->DGrid->CellRect(0, PrevRow);
		Rect = this->DGrid->CellRect(0, ARow);
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_POS;
		if (::GetScrollInfo(this->DGrid->Handle, SB_HORZ, &si))
		{
			int nRange;

			nRange = si.nMax - si.nMin;
			if (nRange != 0)
			{
				int nXOffset;

				nXOffset = ::MulDiv(this->DGrid->DefaultColWidth - this->DGrid->ClientWidth, si.nPos - si.nMin, nRange);
				PrevRect.left -= nXOffset;
				Rect.left -= nXOffset;
			}
		}
		if (PrevRow != ARow)
			this->DrawTreeCell(this->DGrid->Canvas, PrevRow, PrevRect);
		this->DrawTreeCell(this->DGrid->Canvas, ARow, Rect);
		this->SubjectAccess(this->selectSubject = this->treeSubjectVec[this->nowSelectSubjectIndex]);
	}
	else
	{
		this->M_TitleSelectClick(this->M_TitleSelect);
	}
	*/

	__asm
	{
		cmp     byte ptr [TMainForm_DGridLButtonDblClk], 0
		jne     L4
		mov     edx, dword ptr [esp + 4]
		push    ebx
		mov     eax, dword ptr [edx + 3092]
		mov     ecx, dword ptr [edx + 3088]
		sub     eax, ecx
		mov     ebx, edx
		mov     ecx, eax
		push    esi
		sar     eax, 31
		sub     esp, 68
		add     ecx, eax
		add     eax, eax
		add     eax, ecx
		sar     eax, 2
		mov     ecx, dword ptr [TMainForm_DGridLButtonDownRow]
		cmp     eax, ecx
		jb      L3
		mov     edx, dword ptr [esp + 84]
		lea     eax, [esp + 64]
		mov     ecx, edx
		push    eax
		shl     edx, 16
		sub     eax, 4
		sar     edx, 16
		push    eax
		sar     ecx, 16
		mov     eax, dword ptr [ebx + 932]
		call    dword ptr [F00559DB8]
		mov     eax, dword ptr [esp + 60]
		mov     ecx, dword ptr [TMainForm_DGridLButtonDownRow]
		cmp     eax, ecx
		jne     L3
		sub     eax, 1
		jb      L2
		mov     ecx, dword ptr [ebx + 1364]
		lea     edx, [esp + 44]
		mov     dword ptr [ebx + 1364], eax
		push    edx
		inc     ecx
		xor     edx, edx
		mov     esi, ecx
		mov     eax, dword ptr [ebx + 932]
		call    dword ptr [F00559D94]
		mov     ecx, dword ptr [esp + 60]
		lea     eax, [esp + 28]
		push    eax
		xor     edx, edx
		mov     eax, dword ptr [ebx + 932]
		call    dword ptr [F00559D94]
		mov     dword ptr [esp], 28
		mov     dword ptr [esp + 4], 5
		push    esp
		push    0
		mov     eax, dword ptr [ebx + 932]
		call    dword ptr [_TWinControl_GetHandle]
		push    eax
		call    GetScrollInfo
		test    eax, eax
		jz      L1
		mov     edx, dword ptr [esp + 12]
		mov     eax, dword ptr [esp + 8]
		sub     edx, eax
		je      L1
		mov     ecx, dword ptr [esp + 20]
		push    edx
		sub     ecx, eax
		mov     eax, dword ptr [ebx + 932]
		push    ecx
		mov     edx, dword ptr [eax + 560]
		push    edx
		call    dword ptr [F005806A0]
		sub     dword ptr [esp], eax
		call    MulDiv
		mov     ecx, dword ptr [esp + 44]
		mov     edx, dword ptr [esp + 28]
		sub     ecx, eax
		sub     edx, eax
		mov     dword ptr [esp + 44], ecx
		mov     dword ptr [esp + 28], edx
	L1:
		cmp     esi, dword ptr [esp + 60]
		je      AR
		lea     edx, [esp + 44]
		mov     ecx, dword ptr [ebx + 932]
		push    edx
		mov     ecx, dword ptr [ecx + 520]
		push    esi
		push    ecx
		push    ebx
		call    dword ptr [TMainForm_DrawTreeCell]
		add     esp, 16
	AR:
		mov     ecx, dword ptr [ebx + 932]
		lea     eax, [esp + 28]
		mov     edx, dword ptr [esp + 60]
		mov     ecx, dword ptr [ecx + 520]
		push    eax
		push    edx
		push    ecx
		push    ebx
		call    dword ptr [TMainForm_DrawTreeCell]
		mov     eax, dword ptr [ebx + 1364]
		add     esp, 16
		shl     eax, 2
		mov     ecx, dword ptr [ebx + 3088]
		mov     ecx, dword ptr [eax + ecx]
		mov     dword ptr [ebx + 1316], ecx
		push    ecx
		push    ebx
		call    dword ptr [F00439F10]
		add     esp, 8
		jmp     L3

	L2:
		mov     edx, dword ptr [ebx + 772]
		mov     eax, ebx
		call    dword ptr [_TMainForm_M_TitleSelectClick]
	L3:
		add     esp, 68
		pop     esi
		pop     ebx
	L4:
		ret     8
	}
}
