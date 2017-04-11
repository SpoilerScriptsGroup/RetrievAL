.486
.model flat, c

extrn TSSGCtrl_EnumReadSSG:dword

public TSSGCtrl_EnumReadSSG_replace_EnumReadSSG

.code

align 16

TSSGCtrl_EnumReadSSG_replace_EnumReadSSG proc near

	_this        equ <ebp +   8H>
	ParentStack  equ <ebp +  10H>
	ADJElem      equ <ebp +  14H>
	RepeatIndex  equ <ebp +  18H>
	ParentRepeat equ <ebp +  1CH>
	tmpL         equ <ebp - 904H>

	mov     ecx, dword ptr [ParentRepeat]
	mov     eax, dword ptr [RepeatIndex]
	push    ecx
	push    eax
	mov     ecx, dword ptr [ADJElem]
	mov     eax, dword ptr [ParentStack]
	push    ecx
	push    eax
	mov     ecx, dword ptr [tmpL]
	mov     eax, dword ptr [_this]
	push    ecx
	push    eax
	push    004E66CFH
	jmp     dword ptr [TSSGCtrl_EnumReadSSG]

TSSGCtrl_EnumReadSSG_replace_EnumReadSSG endp

end
