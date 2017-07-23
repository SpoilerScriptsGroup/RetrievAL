.486
.model flat

extrn _strlen:proc
extrn __bcb6_std_string_assign_range:dword

public @bcb6_std_string_assign_cstr@8

.code

align 16

;EXTERN_C void __msfastcall bcb6_std_string_assign_cstr(string *dest, LPCSTR src)
;{
;	*dest = src;
;}
@bcb6_std_string_assign_cstr@8 proc near

	push    edx
	push    ecx
	push    edx
	call    _strlen
	mov     edx, dword ptr [esp + 8]
	pop     ecx
	add     eax, edx
	mov     ecx, dword ptr [esp]
	push    esp
	push    eax
	push    edx
	push    ecx
	call    dword ptr [__bcb6_std_string_assign_range]
	add     esp, 16 + 8
	ret

@bcb6_std_string_assign_cstr@8 endp

end
