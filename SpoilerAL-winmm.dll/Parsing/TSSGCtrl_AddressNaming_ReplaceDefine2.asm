.486
.model flat, c

extrn EnableParserFix:dword
extrn ReplaceDefine@8:proc
extrn bcb6_std_string_ctor_assign:dword

public TSSGCtrl_AddressNaming_ReplaceDefine2

.code

align 16

TSSGCtrl_AddressNaming_ReplaceDefine2 proc near

	_bcb6_std_string_ctor_assign        equ 004166F0H
	_this                               equ edi
	offsetof_TSSGCtrl_attributeSelector equ 32
	ReturnString                        equ eax

	mov     ecx, dword ptr [EnableParserFix]
	lea     edx, dword ptr [_this + offsetof_TSSGCtrl_attributeSelector]
	test    ecx, ecx
	jz      L1
	push    ReturnString
	push    edx
	push    _bcb6_std_string_ctor_assign
	jmp     ReplaceDefine@8
L1:
	jmp     dword ptr [bcb6_std_string_ctor_assign]

TSSGCtrl_AddressNaming_ReplaceDefine2 endp

end
