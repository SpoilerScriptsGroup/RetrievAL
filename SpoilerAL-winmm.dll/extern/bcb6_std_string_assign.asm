.486
.model flat

extrn __bcb6_std_string_assign_range:dword

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
	mov     edx, dword ptr [edx]
	push    esp
	push    eax
	push    edx
	push    ecx
	call    dword ptr [__bcb6_std_string_assign_range]
	add     esp, 24
	ret

@bcb6_std_string_assign@8 endp

end
