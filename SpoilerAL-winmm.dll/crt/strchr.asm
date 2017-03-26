.686
.xmm
.model flat, c

extrn __isa_available:dword

public strchr, __from_strstr_to_strchr

.code

align 16

strchr:
	cmp     dword ptr [__isa_available], 1
	jb      strchr_sse_L3
	movzx   eax, byte ptr [esp + 8]
	mov     edx, eax
	shl     eax, 8
	or      edx, eax
	movd    xmm3, edx
	pshuflw xmm3, xmm3, 00H
	movlhps xmm3, xmm3
	mov     edx, dword ptr [esp + 4]
	mov     ecx, 15
	or      eax, -1
	and     ecx, edx
	shl     eax, cl
	sub     edx, ecx
strchr_sse_L1:
	movdqu  xmm1, xmmword ptr [edx]
	pxor    xmm2, xmm2
	pcmpeqb xmm2, xmm1
	pcmpeqb xmm1, xmm3
	por     xmm2, xmm1
	pmovmskb ecx, xmm2
	and     ecx, eax
	jnz     strchr_sse_L2
	or      eax, -1
	add     edx, 16
	jmp     strchr_sse_L1
strchr_sse_L2:
	bsf     eax, ecx
	add     eax, edx
	movd    edx, xmm3
	xor     ecx, ecx
	cmp     dl, byte ptr [eax]
	cmovne  eax, ecx
	ret
strchr_sse_L3:
	xor     eax, eax
	mov     al, byte ptr [esp + 8]
__from_strstr_to_strchr:
	push    ebx
	mov     ebx, eax
	shl     eax, 8
	mov     edx, dword ptr [esp + 8]
	test    edx, 3
	jz      main_loop_start
str_misaligned:
	mov     cl, byte ptr [edx]
	add     edx, 1
	cmp     cl, bl
	je      found_bx
	test    cl, cl
	jz      retnull_bx
	test    edx, 3
	jnz     str_misaligned
main_loop_start:
	or      ebx, eax
	push    edi
	mov     eax, ebx
	shl     ebx, 16
	push    esi
	or      ebx, eax
main_loop:
	mov     ecx, dword ptr [edx]
	mov     edi, 7EFEFEFFH
	mov     eax, ecx
	mov     esi, edi
	xor     ecx, ebx
	add     esi, eax
	add     edi, ecx
	xor     ecx, -1
	xor     eax, -1
	xor     ecx, edi
	xor     eax, esi
	add     edx, 4
	and     ecx, 81010100H
	jnz     chr_is_found
	and     eax, 81010100H
	jz      main_loop
	and     eax, 01010100H
	jnz     retnull
	and     esi, 80000000H
	jnz     main_loop
retnull:
	pop     esi
	pop     edi
retnull_bx:
	pop     ebx
	xor     eax, eax
	ret
found_bx:
	lea     eax, [edx - 1]
	pop     ebx
	ret
chr_is_found:
	mov     eax, dword ptr [edx - 4]
	cmp     al, bl
	je      byte_0
	test    al, al
	jz      retnull
	cmp     ah, bl
	je      byte_1
	test    ah, ah
	jz      retnull
	shr     eax, 16
	cmp     al, bl
	je      byte_2
	test    al, al
	jz      retnull
	cmp     ah, bl
	je      byte_3
	test    ah, ah
	jz      retnull
	jmp     main_loop
byte_3:
	pop     esi
	pop     edi
	lea     eax, [edx - 1]
	pop     ebx
	ret
byte_2:
	lea     eax, [edx - 2]
	pop     esi
	pop     edi
	pop     ebx
	ret
byte_1:
	lea     eax, [edx - 3]
	pop     esi
	pop     edi
	pop     ebx
	ret
byte_0:
	lea     eax, [edx - 4]
	pop     esi
	pop     edi
	pop     ebx
	ret

end
