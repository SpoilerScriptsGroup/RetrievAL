.486
.model flat, c

extrn EnableParserFix:dword
extrn ReplaceDefine@8:proc

public TSSGCtrl_EnumReadSSG_title_ReplaceDefine

.code

align 16

TSSGCtrl_EnumReadSSG_title_ReplaceDefine proc near

	ReturnAddress                       equ 004E9206H
	_this                               equ <ebp + 8H>
	LineS                               equ <ebp - 38H>
	offsetof_TSSGCtrl_attributeSelector equ 32

	mov     eax, dword ptr [EnableParserFix]
	push    ReturnAddress
	test    eax, eax
	jz      L1
	mov     eax, dword ptr [_this]
	lea     ecx, [LineS]
	add     eax, offsetof_TSSGCtrl_attributeSelector
	push    ecx
	push    eax
	call    ReplaceDefine@8
L1:
	mov     eax, dword ptr [LineS]
	lea     edx, [ebp - 498H]
	ret

TSSGCtrl_EnumReadSSG_title_ReplaceDefine endp

end
