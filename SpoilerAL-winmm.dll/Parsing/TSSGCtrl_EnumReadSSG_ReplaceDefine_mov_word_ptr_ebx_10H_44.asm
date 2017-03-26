.486
.model flat, c

extrn EnableParserFix:dword
extrn ReplaceDefine@8:proc

public TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44

.code

align 16

TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 proc near

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
	mov     word ptr [ebx + 10H], 44
	ret

TSSGCtrl_EnumReadSSG_ReplaceDefine_mov_word_ptr_ebx_10H_44 endp

end
