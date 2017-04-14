.486
.model flat

extrn __bcb6_std_string_ctor_assign_cstr:dword

public @bcb6_std_string_ctor_assign_cstr@8

.code

align 16

;void __msfastcall bcb6_std_string_ctor_assign_cstr(string *dest, LPCSTR src);
@bcb6_std_string_ctor_assign_cstr@8 proc near

	sub     esp, 8
	push    esp
	push    edx
	push    ecx
	call    dword ptr [__bcb6_std_string_ctor_assign_cstr]
	add     esp, 12 + 8
	ret

@bcb6_std_string_ctor_assign_cstr@8 endp

end
