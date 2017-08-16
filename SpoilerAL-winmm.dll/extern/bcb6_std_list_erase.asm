.486
.model flat

extrn _bcb6_std_node_alloc_deallocate:dword

public @bcb6_std_list_erase@4

.code

align 16

@bcb6_std_list_erase@4 proc near

	mov     eax, dword ptr [ecx]
	test    eax, eax
	jz      L1
	mov     ecx, dword ptr [eax]
	mov     edx, dword ptr [eax + 4H]
	mov     dword ptr [edx], ecx
	mov     dword ptr [ecx + 4H], edx
	push    12
	push    eax
	call    dword ptr [_bcb6_std_node_alloc_deallocate]
	add     esp, 8
L1:
	ret

@bcb6_std_list_erase@4 endp

end
