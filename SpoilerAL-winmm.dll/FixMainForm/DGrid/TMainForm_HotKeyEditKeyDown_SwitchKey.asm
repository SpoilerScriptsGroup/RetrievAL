.486
.model flat, c

extrn TMainForm_HotKeyEditKeyDown_PageUp@4:proc
extrn TMainForm_HotKeyEditKeyDown_PageDown@4:proc
extrn TMainForm_HotKeyEditKeyDown_End@4:proc
extrn TMainForm_HotKeyEditKeyDown_Home@4:proc

public TMainForm_HotKeyEditKeyDown_SwitchKey

.code

align 16

TMainForm_HotKeyEditKeyDown_SwitchKey proc near

	VK_SPACE equ 20H
	VK_PRIOR equ 21H
	VK_NEXT  equ 22H
	VK_END   equ 23H
	VK_HOME  equ 24H
	VK_UP    equ 26H

	ReturnAddress equ 00443484H
	_this         equ ebx

	dec     cx
	jz      L1
	dec     cx
	jz      L2
	dec     cx
	jz      L3
	dec     cx
	jz      L4
	sub     cx, VK_UP - VK_HOME
	mov     eax, 004431E4H
	mov     edx, 004431F7H
	jz      L5
	jmp     eax
L1:
	push    _this
	push    ReturnAddress
	jmp     TMainForm_HotKeyEditKeyDown_PageUp@4
L2:
	push    _this
	push    ReturnAddress
	jmp     TMainForm_HotKeyEditKeyDown_PageDown@4
L3:
	push    _this
	push    ReturnAddress
	jmp     TMainForm_HotKeyEditKeyDown_End@4
L4:
	push    _this
	push    ReturnAddress
	jmp     TMainForm_HotKeyEditKeyDown_Home@4
L5:
	jmp     edx

TMainForm_HotKeyEditKeyDown_SwitchKey endp

end
