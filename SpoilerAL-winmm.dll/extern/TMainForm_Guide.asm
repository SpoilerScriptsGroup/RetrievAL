.486
.model flat, c

public TMainForm_Guide@8

.code

align 16

;EXTERN_C void __stdcall TMainForm_Guide(const char *Mes, BOOLEAN IsClear)
;{
;	if (MainForm->userMode != 1)
;	{
;		if (MainForm->guideForm == NULL)
;			MainForm->guideForm = new TGuideForm(MainForm);
;		MainForm->guideForm->Guide(Mes, IsClear);
;		if (!MainForm->guideForm->Visible)
;			MainForm->guideForm->Show();
;	}
;}
TMainForm_Guide@8 proc near

	push    ebp
	mov     eax, 0064CE2CH
	mov     ebp, esp
	sub     esp, 36
	push    ebx
	mov     ecx, 005D54CCH
	mov     ebx, dword ptr [eax]
	mov     eax, 006211F4H
	call    ecx
	mov     ecx, dword ptr [ebx + 448H]
	mov     eax, dword ptr [ebx + 56CH]
	dec     ecx
	jz      L3
	test    eax, eax
	jnz     L2
	mov     eax, 0062089CH
	mov     ecx, ebx
	mov     eax, dword ptr [eax]
	mov     dl, 1
	push    offset L1
	push    0048C1F8H
	ret
	align   16
L1:
	mov     dword ptr [ebx + 56CH], eax
L2:
	mov     ecx, dword ptr [ebp + 0CH]
	mov     edx, dword ptr [ebp + 8H]
	push    ecx
	push    edx
	mov     ecx, 0048CF94H
	push    eax
	call    ecx
	mov     edx, dword ptr [ebx + 56CH]
	add     esp, 12
	mov     al, byte ptr [edx + 57H]
	mov     ecx, 0056DDBCH
	test    al, al
	jnz     L3
	mov     eax, edx
	call    ecx
L3:
	pop     ebx
	mov     esp, ebp
	pop     ebp
	ret     8

TMainForm_Guide@8 endp

end
