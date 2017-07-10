.486
.model flat, c

extrn EnableParserFix:dword
extrn ByteArrayReplaceDefine@8:proc
extrn TStringDivision_Half:proc

public TSSGCtrl_AddressNaming_ByteArrayReplaceDefine

.code

align 16

TSSGCtrl_AddressNaming_ByteArrayReplaceDefine proc near

	SSGS equ <ebp + 10H>
	tmpS equ eax

	mov     ecx, dword ptr [EnableParserFix]
	mov     edx, dword ptr [SSGS]
	test    ecx, ecx
	jz      L1
	push    tmpS
	push    edx
	push    offset TStringDivision_Half
	jmp     ByteArrayReplaceDefine@8
L1:
	jmp     TStringDivision_Half

TSSGCtrl_AddressNaming_ByteArrayReplaceDefine endp

end
