.486
.model flat

extrn __bcb6_std_vector_string_deallocate:dword
extrn __bcb6_std_vector_string_destroy:dword

public @bcb6_std_vector_string_clear@4

.code

align 16

;void __msfastcall bcb6_std_vector_string_clear(vector<string> *v)
;{
;	v->clear();
;}
@bcb6_std_vector_string_clear@4 proc near

	push    ebx
	push    esi
	sub     esp, 8
	mov     ebx, ecx
	mov     eax, esp
	mov     edx, dword ptr [ecx]
	mov     esi, dword ptr [ecx + 4]
	push    0
	push    eax
	push    edx
	push    esi
	push    esi
	call    dword ptr [__bcb6_std_vector_string_destroy]
	push    esp
	push    esi
	mov     esi, eax
	push    eax
	call    dword ptr [__bcb6_std_vector_string_deallocate]
	add     esp, 12 + 20 + 8
	mov     dword ptr [ebx + 4], esi
	pop     esi
	pop     ebx
	ret

@bcb6_std_vector_string_clear@4 endp

end
