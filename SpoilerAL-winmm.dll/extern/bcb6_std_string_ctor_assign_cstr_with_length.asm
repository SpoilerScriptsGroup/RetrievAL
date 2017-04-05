.486
.model flat

public @bcb6_std_string_ctor_assign_cstr_with_length@12

.code

align 16

;void __msfastcall bcb6_std_string_ctor_assign_cstr_with_length(string *dest, LPCSTR src, size_t length);
@bcb6_std_string_ctor_assign_cstr_with_length@12 proc near

	sub     esp, 8
	mov     eax, 006076C8H
	push    esp
	push    edx
	push    ecx
	push    offset L1
	push    ebp
	mov     ebp, esp
	sub     esp, 36
	push    ebx
	mov     ebx, edx
	mov     edx, dword ptr [ebp + 20H]
	push    esi
	push    edi
	lea     esi, [ebx + edx]
	inc     edx
	mov     dword ptr [ecx], 0
	mov     dword ptr [ecx + 4H], 0
	mov     dword ptr [ecx + 10H], 0
	push    edx
	push    ecx
	mov     ecx, 005D54CCH
	mov     edx, dword ptr [ebp - 8H]
	push    0041663EH
	add     edx, 3
	mov     word ptr [ebp - 14H], 8
	mov     dword ptr [ebp - 8H], edx
	jmp     ecx
	align   16
L1:
	add     esp, 20
	ret     4

@bcb6_std_string_ctor_assign_cstr_with_length@12 endp

end
