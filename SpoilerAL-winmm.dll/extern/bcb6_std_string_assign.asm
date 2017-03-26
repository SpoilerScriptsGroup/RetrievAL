.486
.model flat

public @bcb6_std_string_assign@8

.code

align 16

;void __msfastcall bcb6_std_string_assign(string *dest, string *src)
;{
;	*dest = *src;
;}
@bcb6_std_string_assign@8 proc near

	sub     esp, 8
	mov     eax, dword ptr [edx + 4]
	push    esp
	push    eax
	mov     edx, dword ptr [edx]
	mov     eax, 004159D4H
	push    edx
	push    ecx
	call    eax
	add     esp, 24
	ret

@bcb6_std_string_assign@8 endp

end
