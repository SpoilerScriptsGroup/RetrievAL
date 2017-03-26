.486
.model flat

public @bcb6_std_vector_string_erase@12

.code

align 16

;void __msfastcall bcb6_std_vector_string_erase(vector<string> *vec, string *first, string *last)
;{
;	vec->erase(first, last);
;}
@bcb6_std_vector_string_erase@12 proc near

	push    ebp
	push    ebx
	mov     ebp, esp
	sub     esp, 20
	mov     ebx, ecx
	lea     eax, [ebp - 12]
	push    0
	push    eax
	mov     eax, dword ptr [ebx + 4]
	push    edx
	push    eax
	mov     ecx, dword ptr [ebp + 12]
	mov     eax, 00449D18H
	push    ecx
	call    eax
	add     esp, 20
	lea     ecx, [ebp - 20]
	push    eax
	push    ecx
	mov     edx, dword ptr [ebx + 4]
	mov     ecx, 00415F90H
	push    edx
	push    eax
	call    ecx
	mov     eax, dword ptr [esp + 12]
	add     esp, 16
	mov     dword ptr [ebx + 4], eax
	mov     esp, ebp
	pop     ebx
	pop     ebp
	ret     4

@bcb6_std_vector_string_erase@12 endp

end
