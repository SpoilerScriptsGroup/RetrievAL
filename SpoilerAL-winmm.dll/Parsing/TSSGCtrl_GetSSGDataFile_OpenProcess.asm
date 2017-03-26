.486
.model flat, c

extrn TSSGCtrl_OpenProcess@12:proc

public TSSGCtrl_GetSSGDataFile_OpenProcess

.code

align 16

TSSGCtrl_GetSSGDataFile_OpenProcess proc near

	StepSize equ <ebp - 308H>
	SHandle  equ <ebp - 350H>

	mov     eax, dword ptr [StepSize]
	mov     ecx, dword ptr [ebp - 30H]
	test    eax, eax
	jz      L2
	mov     eax, dword ptr [ebp + 8H]
	mov     ecx, dword ptr [ecx + 18H]
	add     eax, 432
	push    ecx
	push    16
	push    eax
	call    TSSGCtrl_OpenProcess@12
	test    eax, eax
	jz      L1
	mov     dword ptr [SHandle], eax
	jmp     L2
L1:
	mov     dword ptr [StepSize], eax
L2:
	ret

TSSGCtrl_GetSSGDataFile_OpenProcess endp

end
