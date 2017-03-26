.486
.model flat, c

includelib user32.lib

extrn _imp__GetScrollInfo@12:dword
extrn _imp__DrawFocusRect@8:dword

public TMainForm_DrawTreeCell_DrawFocusRect

.code

align 16

;void __stdcall TMainForm_DrawTreeCell_DrawFocusRect(int ARow, int nowSelectSubjectIndex, long TMainForm *MainForm, long RectH, TCanvas *BSCanvas)
;{
;	if (ARow == nowSelectSubjectIndex + 1)
;	{
;		TDrawGrid *DGrid;
;		RECT      rcItem;
;
;		DGrid = MainForm->DGrid;
;		rcItem.left = 2;
;		rcItem.top = 2;
;		rcItem.right = DGrid->DefaultColWidth - 2;
;		rcItem.bottom = DGrid->DefaultRowHeight - 2;
;		::DrawFocusRect(BSCanvas->Handle, &rcItem);
;	}
;}
TMainForm_DrawTreeCell_DrawFocusRect proc near

	sizeof_RECT                     equ 16

	MainForm                        equ <ebx>
	ARow                            equ <ebp + 16>
	nowSelectSubjectIndex           equ <MainForm + 1364>
	DGrid                           equ <MainForm + 932>
	offsetof_DGrid_DefaultColWidth  equ <560>
	offsetof_DGrid_DefaultRowHeight equ <564>
	BSCanvas                        equ <esi>

	mov     eax, dword ptr [nowSelectSubjectIndex]
	mov     ecx, dword ptr [ARow]
	inc     eax
	cmp     eax, ecx
	jne     L3

	sub     esp, sizeof_RECT

	rcItem        equ <esp>
	rcItem_left   equ <esp     >
	rcItem_top    equ <esp +  4>
	rcItem_right  equ <esp +  8>
	rcItem_bottom equ <esp + 12>

	mov     ecx, dword ptr [DGrid]
	mov     eax, dword ptr [ecx + offsetof_DGrid_DefaultColWidth]
	mov     ecx, dword ptr [ecx + offsetof_DGrid_DefaultRowHeight]
	sub     eax, 2
	sub     ecx, 2
	mov     dword ptr [rcItem_left  ], 2
	mov     dword ptr [rcItem_top   ], 2
	mov     dword ptr [rcItem_right ], eax
	mov     dword ptr [rcItem_bottom], ecx
	mov     ecx, 0055E74CH
	mov     eax, BSCanvas
	call    ecx
	push    rcItem
	push    eax
	call    dword ptr [_imp__DrawFocusRect@8]
	add     esp, sizeof_RECT
L3:
	mov     eax, 004460A0H
	push    00CC0020H
	jmp     eax

TMainForm_DrawTreeCell_DrawFocusRect endp

end
