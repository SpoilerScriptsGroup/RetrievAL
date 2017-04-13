.486
.model flat

extrn @bcb6_std_node_alloc_allocate@4:dword
extrn @bcb6_std_string_dtor@4:proc
extrn _A_memcpy:proc

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
	jmp     L6
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
	jmp     L4
L3:
	push    eax
	mov     ecx, eax
	call    @bcb6_std_node_alloc_allocate@4
L4:
	push    eax
	test    esi, esi
	jz      L5
	mov     ecx, dword ptr [ebx]
	push    esi
	push    ecx
	push    eax
	call    _A_memcpy
	add     esp, 12
L5:
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
L6:
	pop     esi
	pop     ebx
	ret

@bcb6_std_vector_push_back@8 endp

end
