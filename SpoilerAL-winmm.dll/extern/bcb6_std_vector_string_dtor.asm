.486
.model flat

extrn __bcb6_std_vector_string_deallocate:dword
extrn _bcb6_global_operator_delete:dword
extrn _bcb6_std_allocator_deallocate:dword

public @bcb6_std_vector_string_dtor@4

.code

align 16

@bcb6_std_vector_string_dtor@4 proc near

	mov     eax, dword ptr [ecx + 16]
	mov     edx, dword ptr [ecx +  4]
	mov     ecx, dword ptr [ecx     ]
	sub     esp, 8 + 8
	test    ecx, ecx
	jz      L2
	sub     eax, ecx
	mov     dword ptr [esp +  8], ecx
	mov     dword ptr [esp + 12], eax
	push    esp
	push    edx
	push    ecx
	call    dword ptr [__bcb6_std_vector_string_deallocate]
	mov     eax, dword ptr [esp + 12 + 12]
	add     esp, 12 + 8
	cmp     eax, 128
	jbe     L1
	call    dword ptr [_bcb6_global_operator_delete]
	add     esp, 4 + 4
	ret
	align   16
L1:
	call    dword ptr [_bcb6_std_allocator_deallocate]
	add     esp, 8
	ret
	align   16
L2:
	add     esp, 8 + 8
	ret

@bcb6_std_vector_string_dtor@4 endp

end
