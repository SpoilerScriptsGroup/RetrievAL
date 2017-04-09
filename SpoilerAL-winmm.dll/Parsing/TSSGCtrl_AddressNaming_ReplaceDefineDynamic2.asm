.486
.model flat, c

extrn EnableParserFix:dword
extrn ReplaceDefineDynamic@8:proc
extrn bcb6_std_string_ctor_assign:dword

public TSSGCtrl_AddressNaming_ReplaceDefineDynamic2

.code

align 16

TSSGCtrl_AddressNaming_ReplaceDefineDynamic2 proc near

	_bcb6_std_string_ctor_assign equ 004166F0H
	SSGS                         equ <ebp + 10H>
	ReturnString                 equ eax

	mov     ecx, dword ptr [EnableParserFix]
	mov     edx, dword ptr [SSGS]
	test    ecx, ecx
	jz      L1
	push    ReturnString
	push    edx
	push    _bcb6_std_string_ctor_assign
	jmp     ReplaceDefineDynamic@8
L1:
	jmp     dword ptr [bcb6_std_string_ctor_assign]

TSSGCtrl_AddressNaming_ReplaceDefineDynamic2 endp

end
