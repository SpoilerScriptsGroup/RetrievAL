.486
.model flat

public @bcb6_std_vector_string_dtor@4

.code

align 16

@bcb6_std_vector_string_dtor@4 proc near

	mov     eax, dword ptr [ecx + 16]
	mov     ecx, dword ptr [ecx]
	test    ecx, ecx
	jz      L1
	sub     eax, ecx
	mov     edx, 55555556H
	cmp     eax, 24 * 5
	jbe     L2
	mov     eax, 005D4484H
	push    ecx
	call    eax
	pop     ecx
L1:
	ret
	align   16
L2:
	mul     edx
	and     edx, not (8 - 1)
	jz      L3
	mov     eax, 005F47A0H
	lea     edx, [edx + edx * 2]
	push    edx
	push    ecx
	call    eax
	add     esp, 8
L3:
	ret

@bcb6_std_vector_string_dtor@4 endp

end
