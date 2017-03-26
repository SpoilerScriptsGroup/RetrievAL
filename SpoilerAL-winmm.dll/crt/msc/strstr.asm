.486
.xmm
.model flat, c

extrn __isa_available:dword
extrn __from_strstr_to_strchr:proc

public strstr

.code

align 16

strstr proc near

	mov     ecx, dword ptr [esp + 8]
	mov     eax, dword ptr [esp + 4]
	push    edi
	push    ebx
	push    esi
	cmp     dword ptr [__isa_available], 1
	jb      strstr_sse_L22
	ja      strstr_sse_L11
	movzx   edx, byte ptr [ecx]
	mov     ebx, edx
	shl     edx, 8
	or      edx, ebx
	jz      strstr_sse_L10
	movd    xmm3, edx
	pshuflw xmm3, xmm3, 00H
	movlhps xmm3, xmm3
	pxor    xmm0, xmm0
	mov     esi, ecx
	or      edi, -1
strstr_sse_L1:
	movzx   ebx, byte ptr [ecx]
	add     ecx, 1
	test    ebx, ebx
	jz      strstr_sse_L2
	test    ecx, 0FH
	jnz     strstr_sse_L1
	movdqa  xmm2, xmmword ptr [ecx]
	pcmpeqb xmm2, xmm0
	pmovmskb ebx, xmm2
	test    ebx, ebx
	jnz     strstr_sse_L2
	mov     edi, 15
strstr_sse_L2:
	movd    edx, xmm3
strstr_sse_L3:
	mov     ebx, 0FFFH
	and     ebx, eax
	cmp     ebx, 0FF0H
	ja      strstr_sse_L4
	movdqu  xmm1, xmmword ptr [eax]
	pxor    xmm2, xmm2
	pcmpeqb xmm2, xmm1
	pcmpeqb xmm1, xmm3
	por     xmm1, xmm2
	pmovmskb ebx, xmm1
	add     eax, 16
	test    ebx, ebx
	jz      strstr_sse_L3
	bsf     ebx, ebx
	sub     eax, 16
	add     eax, ebx
strstr_sse_L4:
	movzx   ebx, byte ptr [eax]
	test    ebx, ebx
	jz      strstr_sse_L9
	add     eax, 1
	cmp     dl, bl
	jne     strstr_sse_L3
	mov     edx, eax
	lea     ecx, [esi + 1]
strstr_sse_L5:
	mov     ebx, 0FFFH
	test    edi, ecx
	jnz     strstr_sse_L7
	and     ebx, edx
	cmp     ebx, 0FF0H
	ja      strstr_sse_L7
	movdqu  xmm1, xmmword ptr [edx]
	movdqa  xmm2, xmmword ptr [ecx]
	pcmpeqb xmm1, xmm2
	pcmpeqb xmm2, xmm0
	pcmpeqb xmm1, xmm0
	por     xmm2, xmm1
	pmovmskb ebx, xmm2
	test    ebx, ebx
	jnz     strstr_sse_L6
	add     edx, 16
	add     ecx, 16
	jmp     strstr_sse_L5
strstr_sse_L6:
	bsf     ebx, ebx
	add     edx, ebx
	add     ecx, ebx
strstr_sse_L7:
	movzx   ebx, byte ptr [ecx]
	test    ebx, ebx
	jz      strstr_sse_L8
	cmp     bl, byte ptr [edx]
	jne     strstr_sse_L2
	add     edx, 1
	add     ecx, 1
	jmp     strstr_sse_L5
strstr_sse_L8:
	dec     eax
	pop     esi
	pop     ebx
	pop     edi
	ret
strstr_sse_L9:
	xor     eax, eax
strstr_sse_L10:
	pop     esi
	pop     ebx
	pop     edi
	ret
strstr_sse_L11:
	cmp     byte ptr [ecx], 0
	je      strstr_sse_L10
	mov     edi, ecx
	mov     ebx, 0FFFH
	and     ebx, ecx
	cmp     ebx, 0FF0H
	ja      strstr_sse_L12
	movdqu  xmm0, xmmword ptr [ecx]
	jmp     strstr_sse_L16
