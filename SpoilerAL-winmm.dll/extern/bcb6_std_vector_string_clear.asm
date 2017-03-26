.486
.model flat

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
	mov     ebx, ecx
	sub     esp, 20
	mov     eax, dword ptr [ebx]
	lea     ecx, [esp + 8]
	push    0
	push    ecx
	push    eax
	mov     ecx, dword ptr [ebx + 4]
	mov     eax, 00449D18H
	mov     dword ptr [esp + 12 + 16], ecx
	push    ecx
	push    ecx
	call    eax
	add     esp, 20
	mov     esi, eax
	mov     ecx, dword ptr [ebx + 4]
	mov     eax, 00415F90H
	push    esp
	push    ecx
	push    esi
	call    eax
	add     esp, 12 + 20
	mov     dword ptr [ebx + 4], esi
	pop     esi
	pop     ebx
	ret

@bcb6_std_vector_string_clear@4 endp

end
