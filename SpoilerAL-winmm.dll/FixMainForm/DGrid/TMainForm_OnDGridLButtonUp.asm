.486
.model flat, c

includelib user32.lib
includelib kernel32.lib

extrn TMainForm_DGridLButtonDblClk:byte
extrn TMainForm_DGridLButtonDownRow:dword
extrn _imp__GetScrollInfo@12:dword
extrn _imp__MulDiv@12:dword

public TMainForm_OnDGridLButtonUp@8

.code

align 16

;void __stdcall TMainForm_OnDGridLButtonUp(TMainForm *_this, LPARAM lParam)
;{
;	int ACol, ARow;
;
;	if (TMainForm_DGridLButtonDblClk)
;		return;
;	if ((unsigned int)TMainForm_DGridLButtonDownRow > _this->treeSubjectVec.size())
;		return;
;	_this->DGrid->MouseToCell(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), ACol, ARow);
;	if (ARow != TMainForm_DGridLButtonDownRow)
;		return;
;	if (ARow != 0)
;	{
;		int        PrevRow;
;		RECT       PrevRect, Rect;
;		SCROLLINFO si;
;
;		PrevRow = _this->nowSelectSubjectIndex + 1;
;		_this->nowSelectSubjectIndex = ARow - 1;
;		PrevRect = _this->DGrid->CellRect(0, PrevRow);
;		Rect = _this->DGrid->CellRect(0, ARow);
;		si.cbSize = sizeof(si);
;		si.fMask = SIF_RANGE | SIF_POS;
;		if (::GetScrollInfo(_this->DGrid->Handle, SB_HORZ, &si))
;		{
;			int nRange;
;
;			nRange = si.nMax - si.nMin;
;			if (nRange != 0)
;			{
;				int nXOffset;
;
;				nXOffset = ::MulDiv(_this->DGrid->DefaultColWidth - _this->DGrid->ClientWidth, si.nPos - si.nMin, nRange);
;				PrevRect.left -= nXOffset;
;				Rect.left -= nXOffset;
;			}
;		}
;		_this->DrawTreeCell(_this->DGrid->Canvas, PrevRow, PrevRect);
;		_this->DrawTreeCell(_this->DGrid->Canvas, ARow, Rect);
;		_this->SubjectAccess(_this->selectSubject = _this->treeSubjectVec[_this->nowSelectSubjectIndex]);
;	}
;	else
;	{
;		_this->M_TitleSelectClick(_this->M_TitleSelect);
;	}
;}
TMainForm_OnDGridLButtonUp@8 proc near

	cmp     byte ptr [TMainForm_DGridLButtonDblClk], 0
	jne     L7
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
	jb      L6
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
	push    offset L1
	push    00559DB8H
	ret
	align   16
L1:
	mov     eax, dword ptr [esp + 60]
	mov     ecx, dword ptr [TMainForm_DGridLButtonDownRow]
	cmp     eax, ecx
	jne     L6
	sub     eax, 1
	jb      L5
	mov     ecx, dword ptr [ebx + 1364]
	lea     edx, [esp + 44]
	mov     dword ptr [ebx + 1364], eax
	push    edx
	inc     ecx
	xor     edx, edx
	mov     esi, ecx
	mov     eax, dword ptr [ebx + 932]
	push    offset L2
	push    00559D94H
	ret
	align   16
L2:
	lea     eax, [esp + 28]
	mov     ecx, dword ptr [esp + 60]
	push    eax
	push    offset L3
	xor     edx, edx
	mov     eax, dword ptr [ebx + 932]
	push    00559D94H
	ret
	align   16
L3:
	mov     dword ptr [esp], 28
	mov     dword ptr [esp + 4], 5
	push    esp
	push    0
	mov     ecx, 0058750CH
	mov     eax, dword ptr [ebx + 932]
	call    ecx
	push    eax
	call    dword ptr [_imp__GetScrollInfo@12]
	test    eax, eax
	jz      L4
	mov     edx, dword ptr [esp + 12]
	mov     eax, dword ptr [esp + 8]
	sub     edx, eax
	je      L4
	mov     ecx, dword ptr [esp + 20]
	push    edx
	sub     ecx, eax
	mov     eax, dword ptr [ebx + 932]
	push    ecx
	mov     edx, dword ptr [eax + 560]
	mov     ecx, 005806A0H
	push    edx
	call    ecx
	sub     dword ptr [esp], eax
	call    dword ptr [_imp__MulDiv@12]
	mov     ecx, dword ptr [esp + 44]
	mov     edx, dword ptr [esp + 28]
	sub     ecx, eax
	sub     edx, eax
	mov     dword ptr [esp + 44], ecx
	mov     dword ptr [esp + 28], edx
L4:
	lea     edx, [esp + 44]
	mov     ecx, dword ptr [ebx + 932]
	push    edx
	mov     ecx, dword ptr [ecx + 520]
	push    esi
	push    ecx
	mov     eax, 00444848H
	push    ebx
	call    eax
	add     esp, 16
	mov     ecx, dword ptr [ebx + 932]
	lea     eax, [esp + 28]
	mov     edx, dword ptr [esp + 60]
	mov     ecx, dword ptr [ecx + 520]
	push    eax
	push    edx
	push    ecx
	mov     eax, 00444848H
	push    ebx
	call    eax
	mov     eax, dword ptr [ebx + 1364]
	add     esp, 16
	shl     eax, 2
	mov     ecx, dword ptr [ebx + 3088]
	mov     ecx, dword ptr [eax + ecx]
	mov     eax, 00439F10H
	mov     dword ptr [ebx + 1316], ecx
	push    ecx
	push    ebx
	call    eax
	add     esp, 8
	jmp     L6
L5:
	mov     edx, dword ptr [ebx + 772]
	mov     ecx, 0044906CH
	mov     eax, ebx
	call    ecx
L6:
	add     esp, 68
	pop     esi
	pop     ebx
L7:
	ret     8

TMainForm_OnDGridLButtonUp@8 endp

end
