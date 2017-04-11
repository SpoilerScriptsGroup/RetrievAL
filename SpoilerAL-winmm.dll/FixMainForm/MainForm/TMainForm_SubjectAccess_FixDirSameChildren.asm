.486
.model flat, c

extrn TSSDir_IsSameChildren:dword
extrn TSSGCtrl_IsChildRWProhibit:dword
extrn TMainForm_ChangeSubjectPanel:dword

public TMainForm_SubjectAccess_FixDirSameChildren

.code

align 16

TMainForm_SubjectAccess_FixDirSameChildren proc near

	_this                      equ ebx
	SelectS                    equ edi
	ArgType                    equ eax
	ssgCtrl_atDIR              equ 8
	offsetof_TMainForm_ssgCtrl equ 738H

	cmp     ArgType, ssgCtrl_atDIR
	jne     L1
	push    SelectS
	call    dword ptr [TSSDir_IsSameChildren]
	test    al, al
	lea     eax, [_this + offsetof_TMainForm_ssgCtrl]
	pop     ecx
	jz      L1
	push    SelectS
	push    eax
	call    dword ptr [TSSGCtrl_IsChildRWProhibit]
	add     esp, 8
	test    al, al
	jz      L2
L1:
	jmp     dword ptr [TMainForm_ChangeSubjectPanel]
L2:
	ret

TMainForm_SubjectAccess_FixDirSameChildren endp

end
