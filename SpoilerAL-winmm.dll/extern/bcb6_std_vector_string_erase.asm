.486
.model flat

extrn __bcb6_std_vector_string_destroy:dword
extrn __bcb6_std_vector_string_deallocate:dword

public @bcb6_std_vector_string_erase@12

.code

align 16

;EXTERN_C void __msfastcall bcb6_std_vector_string_erase(vector<string> *v, string *first, string *last)
;{
;	v->erase(first, last);
;}
@bcb6_std_vector_string_erase@12 proc near

	push    ebx
	push    esi
	sub     esp, 8
	mov     ebx, ecx
	mov     eax, esp
	mov     esi, dword ptr [ecx + 4]
	mov     ecx, dword ptr [esp + 8 + 8 + 4]
	push    0
	push    eax
	push    edx
	push    esi
	push    ecx
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
	ret     4

@bcb6_std_vector_string_erase@12 endp

end
