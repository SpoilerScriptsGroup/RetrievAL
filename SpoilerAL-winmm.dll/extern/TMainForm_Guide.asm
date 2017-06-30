.486
.model flat, c

extrn F005D54CC:dword
extrn F0048C1F8:dword
extrn _TGuideForm_Guide:dword
extrn F0056DDBC:dword

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
	mov     ebx, dword ptr [eax]
	mov     eax, 006211F4H
	call    dword ptr [F005D54CC]
	mov     ecx, dword ptr [ebx + 448H]
	mov     eax, dword ptr [ebx + 56CH]
	dec     ecx
	jz      L2
	test    eax, eax
	jnz     L1
	mov     eax, 0062089CH
	mov     ecx, ebx
	mov     eax, dword ptr [eax]
	mov     dl, 1
	call    dword ptr [F0048C1F8]
	mov     dword ptr [ebx + 56CH], eax
L1:
	mov     ecx, dword ptr [ebp + 0CH]
	mov     edx, dword ptr [ebp + 8H]
	push    ecx
	push    edx
	push    eax
	call    dword ptr [_TGuideForm_Guide]
	mov     edx, dword ptr [ebx + 56CH]
	add     esp, 12
	mov     cl, byte ptr [edx + 57H]
	mov     eax, edx
	test    cl, cl
	jnz     L2
	call    dword ptr [F0056DDBC]
L2:
	pop     ebx
	mov     esp, ebp
	pop     ebp
	ret     8

TMainForm_Guide@8 endp

end
