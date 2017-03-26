.486
.model flat, c

extrn EnableParserFix:dword
extrn ReplaceDefine@8:proc

public TSSGCtrl_AddressNaming_ReplaceDefine3

.code

align 16

TSSGCtrl_AddressNaming_ReplaceDefine3 proc near

	CallAddress                         equ 004AC3ECH
	_this                               equ edi
	offsetof_TSSGCtrl_attributeSelector equ 32
	tmpS                                equ eax

	mov     ecx, dword ptr [EnableParserFix]
	lea     edx, dword ptr [_this + offsetof_TSSGCtrl_attributeSelector]
	test    ecx, ecx
	jz      L1
	push    tmpS
	push    edx
	push    CallAddress
	jmp     ReplaceDefine@8
L1:
	mov     eax, CallAddress
	jmp     eax

TSSGCtrl_AddressNaming_ReplaceDefine3 endp

end
