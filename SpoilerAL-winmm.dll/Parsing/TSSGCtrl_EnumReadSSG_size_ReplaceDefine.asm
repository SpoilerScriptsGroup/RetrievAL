.486
.model flat, c

extrn EnableParserFix:dword
extrn ReplaceDefine@8:proc

public TSSGCtrl_EnumReadSSG_size_ReplaceDefine

.code

align 16

TSSGCtrl_EnumReadSSG_size_ReplaceDefine proc near

	ReturnAddress                       equ 004E6880H
	_this                               equ <ebp + 8H>
	LineS                               equ <ebp - 38H>
	offsetof_TSSGCtrl_attributeSelector equ 32

	mov     eax, dword ptr [EnableParserFix]
	mov     ecx, dword ptr [_this]
	test    eax, eax
	jz      L1
	add     ecx, offsetof_TSSGCtrl_attributeSelector
	lea     eax, [LineS]
	push    eax
	push    ecx
	call    ReplaceDefine@8
L1:
	mov     ecx, ReturnAddress
	lea     eax, [LineS]
	push    -1
	jmp     ecx

TSSGCtrl_EnumReadSSG_size_ReplaceDefine endp

end
