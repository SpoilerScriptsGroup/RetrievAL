.486
.model flat, c
assume fs:nothing

extrn bcb6_global_operator_new:dword
extrn F005D54CC:dword
extrn F004FE200:dword

public FixTraceAndCopySetting@8

.code

align 16

FixTraceAndCopySetting@8 proc near

	push    ebp
	mov     eax, 006164C4H
	mov     ebp, esp
	sub     esp, 40
	call    dword ptr [F005D54CC]
	push    12
	call    dword ptr [bcb6_global_operator_new]
	test    eax, eax
	jz      L1
	mov     dword ptr [eax], 00615314H
	mov     dword ptr [eax + 4H], 3
	mov     byte ptr [eax + 8H], 1
L1:
	mov     ecx, dword ptr [ebp + 8H]
	mov     dword ptr [esp], eax
	push    ecx
	mov     ecx, dword ptr [ebp + 0CH]
	push    1
	push    ecx
	call    dword ptr [F004FE200]
	mov     ecx, dword ptr [ebp - 40]
	mov     dword ptr fs:[0], ecx
	mov     esp, ebp
	pop     ebp
	ret     8

FixTraceAndCopySetting@8 endp

end
