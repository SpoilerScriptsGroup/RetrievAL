.486
.model flat, c

public TMainForm_SubjectAccess_FixDirSameChildren

.code

align 16

TMainForm_SubjectAccess_FixDirSameChildren proc near

	_this                        equ ebx
	SelectS                      equ edi
	ArgType                      equ eax
	ssgCtrl_atDIR                equ 8
	offsetof_TMainForm_ssgCtrl   equ 738H
	TMainForm_ChangeSubjectPanel equ 004465ACH
	TSSDir_IsSameChildren        equ 004C31E0H
	TSSGCtrl_IsChildRWProhibit   equ 00511318H

	cmp     ArgType, ssgCtrl_atDIR
	jne     L1
	mov     eax, TSSDir_IsSameChildren
	push    SelectS
	call    eax
	pop     edx
	test    al, al
	mov     eax, TSSGCtrl_IsChildRWProhibit
	jz      L1
	lea     ecx, [_this + offsetof_TMainForm_ssgCtrl]
	push    SelectS
	push    ecx
	call    eax
	add     esp, 8
	test    al, al
	jz      L2
L1:
	mov     ecx, TMainForm_ChangeSubjectPanel
	jmp     ecx
L2:
	ret

TMainForm_SubjectAccess_FixDirSameChildren endp

end
