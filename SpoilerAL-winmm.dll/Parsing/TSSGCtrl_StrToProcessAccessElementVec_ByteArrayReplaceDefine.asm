.486
.model flat, c

public TSSGCtrl_StrToProcessAccessElementVec_ByteArrayReplaceDefine

extrn EnableParserFix:dword
extrn ByteArrayReplaceDefine@8:proc

.code

align 16

TSSGCtrl_StrToProcessAccessElementVec_ByteArrayReplaceDefine proc near

	SSGS equ <ebp + 10H>
	Code equ <ebp + 14H>

	cmp     dword ptr [EnableParserFix], 0
	je      L1
	mov     eax, dword ptr [SSGS]
	lea     ecx, [Code]
	push    ecx
	push    eax
	call    ByteArrayReplaceDefine@8
L1:
	mov     eax, dword ptr [Code]
	mov     al, byte ptr [eax]
	ret

TSSGCtrl_StrToProcessAccessElementVec_ByteArrayReplaceDefine endp

end
