.486
.model flat, c

extrn RepeatDepth:dword

public TSSGCtrl_ReadSSG_EnumReadSSG

.code

align 16

TSSGCtrl_ReadSSG_EnumReadSSG proc near

	NULL        equ 0
	_this       equ ebx
	SSGFile     equ <ebp + 0CH>
	ParentStack equ edi
	EnumReadSSG equ 004E5090H

	mov     dword ptr [RepeatDepth], 0
	mov     ecx, dword ptr [SSGFile]
	mov     eax, EnumReadSSG
	push    -1
	push    0
	push    NULL
	push    ParentStack
	push    ecx
	push    _this
	push    004E4708H
	jmp     eax

TSSGCtrl_ReadSSG_EnumReadSSG endp

end
