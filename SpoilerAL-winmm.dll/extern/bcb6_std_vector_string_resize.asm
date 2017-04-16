.486
.model flat

extrn @bcb6_std_vector_string_deallocate@8:proc
extrn @bcb6_std_string_dtor@4:proc
extrn _bcb6_std_string_ctor:dword
extrn __bcb6_std_vector_string_destroy:dword

public @bcb6_std_vector_string_resize@8

.data

data1 label byte
	dword   00416274H
	dword   00000004H, -24
	dword   00000000H

data2 label byte
	dword   00000000H, -60
	dword   00050000H, 00000000H
	dword   00000000H, 00050008H
	dword   00000000H
	dword   data1
	dword   00050014H, 00000000H
	dword   00000000H, 00050014H
	dword   00000000H, 00000000H

.code

align 16

;EXTERN_C void __msfastcall bcb6_std_vector_string_resize(vector<string> *v, size_t n)
;{
;	v->resize(n);
;}
@bcb6_std_vector_string_resize@8 proc near

	push    ebp
	mov     ebp, esp
	sub     esp, 112
	mov     eax, offset data2
	push    ebx
	push    esi
	push    edi
	mov     ebx, ecx
	mov     ecx, 005D54CCH
	mov     esi, edx
	call    ecx
	lea     ecx, [ebp - 18H]
	push    ecx
	call    dword ptr [_bcb6_std_string_ctor]
	mov     edx, dword ptr [ebp - 20H]
	pop     ecx
	add     edx, 4
	mov     edi, dword ptr [ebx + 4H]
	mov     dword ptr [ebp - 40H], eax
	mov     eax, edi
	mov     ecx, dword ptr [ebx]
	mov     dword ptr [ebp - 20H], edx
	sub     eax, ecx
	mov     ecx, 0AAAAAAABH
	mul     ecx
	shr     edx, 4
	cmp     esi, edx
	jae     L1
	mov     eax, esi
	mov     dword ptr [ebp - 44H], edi
	shl     eax, 3
	mov     ecx, dword ptr [ebx]
	mov     dword ptr [ebp - 50H], edi
	lea     eax, [eax + eax * 2]
	push    0
	lea     edx, [ebp - 58H]
	add     eax, ecx
	push    edx
	mov     dword ptr [ebp - 48H], eax
	push    eax
	push    edi
	push    edi
	call    dword ptr [__bcb6_std_vector_string_destroy]
	add     esp, 20
	mov     edx, dword ptr [ebx + 4H]
	mov     dword ptr [ebp - 4CH], eax
	mov     ecx, eax
	call    @bcb6_std_vector_string_deallocate@8
	mov     eax, dword ptr [ebp - 4CH]
	mov     dword ptr [ebx + 4H], eax
	jmp     L2
L1:
	mov     ecx, dword ptr [ebx]
	mov     eax, dword ptr [ebx + 4H]
	sub     eax, ecx
	mov     ecx, 0AAAAAAABH
	mul     ecx
	shr     edx, 4
	sub     esi, edx
	mov     dword ptr [ebp - 70H], edi
	mov     dword ptr [ebp - 6CH], esi
	mov     eax, dword ptr [ebp - 40H]
	mov     ecx, dword ptr [ebp - 6CH]
	push    eax
	push    ecx
	push    edi
	mov     eax, 00458388H
	push    ebx
	call    eax
	add     esp, 16
L2:
	lea     ecx, [ebp - 18H]
	call    @bcb6_std_string_dtor@4
	pop     edi
	pop     esi
	pop     ebx
	mov     esp, ebp
	pop     ebp
	ret

@bcb6_std_vector_string_resize@8 endp

end
