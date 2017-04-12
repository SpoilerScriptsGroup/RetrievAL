.486
.model flat

extrn _A_memcpy:proc
extrn @bcb6_std_string_dtor@4:proc
extrn _bcb6_std_allocator_allocate:dword
extrn _bcb6_global_operator_new:dword

public @bcb6_std_vector_push_back@8

.code

align 16

;EXTERN_C void __msfastcall bcb6_std_vector_push_back(vector<char> *v, char c)
;{
;	v->push_back(c);
;}
@bcb6_std_vector_push_back@8 proc near

	push    ebx
	push    esi
	mov     eax, dword ptr [ecx +  4]
	mov     ebx, dword ptr [ecx + 16]
	cmp     eax, ebx
	je      L1
	mov     byte ptr [eax], dl
	inc     eax
	mov     dword ptr [ecx + 4], eax
	jmp     L7
L1:
	mov     esi, dword ptr [ecx]
	push    edx
	sub     eax, esi
	mov     ebx, ecx
	mov     esi, eax
	add     eax, eax
	jnz     L3
	inc     eax
	push    eax
	jmp     L5
L3:
	push    eax
	push    eax
	cmp     eax, 128
	jbe     L4
	call    dword ptr [_bcb6_global_operator_new]
	pop     ecx
	jmp     L5
L4:
	call    dword ptr [_bcb6_std_allocator_allocate]
	pop     ecx
L5:
	push    eax
	test    esi, esi
	jz      L6
	mov     ecx, dword ptr [ebx]
	push    esi
	push    ecx
	push    eax
	call    _A_memcpy
	add     esp, 12
L6:
	mov     ecx, ebx
	call    @bcb6_std_string_dtor@4
	pop     eax
	pop     ecx
	pop     edx
	add     esi, eax
	add     ecx, eax
	mov     byte ptr [esi], dl
	inc     esi
	mov     dword ptr [ebx     ], eax
	mov     dword ptr [ebx +  4], esi
	mov     dword ptr [ebx + 16], ecx
L7:
	pop     esi
	pop     ebx
	ret

@bcb6_std_vector_push_back@8 endp

end
