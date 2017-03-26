.486
.model flat

public @bcb6_std_vector_resize@8

.code

align 16

;void __msfastcall bcb6_std_vector_resize(vector<char> *vec, size_t size)
;{
;	vec->resize(size);
;}
@bcb6_std_vector_resize@8 proc near

	push    ebx
	mov     eax, dword ptr [ecx + 4]
	sub     esp, 28 - 4
	mov     ebx, dword ptr [ecx]
	push    eax
	sub     eax, ebx
	sub     edx, eax
	jae     L1
	add     eax, ebx
	add     esp, 28
	add     eax, edx
	pop     ebx
	mov     dword ptr [ecx + 4], eax
	ret

	align   16
L1:
	mov     ebx, dword ptr [esp]
	lea     eax, [esp + 27]
	mov     byte ptr [eax], 0
	push    eax
	mov     eax, 0048D528H
	push    edx
	push    ebx
	push    ecx
	call    eax
	mov     ebx, dword ptr [esp + 16 + 28]
	add     esp, 16 + 28 + 4
	ret

@bcb6_std_vector_resize@8 endp

end
