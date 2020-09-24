.486
.model flat, c

public memcmp

.code

align 16

memcmp proc near

	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]
	mov     edi, dword ptr [esp + 16]
	mov     eax, dword ptr [esp + 20]
	jmp     L2

	align   16
L1:
	mov     ecx, dword ptr [esi]
	mov     edx, dword ptr [edi]
	cmp     ecx, edx
	jne     L3
	add     esi, 4
	add     edi, 4
L2:
	sub     eax, 4
	jae     L1
	add     eax, 4
	jz      L5
L3:
	mov     cl, byte ptr [esi]
	mov     dl, byte ptr [edi]
	cmp     cl, dl
	jne     L4
	dec     eax
	jz      L5
	mov     cl, byte ptr [esi + 1]
	mov     dl, byte ptr [edi + 1]
	cmp     cl, dl
	jne     L4
	dec     eax
	jz      L5
	mov     cl, byte ptr [esi + 2]
	mov     dl, byte ptr [edi + 2]
	cmp     cl, dl
	jne     L4
	dec     eax
	jz      L5
	mov     cl, byte ptr [esi + 3]
	mov     dl, byte ptr [edi + 3]
	cmp     cl, dl
L4:
	sbb     eax, eax
	pop     edi
	or      eax, 1
	pop     esi
	ret

	align   16
L5:
	pop     edi
	pop     esi
	ret

memcmp endp

end
