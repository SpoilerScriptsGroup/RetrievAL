#include <string.h>

#ifndef _M_IX86
char * __cdecl strrchr(const char *string, int c)
{
	char *p, c2;

	if (!c)
		return (char *)string + strlen(string);
	p = NULL;
	while (c2 = *(string++))
		if (c2 == (char)c)
			p = (char *)string - 1;
	return p;
}
#else
#pragma function(strlen)

static char * __cdecl strrchrSSE2(const char *string, int c);
static char * __cdecl strrchr386(const char *string, int c);
static char * __cdecl strrchrCPUDispatch(const char *string, int c);

static char *(__cdecl * strrchrDispatch)(const char *string, int c) = strrchrCPUDispatch;

__declspec(naked) char * __cdecl strrchr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strrchrDispatch]
	}
}

__declspec(naked) static char * __cdecl strrchrSSE2(const char *string, int c)
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
		push    ebx
		push    esi
		mov     ecx, edx
		xor     esi, esi
		movd    xmm2, al
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		or      eax, -1
		and     ecx, 15
		shl     eax, cl
		sub     edx, ecx
		jmp     main_loop_entry

		align   16
	main_loop:
		mov     ebx, eax
		mov     eax, -2
		shl     eax, cl
		lea     esi, [edx + ecx]
		and     eax, ebx
		jnz     is_null
	main_loop_by_xmmword:
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
		jz      main_loop_by_xmmword
	is_null:
		bsf     ecx, eax
		cmp     byte ptr [edx + ecx], 0
		jne     main_loop
		mov     eax, esi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strrchr386(const char *string, int c)
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
		push    ebp
		or      eax, ecx
		push    esi
		mov     ebx, eax
		push    edi
		shl     eax, 16
		xor     ebp, ebp
		or      ebx, eax
		jmp     is_aligned

		align   16
	str_misaligned:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, bl
		jne     is_null
		lea     ebp, [edx - 1]
		jmp     is_aligned
	is_null:
		test    al, al
		jz      epilogue
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
	epilogue:
		mov     eax, ebp
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	byte_3:
		lea     ebp, [edx + 3]
		add     edx, 4
		jmp     main_loop

		align   16
	byte_0_to_2:
		mov     ebp, edx
		inc     edx
		test    eax, 00000100H
		jnz     str_misaligned
		inc     edx
		inc     ebp
		test    eax, 00010000H
		jnz     str_misaligned
		inc     edx
		inc     ebp
		jmp     str_misaligned

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strrchrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strrchrDispatch], offset strrchr386
		jmp     strrchr386
	L1:
		mov     dword ptr [strrchrDispatch], offset strrchrSSE2
		jmp     strrchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
