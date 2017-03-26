.486
.model flat, c

extrn EnableParserFix:dword
extrn ReplaceDefine@8:proc

public TSSGCtrl_AddressNaming_ReplaceDefine1

.code

align 16

TSSGCtrl_AddressNaming_ReplaceDefine1 proc near

	CallAddress                         equ 00504284H
	_this                               equ edi
	offsetof_TSSGCtrl_attributeSelector equ 32
	NameStr                             equ edx

	mov     eax, dword ptr [EnableParserFix]
	lea     ecx, dword ptr [_this + offsetof_TSSGCtrl_attributeSelector]
	test    eax, eax
	jz      L1
	push    NameStr
	push    ecx
	push    CallAddress
	jmp     ReplaceDefine@8
L1:
	mov     eax, CallAddress
	jmp     eax

TSSGCtrl_AddressNaming_ReplaceDefine1 endp

end
