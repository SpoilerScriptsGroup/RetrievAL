.486
.model flat

extrn _bcb6_global_operator_delete:dword
extrn _bcb6_std_allocator_deallocate:dword

public @bcb6_std_vector_dtor@4
alias <@bcb6_std_string_dtor@4> = <@bcb6_std_vector_dtor@4>

.code

align 16

@bcb6_std_vector_dtor@4 proc near

	mov     edx, dword ptr [ecx + 16]
	mov     ecx, dword ptr [ecx]
	test    ecx, ecx
	jz      L1
	sub     edx, ecx
	push    ecx
	cmp     edx, 128
	jbe     L2
	call    dword ptr [_bcb6_global_operator_delete]
	pop     ecx
L1:
	ret
	align   16
L2:
	mov     dword ptr [esp], edx
	push    ecx
	call    dword ptr [_bcb6_std_allocator_deallocate]
	add     esp, 8
	ret

@bcb6_std_vector_dtor@4 endp

end
