.486
.model flat, c

extrn TSSGCtrl_GetAttribute:dword

public TSSGActionListner_OnSubjectWriteError_ErrorSkip

.code

align 16

TSSGActionListner_OnSubjectWriteError_ErrorSkip proc near

	AT_ERRORSKIP equ 2000H
	NULL         equ 0
	SSGS         equ <ebp + 12>

	push    eax
	mov     ecx, dword ptr [SSGS]
	push    AT_ERRORSKIP
	push    ecx
	push    NULL
	call    dword ptr [TSSGCtrl_GetAttribute]
	test    eax, eax
	jnz     L1
	add     esp, 12
	mov     ecx, 0057AEDCH
	pop     eax
	jmp     ecx
L1:
	add     esp, 12 + 4
	ret

TSSGActionListner_OnSubjectWriteError_ErrorSkip endp

end
