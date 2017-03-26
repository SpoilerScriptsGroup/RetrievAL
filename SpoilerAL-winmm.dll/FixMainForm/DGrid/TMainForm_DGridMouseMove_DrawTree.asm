.486
.model flat, c

extrn TMainForm_DrawTree@20:proc

public TMainForm_DGridMouseMove_DrawTree

.code

align 16

TMainForm_DGridMouseMove_DrawTree proc near

	NULL                        equ 0
	_this                       equ ebx
	offsetof_TMainForm_userMode equ 448H
	ReturnAddress               equ 00446C6DH

	xor     eax, eax
	cmp     dword ptr [_this + offsetof_TMainForm_userMode], 4
	sete    al
	push    eax
	push    0
	push    0
	push    NULL
	push    _this
	push    ReturnAddress
	jmp     TMainForm_DrawTree@20

TMainForm_DGridMouseMove_DrawTree endp

end
