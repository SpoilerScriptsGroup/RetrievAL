.486
.model flat, c

public memcmp

.code

align 16

memcmp proc near

	push    ebx
	push    esi
	mov     ebx, dword ptr [esp + 12]
	mov     esi, dword ptr [esp + 16]
	mov     ecx, dword ptr [esp + 20]
	sub     ecx, 4
	jb      L2

	align   16
L1:
	mov     eax, dword ptr [ebx]
	mov     edx, dword ptr [esi]
	cmp     eax, edx
	jne     L3
	add     ebx, 4
	add     esi, 4
	sub     ecx, 4
	jae     L1
L2:
	cmp     ecx, -4
	je      L5
L3:
	mov     al, byte ptr [ebx]
	mov     dl, byte ptr [esi]
	cmp     al, dl
	jne     L4
	cmp     ecx, -3
	je      L5
	mov     al, byte ptr [ebx + 1]
	mov     dl, byte ptr [esi + 1]
	cmp     al, dl
	jne     L4
	cmp     ecx, -2
	je      L5
	mov     al, byte ptr [ebx + 2]
	mov     dl, byte ptr [esi + 2]
	cmp     al, dl
	jne     L4
	cmp     ecx, -1
	je      L5
	mov     al, byte ptr [ebx + 3]
	mov     dl, byte ptr [esi + 3]
	cmp     al, dl
	je      L5
L4:
	sbb     eax, eax
	pop     esi
	or      eax, 1
	pop     ebx
	ret

L5:
	xor     eax, eax
	pop     esi
	pop     ebx
	ret

memcmp endp

end
