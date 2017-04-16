.486
.model flat

public @bcb6_std_string_ctor_assign_range@12

.code

align 16

;EXTERN_C void __msfastcall bcb6_std_string_ctor_assign_range(string *dest, LPCSTR first, LPCSTR last);
@bcb6_std_string_ctor_assign_range@12 proc near

	sub     esp, 8
	mov     eax, 006076C8H
	push    esp
	push    edx
	push    ecx
	push    offset L1
	push    ebp
	mov     ecx, 005D54CCH
	mov     ebp, esp
	sub     esp, 36
	push    ebx
	push    esi
	push    edi
	mov     ebx, edx
	call    ecx
	mov     eax, dword ptr [ebp + 8H]
	xor     ecx, ecx
	mov     dword ptr [eax], ecx
	mov     dword ptr [eax + 4H], ecx
	mov     dword ptr [eax + 10H], ecx
	mov     word ptr [ebp - 14H], 8
	mov     edx, dword ptr [ebp - 8H]
	mov     esi, dword ptr [ebp + 20H]
	add     edx, 3
	lea     ecx, [esi + 1]
	mov     dword ptr [ebp - 8H], edx
	sub     ecx, ebx
	mov     edx, 0041668CH
	push    ecx
	push    eax
	push    00416643H
	jmp     edx
	align   16
L1:
	add     esp, 20
	ret     4

@bcb6_std_string_ctor_assign_range@12 endp

end
