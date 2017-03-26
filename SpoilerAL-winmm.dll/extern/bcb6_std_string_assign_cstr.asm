.486
.model flat, c

extrn A_strlen:proc

public bcb6_std_string_assign_cstr@8

.code

align 16

;void __stdcall bcb6_std_string_assign_cstr(string *dest, LPCSTR src)
;{
;	*dest = src;
;}
bcb6_std_string_assign_cstr@8 proc near

	push    dword ptr [esp + 8]
	call    A_strlen

	; 004159D4H (string *, begin, end, out_ptr_size8)
	mov     ecx, dword ptr [esp + 8]
	mov     edx, dword ptr [esp + 12]
	push    eax
	add     eax, edx
	push    esp
	push    eax
	mov     eax, 004159D4H
	push    edx
	push    ecx
	call    eax
	add     esp, 24
	ret     8

bcb6_std_string_assign_cstr@8 endp

end
