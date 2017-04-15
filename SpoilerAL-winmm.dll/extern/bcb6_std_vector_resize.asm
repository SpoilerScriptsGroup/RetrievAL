.486
.model flat

public @bcb6_std_vector_resize@8

.const

F0048D528 dword 0048D528H

.code

align 16

;void __msfastcall bcb6_std_vector_resize(vector<BYTE> *v, size_t n)
;{
;	v->resize(n);
;}
@bcb6_std_vector_resize@8 proc near

	mov     eax, dword ptr [ecx]
	sub     esp, 20
	add     edx, eax
	mov     eax, dword ptr [ecx + 4]
	cmp     eax, edx
	jb      L1
	add     esp, 20
	mov     dword ptr [ecx + 4], edx
	ret

	align   16
L1:
	mov     byte ptr [esp], 0
	sub     edx, eax
	push    esp
	push    edx
	push    eax
	push    ecx
	call    dword ptr [F0048D528]
	add     esp, 16 + 20
	ret

@bcb6_std_vector_resize@8 endp

end
