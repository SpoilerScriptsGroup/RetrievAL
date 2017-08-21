.486
.model flat

extrn _EnableParserFix:dword
extrn _ReplaceDefineDynamic@8:proc
extrn @bcb6_std_string_ctor_assign@8:proc

public _TSSGCtrl_AddressNaming_ReplaceDefineDynamic2

.code

align 16

_TSSGCtrl_AddressNaming_ReplaceDefineDynamic2 proc near

	SSGS         equ <ebp + 10H>
	ReturnString equ eax

	mov     ecx, dword ptr [_EnableParserFix]
	mov     edx, dword ptr [SSGS]
	test    ecx, ecx
	jz      L1
	push    ReturnString
	push    edx
	call    _ReplaceDefineDynamic@8
L1:
	mov     edx, dword ptr [esp + 8]
	mov     ecx, dword ptr [esp + 4]
	jmp     @bcb6_std_string_ctor_assign@8

_TSSGCtrl_AddressNaming_ReplaceDefineDynamic2 endp

end
