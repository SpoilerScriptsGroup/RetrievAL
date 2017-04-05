.486
.model flat

extrn @bcb6_std_string_ctor_assign_cstr_with_length@12:proc

public @bcb6_std_string_ctor_assign_range@12

.code

align 16

;void __msfastcall bcb6_std_string_ctor_assign_range(string *dest, LPCSTR first, LPCSTR last);
@bcb6_std_string_ctor_assign_range@12 proc near

	sub     dword ptr [esp + 4], edx
	jmp     @bcb6_std_string_ctor_assign_cstr_with_length@12

@bcb6_std_string_ctor_assign_range@12 endp

end
