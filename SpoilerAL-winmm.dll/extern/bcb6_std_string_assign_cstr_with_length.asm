.486
.model flat

extrn __bcb6_std_string_assign_range:dword

public @bcb6_std_string_assign_cstr_with_length@12

.code

align 16

;void __msfastcall bcb6_std_string_assign_cstr_with_length(string *dest, LPCSTR src, size_t length);
@bcb6_std_string_assign_cstr_with_length@12 proc near

	mov     eax, dword ptr [esp + 4]
	sub     esp, 8
	add     eax, edx
	push    esp
	push    eax
	push    edx
	push    ecx
	call    dword ptr [__bcb6_std_string_assign_range]
	add     esp, 24
	ret     4

@bcb6_std_string_assign_cstr_with_length@12 endp

end
