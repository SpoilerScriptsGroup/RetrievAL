.486
.model flat, c

includelib user32.lib

extrn _imp__SetWindowLongA@12:dword
extrn InitializeProcessMonitor:proc
extrn InitializeMenuId@4:proc
extrn AppendDebugWithoutMouseOverModeMenu@4:proc
extrn AppendToolMenu@4:proc
extrn MidifyVersionString@4:proc
extrn TMainForm_WindowProc@16:proc
extrn TMainForm_DGridProc@16:proc
extrn TMainForm_PrevWindowProc:dword
extrn TMainForm_PrevDGridProc:dword
extrn InitializeWaitCursor:proc
extrn _TWinControl_GetHandle:dword

public TMainForm_ctor

.code

align 16

TMainForm_ctor proc near

	GWL_WNDPROC equ -4

	call    InitializeProcessMonitor

	mov     ecx, dword ptr [ebp - 4H]
	push    ecx
	push    ecx
	push    ecx
	push    ecx
	call    InitializeMenuId@4
	call    AppendDebugWithoutMouseOverModeMenu@4
	call    AppendToolMenu@4
	call    MidifyVersionString@4

	mov     eax, dword ptr [ebp - 4H]
	call    dword ptr [_TWinControl_GetHandle]
	push    offset TMainForm_WindowProc@16
	push    GWL_WNDPROC
	push    eax
	call    dword ptr [_imp__SetWindowLongA@12]
	mov     dword ptr [TMainForm_PrevWindowProc], eax

	mov     eax, dword ptr [ebp - 4H]
	mov     eax, dword ptr [eax + 3A4H]
	call    dword ptr [_TWinControl_GetHandle]
	push    offset TMainForm_DGridProc@16
	push    GWL_WNDPROC
	push    eax
	call    dword ptr [_imp__SetWindowLongA@12]
	mov     dword ptr [TMainForm_PrevDGridProc], eax

	call    InitializeWaitCursor

	mov     eax, 00401FAEH
	dec     dword ptr [ebx + 1CH]
	dec     dword ptr [ebx + 1CH]
	jmp     eax

TMainForm_ctor endp

end
