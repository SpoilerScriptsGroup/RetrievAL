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

		mov     al, byte ptr [c]
		mov     edx, dword ptr [string]
		test    al, al
		jnz     chr_is_not_null
		push    edx
		push    edx
		call    strlen
		pop     edx
		pop     ecx
		add     eax, ecx
		ret

		align   16
	chr_is_not_null:
		movd    xmm2, al
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		mov     ecx, edx
		mov     eax, -1
		and     ecx, 15
		and     edx, -16
		shl     eax, cl
		movdqa  xmm0, xmmword ptr [edx]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     eax, ecx
		jnz     epilogue

		align   16
	main_loop:
		movdqa  xmm0, xmmword ptr [edx + 16]
		add     edx, 16
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb eax, xmm0
		test    eax, eax
		jz      main_loop
	epilogue:
		bsf     eax, eax
		mov     cl, byte ptr [edx + eax]
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
	str_misaligned:
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		test    cl, cl
		jz      retnull
	is_aligned:
		test    eax, 3
		jnz     str_misaligned

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
		test    ecx, 81010100H
		jnz     byte_0_to_3
	compare_null:
		xor     edx, -1
		sub     edi, 01010101H
		and     edx, edi
		add     eax, 4
		test    edx, 80808080H
		jz      main_loop
	retnull:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0_to_3:
		test    ecx, 01010100H
		jnz     byte_0_to_2
		test    esi, 80000000H
		jnz     compare_null
		xor     edx, -1
		sub     edi, 01010101H
		and     edx, edi
		add     eax, 4
		test    edx, 00808080H
		jnz     retnull
	found:
		dec     eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0_to_2:
		cmp     dl, bl
		je      epilogue
		test    dl, dl
		jz      retnull
		inc     eax
		cmp     dh, bl
		je      epilogue
		test    dh, dh
		jz      retnull
		inc     eax
	epilogue:
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
