.486
.model flat

public @bcb6_std_string_append_range@12

.code

align 16

;void __msfastcall bcb6_std_string_append_range(string *s, LPCSTR first, LPCSTR last);
@bcb6_std_string_append_range@12 proc near

	mov     eax, dword ptr [esp + 4]
	sub     esp, 8
	push    esp
	push    eax
	mov     eax, 00418610H
	push    edx
	push    ecx
	call    eax
	add     esp, 24
	ret     4

@bcb6_std_string_append_range@12 endp

end
