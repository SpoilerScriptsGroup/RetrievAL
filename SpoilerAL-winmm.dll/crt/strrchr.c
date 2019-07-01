#include <string.h>

#pragma warning(disable:4414)

#define __ISA_AVAILABLE_X86 0

extern unsigned int __isa_available;

__declspec(naked) char * __cdecl strrchr(const char *string, int c)
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
		jz      strrchr386
		test    al, al
		jz      strchr
		push    ebx
		push    esi
		push    edi
		mov     ebx, eax
		shl     eax, 8
		mov     ecx, 15
		or      eax, edx
		mov     edx, dword ptr [string + 12]
		movd    xmm3, eax
		pshuflw xmm3, xmm3, 00H
		movlhps xmm3, xmm3
		or      eax, -1
		and     ecx, edx
		shl     eax, cl
		sub     edx, ecx
		xor     edi, edi

		align   16
	strrchrSSE2_1:
		movdqu  xmm1, xmmword ptr [edx]
		pxor    xmm2, xmm2
		pcmpeqb xmm2, xmm1
		pcmpeqb xmm1, xmm3
		por     xmm2, xmm1
		pmovmskb ecx, xmm2
		and     eax, ecx
		jz      strrchrSSE2_3
	strrchrSSE2_2:
		bsf     ecx, eax
		cmp     bl, byte ptr [edx + ecx]
		jne     strrchrSSE2_4
		mov     esi, eax
		mov     eax, -2
		shl     eax, cl
		lea     edi, [edx + ecx]
		cmp     ecx, 15
		jae     strrchrSSE2_3
		and     eax, esi
		jnz     strrchrSSE2_2
	strrchrSSE2_3:
		or      eax, -1
		add     edx, 16
		jmp     strrchrSSE2_1

		align   16
	strrchrSSE2_4:
		mov     eax, edi
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	strrchr386:
		test    al, al
		jz      strchr
		shl     eax, 8
		push    ebx
		or      eax, edx
		push    ebp
		mov     ebx, eax
		push    esi
		shl     eax, 16
		push    edi
		or      ebx, eax
		mov     edx, dword ptr [string + 16]
		xor     ebp, ebp
		jmp     is_aligned

		align   16
	str_misaligned:
		mov     cl, byte ptr [edx]
		add     edx, 1
		cmp     cl, bl
		je      byte_3
		test    cl, cl
		jz      prologue
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
		jnz     prologue
		and     esi, 80000000H
		jnz     main_loop
	prologue:
		mov     eax, ebp
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	chr_is_found:
		mov     eax, dword ptr [edx - 4]
		cmp     al, bl
		je      byte_0
		test    al, al
		jz      prologue
		cmp     ah, bl
		je      byte_1
		test    ah, ah
		jz      prologue
		shr     eax, 16
		cmp     al, bl
		je      byte_2
		test    al, al
		jz      prologue
		cmp     ah, bl
		je      byte_3
		test    ah, ah
		jz      prologue
		jmp     main_loop

		align   16
	byte_3:
		lea     ebp, [edx - 1]
		jmp     is_aligned

		align   16
	byte_2:
		lea     ebp, [edx - 2]
		dec     edx
		jmp     is_aligned

		align   16
	byte_1:
		lea     ebp, [edx - 3]
		sub     edx, 2
		jmp     is_aligned

		align   16
	byte_0:
		lea     ebp, [edx - 4]
		sub     edx, 3
		jmp     is_aligned

		#undef string
		#undef c
	}
}
