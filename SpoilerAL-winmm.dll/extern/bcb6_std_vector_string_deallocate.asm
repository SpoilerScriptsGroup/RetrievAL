.486
.model flat

extrn __bcb6_std_vector_string_deallocate:dword

public @bcb6_std_vector_string_deallocate@8

.code

align 16

@bcb6_std_vector_string_deallocate@8 proc near

	sub     esp, 8
	push    esp
	push    edx
	push    ecx
	call    dword ptr [__bcb6_std_vector_string_deallocate]
	add     esp, 12 + 8
	ret

@bcb6_std_vector_string_deallocate@8 endp

end
