.486
.model flat

extrn _F004836B4:dword
extrn _F00483678:dword
extrn _F00439F10:dword

public @TMainForm_M_CustomizeClick_RedrawCalcImage@4

.code

align 16

@TMainForm_M_CustomizeClick_RedrawCalcImage@4 proc near

	push    ebx
	push    esi
	mov     ebx, ecx
	mov     esi, dword ptr [ecx + 528H]
	push    18
	push    esi
	call    dword ptr [_F004836B4]
	mov     cl, byte ptr [ebx + 4D0H]
	mov     dword ptr [esp], 18
	test    cl, cl
	jnz     L1
	inc     al
L1:
	mov     dword ptr [esp + 4], eax
	push    esi
	call    dword ptr [_F00483678]
	mov     dword ptr [esp + 4], 0048292CH
	mov     dword ptr [esp + 8], offset L2
	pop     eax
	xor     ecx, ecx
	mov     edx, 18
	mov     eax, esi
	ret
L2:
	mov     eax, dword ptr [ebx + 55CH]
	cmp     eax, 1
	je      L3
	cmp     eax, 6
	jne     L4
L3:
	mov     edx, dword ptr [ebx + 524H]
	push    edx
	push    ebx
	call    dword ptr [_F00439F10]
	add     esp, 8
L4:
	pop     esi
	pop     ebx
	ret

@TMainForm_M_CustomizeClick_RedrawCalcImage@4 endp

end
