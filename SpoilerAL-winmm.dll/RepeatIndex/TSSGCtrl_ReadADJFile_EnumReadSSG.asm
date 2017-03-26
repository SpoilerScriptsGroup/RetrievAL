.486
.model flat, c

public TSSGCtrl_ReadADJFile_EnumReadSSG

.code

align 16

TSSGCtrl_ReadADJFile_EnumReadSSG proc near

	_this       equ esi
	ADJElem     equ <ebp +  0CH>
	ADJ         equ <ebp - 110H>
	ParentStack equ <ebp - 0B8H>
	EnumReadSSG equ 004E5090H

	mov     ecx, dword ptr [ADJElem]
	lea     eax, [ParentStack]
	push    -1
	push    0
	push    ecx
	push    eax
	mov     ecx, dword ptr [ADJ]
	mov     eax, EnumReadSSG
	push    ecx
	push    _this
	push    004FD6F6H
	jmp     eax

TSSGCtrl_ReadADJFile_EnumReadSSG endp

end