strstr_sse_L12:
	mov     ebx, 15
strstr_sse_L13:
	movzx   esi, byte ptr [ecx]
	add     ecx, 1
strstr_sse_L14:
	pinsrb  xmm0, esi, 0FH
	psrldq  xmm0, 1
	dec     ebx
	jz      strstr_sse_L16
	test    esi, esi
	jz      strstr_sse_L14
	jmp     strstr_sse_L13
strstr_sse_L15:
	add     eax, 1
strstr_sse_L16:
	movd    edx, xmm0
strstr_sse_L17:
	mov     ebx, 0FFFH
	and     ebx, eax
	cmp     ebx, 0FF0H
	jbe     strstr_sse_L18
	movzx   ebx, byte ptr [eax]
	test    ebx, ebx
	jz      strstr_sse_L9
	cmp     dl, bl
	je      strstr_sse_L19
	add     eax, 1
	jmp     strstr_sse_L17
strstr_sse_L18:
	add     eax, 16
	pcmpistri xmm0, xmmword ptr [eax - 10H], 0CH
	ja      strstr_sse_L17
	jae     strstr_sse_L9
	sub     eax, 16
	add     eax, ecx
strstr_sse_L19:
	mov     edx, edi
	mov     esi, eax
strstr_sse_L20:
	mov     ebx, 0FFFH
	and     ebx, esi
	cmp     ebx, 0FF0H
	ja      strstr_sse_L21
	mov     ebx, 0FFFH
	and     ebx, edx
	cmp     ebx, 0FF0H
	ja      strstr_sse_L21
	movdqu  xmm1, xmmword ptr [edx]
	add     esi, 16
	add     edx, 16
	pcmpistri xmm1, xmmword ptr [esi - 10H], 0CH
	jno     strstr_sse_L15
	jns     strstr_sse_L20
	jmp     strstr_sse_L10
strstr_sse_L21:
	movzx   ebx, byte ptr [edx]
	test    ebx, ebx
	jz      strstr_sse_L10
	cmp     bl, byte ptr [esi]
	jne     strstr_sse_L15
	add     esi, 1
	add     edx, 1
	jmp     strstr_sse_L20
strstr_sse_L22:
	mov     dl, byte ptr [ecx]
	mov     edi, eax
	test    dl, dl
	jz      empty_str2
	mov     dh, byte ptr [ecx + 1]
	test    dh, dh
	jz      strchr_call
findnext:
	mov     esi, edi
	mov     ecx, dword ptr [esp + 14H]
	mov     al, byte ptr [edi]
	add     esi, 1
	cmp     al, dl
	je      first_char_found
	test    al, al
	jz      not_found
loop_start:
	mov     al, byte ptr [esi]
	add     esi, 1
in_loop:
	cmp     al, dl
	je      first_char_found
	test    al, al
	jnz     loop_start
not_found:
	pop     esi
	pop     ebx
	pop     edi
	xor     eax, eax
	ret
first_char_found:
	mov     al, byte ptr [esi]
	add     esi, 1
	cmp     al, dh
	jne     in_loop
two_first_chars_equal:
	lea     edi, [esi - 1]
compare_loop:
	mov     ah, byte ptr [ecx + 2]
	test    ah, ah
	jz      match
	mov     al, byte ptr [esi]
	add     esi, 2
	cmp     al, ah
	jne     findnext
	mov     al, byte ptr [ecx + 3]
	test    al, al
	jz      match
	mov     ah, byte ptr [esi - 1]
	add     ecx, 2
	cmp     al, ah
	je      compare_loop
	jmp     findnext
strchr_call:
	xor     eax, eax
	pop     esi
	pop     ebx
	pop     edi
	mov     al, dl
	jmp     __from_strstr_to_strchr
match:
	lea     eax, [edi - 1]
	pop     esi
	pop     ebx
	pop     edi
	ret
empty_str2:
	mov     eax, edi
	pop     esi
	pop     ebx
	pop     edi
	ret

strstr endp

end
