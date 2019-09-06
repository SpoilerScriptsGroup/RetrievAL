#include <string.h>

#ifndef _M_IX86
char * __cdecl strchr(const char *string, int c)
{
	char c2;

	if (!c)
		return (char *)string + strlen(string);
	do
		if ((c2 = *(string++)) == (char)c)
			return (char *)string - 1;
	while (c2);
	return NULL;
}
#else
#pragma function(strlen)

static char * __cdecl strchrSSE2(const char *string, int c);
static char * __cdecl strchr386(const char *string, int c);
static char * __cdecl strchrCPUDispatch(const char *string, int c);

static char *(__cdecl * strchrDispatch)(const char *string, int c) = strchrCPUDispatch;

__declspec(naked) char * __cdecl strchr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strchrDispatch]
	}
}

__declspec(naked) static char * __cdecl strchrSSE2(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		movzx   edx, byte ptr [c]
		mov     eax, dword ptr [string]
		test    edx, edx
		jnz     chr_is_not_null
		push    eax
		push    eax
		call    strlen
		pop     edx
		pop     ecx
		add     eax, ecx
		ret

		align   16
	chr_is_not_null:
		pxor    xmm1, xmm1
		movd    xmm2, edx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		test    eax, 15
		jz      main_loop_entry
		mov     ecx, eax
		and     eax, -16
		and     ecx, 15
		or      edx, -1
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     epilogue
		pxor    xmm1, xmm1

		align   16
	main_loop:
		add     eax, 16
	main_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      main_loop
	epilogue:
		bsf     edx, edx
		mov     cl, byte ptr [eax + edx]
		add     eax, edx
		xor     edx, edx
		test    cl, cl
		cmovz   eax, edx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strchr386(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [string]
		xor     ecx, ecx
		mov     cl, byte ptr [c]
		test    cl, cl
		jnz     chr_is_not_null
		push    eax
		push    eax
		call    strlen
		pop     edx
		pop     ecx
		add     eax, ecx
		ret

		align   16
	chr_is_not_null:
		mov     edx, ecx
		push    ebx
		shl     ecx, 8
		push    esi
		or      ecx, edx
		push    edi
		mov     ebx, ecx
		shl     ecx, 16
		or      ebx, ecx
		jmp     is_aligned

		align   16
	misaligned_loop:
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		test    cl, cl
		jz      retnull
	is_aligned:
		test    eax, 3
		jnz     misaligned_loop

		align   16
	main_loop:
		mov     ecx, dword ptr [eax]
		mov     esi, 7EFEFEFFH
		mov     edx, ecx
		xor     ecx, ebx
		add     esi, ecx
		xor     ecx, -1
		xor     ecx, esi
		mov     edi, edx
		sub     edx, 01010101H
		xor     edi, -1
		and     edx, 80808080H
		add     eax, 4
		and     edx, edi
		jnz     null_is_found
		and     ecx, 81010100H
		jz      main_loop
		and     ecx, 01010100H
		jnz     byte_0_to_2
		test    esi, esi
		js      main_loop
	found:
		dec     eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	retnull:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	null_is_found:
		and     ecx, 01010100H
		jz      retnull
		test    dl, dl
		jnz     retnull
		test    ch, ch
		jnz     byte_0
		test    dh, dh
		jnz     retnull
		shl     ecx, 16
		jc      byte_1
		shr     edx, 24
		jc      retnull
		sub     eax, 2
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0_to_2:
		test    ch, ch
		jnz     byte_0
		shl     ecx, 16
		pop     edi
		sbb     eax, 2
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0:
		sub     eax, 4
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_1:
		sub     eax, 3
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strchrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strchrDispatch], offset strchr386
		jmp     strchr386
	L1:
		mov     dword ptr [strchrDispatch], offset strchrSSE2
		jmp     strchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
