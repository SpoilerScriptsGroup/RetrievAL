.486
.model flat

extrn @bcb6_std_node_alloc_allocate@4:proc
extrn @bcb6_std_vector_dtor@4:proc
extrn _A_memcpy:proc

public @bcb6_std_vector_DWORD_push_back@8

.code

align 16

;EXTERN_C void __msfastcall bcb6_std_vector_DWORD_push_back(vector<DWORD> *v, DWORD value)
;{
;	v->push_back(value);
;}
@bcb6_std_vector_DWORD_push_back@8 proc near

	push    ebx
	push    esi
	mov     eax, dword ptr [ecx +  4]
	mov     ebx, dword ptr [ecx + 16]
	add     eax, 4
	cmp     eax, ebx
	ja      L1
	mov     dword ptr [ecx + 4], eax
	sub     eax, 4
	mov     dword ptr [eax], edx
	jmp     L4
L1:
	sub     eax, 4
	mov     esi, dword ptr [ecx]
	push    edx
	sub     eax, esi
	mov     ebx, ecx
	mov     esi, eax
	add     eax, eax
	jnz     L2
	add     eax, 4
L2:
	push    eax
	mov     ecx, eax
	call    @bcb6_std_node_alloc_allocate@4
	push    eax
	test    esi, esi
	jz      L3
	mov     ecx, dword ptr [ebx]
	push    esi
	push    ecx
	push    eax
	call    _A_memcpy
	add     esp, 12
L3:
	mov     ecx, ebx
	call    @bcb6_std_vector_dtor@4
	pop     eax
	pop     ecx
	pop     edx
	add     esi, eax
	add     ecx, eax
	mov     dword ptr [esi], edx
	add     esi, 4
	mov     dword ptr [ebx     ], eax
	mov     dword ptr [ebx +  4], esi
	mov     dword ptr [ebx + 16], ecx
L4:
	pop     esi
	pop     ebx
	ret

@bcb6_std_vector_DWORD_push_back@8 endp

end
