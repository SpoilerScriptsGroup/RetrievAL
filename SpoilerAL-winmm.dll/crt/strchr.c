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
		or      eax, -1
		and     ecx, 15
		and     edx, -16
		shl     eax, cl
		jmp     main_loop_entry

		align   16
	main_loop:
		add     edx, 16
		or      eax, -1
	main_loop_entry:
		movdqa  xmm0, xmmword ptr [edx]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm1, xmm0
		pmovmskb ecx, xmm1
		and     eax, ecx
		jz      main_loop
		bsf     eax, eax
		add     eax, edx
		xor     ecx, ecx
		cmp     byte ptr [eax], 0
		cmove   eax, ecx
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

		mov     edx, dword ptr [string]
		xor     eax, eax
		mov     al, byte ptr [c]
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
		mov     ecx, eax
		push    ebx
		shl     eax, 8
		push    esi
		or      eax, ecx
		push    edi
		mov     ebx, eax
		shl     eax, 16
		or      ebx, eax
		jmp     is_aligned

		align   16
	str_misaligned:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, bl
		je      byte_3
		test    al, al
		jz      retnull
	is_aligned:
		test    edx, 3
		jnz     str_misaligned

		align   16
	main_loop:
		mov     eax, dword ptr [edx]
		mov     esi, 7EFEFEFFH
		mov     ecx, eax
		xor     eax, ebx
		add     esi, eax
		xor     eax, -1
		xor     eax, esi
		mov     edi, ecx
		test    eax, 81010100H
		jz      chr_is_not_found
		test    eax, 01010100H
		jnz     byte_0_to_2
		test    esi, 80000000H
		jz      byte_3
	chr_is_not_found:
		xor     ecx, -1
		sub     edi, 01010101H
		and     ecx, edi
		add     edx, 4
		test    ecx, 80808080H
		jz      main_loop
	retnull:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_3:
		lea     eax, [edx + 3]
		jmp     epilogue

		align   16
	byte_0_to_2:
		mov     ecx, eax
		mov     eax, edx
		test    ecx, 00000100H
		jnz     epilogue
		inc     eax
		test    ecx, 00010000H
		jnz     epilogue
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
