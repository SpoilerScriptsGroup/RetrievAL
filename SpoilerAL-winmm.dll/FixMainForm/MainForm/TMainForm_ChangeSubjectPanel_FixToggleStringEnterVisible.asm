.486
.model flat, c

extrn F00580BBC:dword

public TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible

.code

align 16

TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible proc near

	call    dword ptr [F00580BBC]
	xor     edx, edx
	mov     eax, dword ptr [ebx + 3E0H]
	jmp     dword ptr [F00580BBC]

TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible endp

end
