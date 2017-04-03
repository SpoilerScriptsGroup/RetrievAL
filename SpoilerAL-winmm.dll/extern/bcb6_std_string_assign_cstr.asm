.486
.model flat

extrn _A_strlen:proc

public @bcb6_std_string_assign_cstr@8

.code

align 16

;void __msfastcall bcb6_std_string_assign_cstr(string *dest, LPCSTR src)
;{
;	*dest = src;
;}
@bcb6_std_string_assign_cstr@8 proc near

	sub     esp, 8
	push    esp
	push    edx
	push    edx
	push    ecx
	push    edx
	call    _A_strlen
	mov     edx, dword ptr [esp + 8]
	pop     ecx
	add     edx, eax
	mov     eax, 004159D4H
	mov     dword ptr [esp + 8], edx
	call    eax
	add     esp, 24
	ret

@bcb6_std_string_assign_cstr@8 endp

end
