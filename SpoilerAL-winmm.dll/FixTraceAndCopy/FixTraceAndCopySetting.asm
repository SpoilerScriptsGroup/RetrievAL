.486
.model flat, c

public FixTraceAndCopySetting@8

.code

align 16

FixTraceAndCopySetting@8 proc near

	push    ebp
	mov     ecx, 005D54CCH
	mov     ebp, esp
	sub     esp, 40
	mov     eax, 006164C4H
	call    ecx
	mov     eax, 005D44B8H
	push    12
	call    eax
	test    eax, eax
	jz      L1
	mov     dword ptr [eax], 00615314H
	mov     dword ptr [eax + 4H], 3
	mov     byte ptr [eax + 8H], 1
L1:
	mov     ecx, dword ptr [ebp + 8H]
	mov     dword ptr [esp], eax
	push    ecx
	push    1
	mov     ecx, dword ptr [ebp + 0CH]
	mov     eax, 004FE200H
	push    ecx
	call    eax
	mov     esp, ebp
	pop     ebp
	ret     8

FixTraceAndCopySetting@8 endp

end
