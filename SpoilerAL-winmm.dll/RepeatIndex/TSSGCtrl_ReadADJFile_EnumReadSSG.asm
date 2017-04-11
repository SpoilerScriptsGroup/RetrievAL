.486
.model flat, c

extrn TSSGCtrl_EnumReadSSG:dword

public TSSGCtrl_ReadADJFile_EnumReadSSG

.code

align 16

TSSGCtrl_ReadADJFile_EnumReadSSG proc near

	_this       equ esi
	ADJElem     equ <ebp +  0CH>
	ADJ         equ <ebp - 110H>
	ParentStack equ <ebp - 0B8H>

	mov     edx, dword ptr [ADJElem]
	lea     ecx, [ParentStack]
	mov     eax, dword ptr [ADJ]
	push    -1
	push    0
	push    edx
	push    ecx
	push    eax
	push    _this
	push    004FD6F6H
	jmp     dword ptr [TSSGCtrl_EnumReadSSG]

TSSGCtrl_ReadADJFile_EnumReadSSG endp

end
