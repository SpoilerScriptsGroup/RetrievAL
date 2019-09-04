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

		mov     dl, byte ptr [c]
		mov     eax, dword ptr [string]
		test    dl, dl
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
		movd    xmm2, dl
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		mov     ecx, eax
		mov     edx, -1
		and     ecx, 15
		and     eax, -16
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     epilogue
		pxor    xmm1, xmm1

		align   16
	main_loop:
		movdqa  xmm0, xmmword ptr [eax + 16]
		add     eax, 16
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
		mov     esi, dword ptr [eax]
		mov     edi, 7EFEFEFFH
		mov     ecx, esi
		xor     esi, ebx
		add     edi, esi
		xor     esi, -1
		xor     esi, edi
		mov     edx, ecx
		and     esi, 81010100H
		jz      compare_null
		and     esi, 01010100H
		jnz     byte_0_to_2
		test    edi, edi
		jns     byte_3
	compare_null:
		sub     ecx, 01010101H
		xor     edx, -1
		and     ecx, 80808080H
		add     eax, 4
		test    ecx, edx
		jz      main_loop
	retnull:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_3:
		sub     ecx, 01010101H
		xor     edx, -1
		and     ecx, 80808080H
		add     eax, 4
		test    ecx, edx
		jnz     retnull
	found:
		dec     eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0_to_2:
		cmp     cl, bl
		je      epilogue
		test    cl, cl
		jz      retnull
		inc     eax
		cmp     ch, bl
		je      epilogue
		test    ch, ch
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
