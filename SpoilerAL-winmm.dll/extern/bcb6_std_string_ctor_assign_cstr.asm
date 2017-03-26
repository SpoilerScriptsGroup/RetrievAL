.486
.model flat

public @bcb6_std_string_ctor_assign_cstr@8

.code

align 16

;void __msfastcall bcb6_std_string_ctor_assign_cstr(string *dest, LPCSTR src);
@bcb6_std_string_ctor_assign_cstr@8 proc near

	sub     esp, 8
	mov     eax, 004165E8H
	push    esp
	push    edx
	push    ecx
	call    eax
	add     esp, 20
	ret

@bcb6_std_string_ctor_assign_cstr@8 endp

end
