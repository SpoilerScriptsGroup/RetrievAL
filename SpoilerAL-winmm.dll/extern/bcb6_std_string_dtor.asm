.486
.model flat

public @bcb6_std_string_dtor@4

.code

align 16

@bcb6_std_string_dtor@4 proc near

	mov     edx, dword ptr [ecx + 16]
	mov     ecx, dword ptr [ecx]
	test    ecx, ecx
	jz      L1
	sub     edx, ecx
	mov     eax, 005F47A0H
	cmp     edx, 128
	jbe     L2
	mov     eax, 005D4484H
	push    ecx
	call    eax
	pop     ecx
L1:
	ret
	align   16
L2:
	push    edx
	push    ecx
	call    eax
	add     esp, 8
	ret

@bcb6_std_string_dtor@4 endp

end
