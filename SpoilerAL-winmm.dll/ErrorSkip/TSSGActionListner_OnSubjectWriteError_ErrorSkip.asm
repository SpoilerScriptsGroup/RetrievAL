.486
.model flat, c

public TSSGActionListner_OnSubjectWriteError_ErrorSkip

.code

align 16

TSSGActionListner_OnSubjectWriteError_ErrorSkip proc near

	AT_ERRORSKIP          equ 2000H
	NULL                  equ 0
	SSGS                  equ <ebp + 12>
	TSSGCtrl_GetAttribute equ 005038E8H

	push    eax
	mov     ecx, dword ptr [SSGS]
	mov     eax, TSSGCtrl_GetAttribute
	push    AT_ERRORSKIP
	push    ecx
	push    NULL
	call    eax
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
