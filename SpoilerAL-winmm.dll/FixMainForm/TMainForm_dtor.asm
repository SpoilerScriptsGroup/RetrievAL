.486
.model flat, c

includelib user32.lib

extrn _imp__SetWindowLongA@12:dword
extrn DeleteProcessMonitor:proc
extrn DeleteWaitCursor:proc
extrn ClearGuideBuffer:proc
extrn SubjectStringTable_dtor:proc

extrn TMainForm_PrevWindowProc:dword
extrn TMainForm_PrevDGridProc:dword
extrn _TWinControl_GetHandle:dword

public TMainForm_dtor

.code

align 16

TMainForm_dtor proc near

	GWL_WNDPROC equ -4

	call    ClearGuideBuffer

	call    DeleteWaitCursor

	mov     eax, 0064CE2CH
	mov     eax, dword ptr [eax]
	mov     eax, dword ptr [eax + 3A4H]
	call    dword ptr [_TWinControl_GetHandle]
	push    dword ptr [TMainForm_PrevDGridProc]
	push    GWL_WNDPROC
	push    eax
	call    dword ptr [_imp__SetWindowLongA@12]

	mov     eax, 0064CE2CH
	mov     eax, dword ptr [eax]
	call    dword ptr [_TWinControl_GetHandle]
	push    dword ptr [TMainForm_PrevWindowProc]
	push    GWL_WNDPROC
	push    eax
	call    dword ptr [_imp__SetWindowLongA@12]

	call    DeleteProcessMonitor

	call    SubjectStringTable_dtor

	mov     eax, 0045FDF2H
	dec     dword ptr [ebx + 1CH]
	lea     edx, [esi + 0C10H]
	jmp     eax

TMainForm_dtor endp

end
