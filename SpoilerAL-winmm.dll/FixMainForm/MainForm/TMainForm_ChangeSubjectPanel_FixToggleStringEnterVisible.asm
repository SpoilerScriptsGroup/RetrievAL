.486
.model flat, c

public TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible

.code

align 16

TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible proc near

	mov     ecx, 00580BBCH
	call    ecx
	xor     edx, edx
	mov     ecx, 00580BBCH
	mov     eax, dword ptr [ebx + 3E0H]
	jmp     ecx

TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible endp

end
