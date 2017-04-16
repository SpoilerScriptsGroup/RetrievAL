.486
.model flat

extrn __bcb6_std_string_assign_range:dword

public @bcb6_std_string_assign_range@12

.code

align 16

;EXTERN_C void __msfastcall bcb6_std_string_assign_range(string *dest, LPCSTR first, LPCSTR last);
@bcb6_std_string_assign_range@12 proc near

	mov     eax, dword ptr [esp + 4]
	sub     esp, 8
	push    esp
	push    eax
	push    edx
	push    ecx
	call    dword ptr [__bcb6_std_string_assign_range]
	add     esp, 24
	ret     4

@bcb6_std_string_assign_range@12 endp

end
