.486
.model flat

extrn @bcb6_std_vector_string_deallocate@8:proc
extrn _bcb6_global_operator_delete:dword
extrn _bcb6_std_allocator_deallocate:dword

public @bcb6_std_vector_string_dtor@4

.code

align 16

@bcb6_std_vector_string_dtor@4 proc near

	mov     eax, dword ptr [ecx + 16]
	mov     edx, dword ptr [ecx + 4]
	mov     ecx, dword ptr [ecx]
	push    eax
	push    ecx
	call    @bcb6_std_vector_string_deallocate@8
	pop     ecx
	pop     eax
	test    ecx, ecx
	jz      L1
	sub     eax, ecx
	mov     edx, 55555556H
	cmp     eax, 24 * 5
	jbe     L2
	push    ecx
	call    dword ptr [_bcb6_global_operator_delete]
	pop     ecx
L1:
	ret
	align   16
L2:
	mul     edx
	and     edx, not (8 - 1)
	jz      L3
	lea     eax, [edx + edx * 2]
	push    eax
	push    ecx
	call    dword ptr [_bcb6_std_allocator_deallocate]
	add     esp, 8
L3:
	ret

@bcb6_std_vector_string_dtor@4 endp

end
