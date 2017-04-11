.486
.model flat, c

extrn RepeatDepth:dword
extrn TSSGCtrl_EnumReadSSG:dword

public TSSGCtrl_ReadSSG_EnumReadSSG

.code

align 16

TSSGCtrl_ReadSSG_EnumReadSSG proc near

	NULL        equ 0
	_this       equ ebx
	SSGFile     equ <ebp + 0CH>
	ParentStack equ edi

	mov     dword ptr [RepeatDepth], 0
	mov     eax, dword ptr [SSGFile]
	push    -1
	push    0
	push    NULL
	push    ParentStack
	push    eax
	push    _this
	push    004E4708H
	jmp     dword ptr [TSSGCtrl_EnumReadSSG]

TSSGCtrl_ReadSSG_EnumReadSSG endp

end
