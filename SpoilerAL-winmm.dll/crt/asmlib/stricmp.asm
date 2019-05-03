.386
.model flat

public __stricmp

.code

__stricmp proc near
	mov     ecx, dword ptr [esp + 4]    ; string1
	mov     edx, dword ptr [esp + 8]    ; string2
	sub     edx, ecx

L10:
	mov     al, byte ptr [ecx]
	cmp     al, byte ptr [ecx + edx]
	jnz     L20
	inc     ecx
	test    al, al
	jnz     L10                         ; continue with next byte

	; terminating zero found. Strings are equal
	xor     eax, eax
	ret

L20:
	; bytes are different. check case
	xor     al, 20H                     ; toggle case
	cmp     al, byte ptr [ecx + edx]
	jne     L30

	; possibly differing only by case. Check if a-z
	or      al, 20H                     ; upper case
	sub     al, 'a'
	cmp     al, 'z'-'a'
	ja      L30                         ; not a-z

	; a-z and differing only by case
	inc     ecx
	jmp     L10                         ; continue with next byte

L30:
	; bytes are different, even after changing case
	movzx   eax, byte ptr [ecx]         ; get original value again
	sub     eax, 'A'
	cmp     eax, 'Z' - 'A'
	ja      L40
	add     eax, 20H

L40:
	movzx   edx, byte ptr [ecx + edx]
	sub     edx, 'A'
	cmp     edx, 'Z' - 'A'
	ja      L50
	add     edx, 20H

L50:
	sub     eax, edx                    ; subtract to get result
	ret
__stricmp endp

end
