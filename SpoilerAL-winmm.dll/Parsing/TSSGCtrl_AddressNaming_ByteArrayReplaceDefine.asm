.486
.model flat, c

extrn EnableParserFix:dword
extrn ByteArrayReplaceDefine@8:proc

public TSSGCtrl_AddressNaming_ByteArrayReplaceDefine

.code

align 16

TSSGCtrl_AddressNaming_ByteArrayReplaceDefine proc near

	CallAddress equ 004AC3ECH
	SSGS        equ <ebp + 10H>
	tmpS        equ eax

	mov     ecx, dword ptr [EnableParserFix]
	mov     edx, dword ptr [SSGS]
	test    ecx, ecx
	jz      L1
	push    tmpS
	push    edx
	push    CallAddress
	jmp     ByteArrayReplaceDefine@8
L1:
	mov     eax, CallAddress
	jmp     eax

TSSGCtrl_AddressNaming_ByteArrayReplaceDefine endp

end
