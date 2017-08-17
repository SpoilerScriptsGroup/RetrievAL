.486
.model flat

extrn @bcb6_std_allocator_deallocate@8:proc

public @bcb6_std_vector_dtor@4
alias <@bcb6_std_string_dtor@4> = <@bcb6_std_vector_dtor@4>

.code

align 16

@bcb6_std_vector_dtor@4 proc near

	mov     edx, dword ptr [ecx + 16]
	mov     ecx, dword ptr [ecx]
	sub     edx, ecx
	jmp     @bcb6_std_allocator_deallocate@8

@bcb6_std_vector_dtor@4 endp

end
