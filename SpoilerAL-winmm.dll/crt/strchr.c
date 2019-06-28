#include <string.h>

extern unsigned int __isa_available;

#define __ISA_AVAILABLE_X86 0

__declspec(naked) char * __cdecl strchr(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		xor     eax, eax
		mov     ecx, dword ptr [__isa_available]
		mov     al, byte ptr [c]
		test    ecx, ecx
		mov     edx, eax
		jz      strchr386
		shl     eax, 8
		mov     ecx, 15
		or      eax, edx
		mov     edx, dword ptr [string]
		movd    xmm3, eax
		pshuflw xmm3, xmm3, 00H
		movlhps xmm3, xmm3
		or      eax, -1
		and     ecx, edx
		shl     eax, cl
		sub     edx, ecx

		align   16
	strchrSSE2_1:
		movdqu  xmm1, xmmword ptr [edx]
		pxor    xmm2, xmm2
		pcmpeqb xmm2, xmm1
		pcmpeqb xmm1, xmm3
		por     xmm2, xmm1
		pmovmskb ecx, xmm2
		and     eax, ecx
		jnz     strchrSSE2_2
		or      eax, -1
		add     edx, 16
		jmp     strchrSSE2_1

		align   16
	strchrSSE2_2:
		bsf     eax, eax
		add     eax, edx
		movd    edx, xmm3
		xor     ecx, ecx
		cmp     dl, byte ptr [eax]
		cmovne  eax, ecx
		ret

		align   16
	strchr386:
		shl     eax, 8
		push    ebx
		or      eax, edx
		push    esi
		mov     ebx, eax
		push    edi
		shl     eax, 16
		mov     edx, dword ptr [string + 12]
		or      ebx, eax
		jmp     is_aligned

		align   16
	str_misaligned:
		mov     cl, byte ptr [edx]
		add     edx, 1
		cmp     cl, bl
		je      byte_3
		test    cl, cl
		jz      retnull
	is_aligned:
		test    edx, 3
		jnz     str_misaligned

		align   16
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
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
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

		align   16
	byte_3:
		lea     eax, [edx - 1]
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_2:
		lea     eax, [edx - 2]
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_1:
		lea     eax, [edx - 3]
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0:
		lea     eax, [edx - 4]
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
