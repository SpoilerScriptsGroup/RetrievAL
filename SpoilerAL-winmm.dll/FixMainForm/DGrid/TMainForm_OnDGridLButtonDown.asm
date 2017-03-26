.486
.model flat, c

public TMainForm_DGridLButtonDownRow
public TMainForm_OnDGridLButtonDown@8

.data

TMainForm_DGridLButtonDownRow dword -1

.code

align 16

;int TMainForm_DGridLButtonDownRow = -1;
;void __stdcall TMainForm_OnDGridLButtonDown(TMainForm *_this, LPARAM lParam)
;{
;	_this->DGrid->MouseToCell(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), ACol, TMainForm_DGridLButtonDownRow);
;}
TMainForm_OnDGridLButtonDown@8 proc near

	_this  equ <esp + 4>
	lParam equ <esp + 8>

	mov     edx, dword ptr [lParam]
	mov     eax, dword ptr [_this]
	mov     ecx, edx
	mov     eax, dword ptr [eax + 932]
	shl     edx, 16
	push    ecx
	sar     edx, 16
	push    esp
	sar     ecx, 16
	push    offset TMainForm_DGridLButtonDownRow
	push    offset L1
	push    00559DB8H
	ret
	align   16
L1:
	pop     eax
	ret     8

TMainForm_OnDGridLButtonDown@8 endp

end
