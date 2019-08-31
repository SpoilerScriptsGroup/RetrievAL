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
		push    ebx
		push    esi
		xor     ebx, ebx
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
		jz      main_loop_by_xmmword
		jmp     is_null

		align   16
	main_loop:
		mov     esi, edx
		mov     edx, -2
		shl     edx, cl
		lea     ebx, [eax + ecx]
		and     edx, esi
		jnz     is_null
	main_loop_by_xmmword:
		movdqa  xmm0, xmmword ptr [eax + 16]
		add     eax, 16
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      main_loop_by_xmmword
	is_null:
		bsf     ecx, edx
		cmp     byte ptr [eax + ecx], 0
		jne     main_loop
		mov     eax, ebx
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
		push    ebp
		or      ecx, edx
		push    esi
		mov     ebx, ecx
		push    edi
		shl     ecx, 16
		mov     ebp, 1
		or      ebx, ecx
		jmp     is_aligned

		align   16
	str_misaligned:
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		jne     is_null
		mov     ebp, eax
		jmp     is_aligned
	is_null:
		test    cl, cl
		jz      epilogue
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
		and     edi, 80000000H
		jz      byte_3
	compare_null:
		sub     ecx, 01010101H
		xor     edx, -1
		and     ecx, 80808080H
		add     eax, 4
		test    ecx, edx
		jz      main_loop
	epilogue:
		mov     eax, ebp
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		dec     eax
		ret

		align   16
	byte_3:
		sub     ecx, 01010101H
		xor     edx, -1
		and     ecx, 80808080H
		add     eax, 4
		test    ecx, edx
		jnz     epilogue
		mov     ebp, eax
		jmp     main_loop

		align   16
	byte_0_to_2:
		cmp     cl, bl
		je      assign_0
		test    cl, cl
		jz      epilogue
		cmp     ch, bl
		je      assign_1
		test    ch, ch
		jz      epilogue
		shr     ecx, 16
		jmp     assign_2

		align   16
	assign_0:
		mov     ebp, 1
		cmp     ch, bl
		jne     is_null_1
	assign_1:
		mov     ebp, 2
		jmp     compare_2
	is_null_1:
		test    ch, ch
		jz      null_found
	compare_2:
		shr     ecx, 16
		cmp     cl, bl
		jne     is_null_2
	assign_2:
		mov     ebp, 3
		jmp     compare_3
	is_null_2:
		test    cl, cl
		jz      null_found
	compare_3:
		cmp     ch, bl
		jne     is_null_3
		mov     ebp, 4
		jmp     next_word
	is_null_3:
		test    ch, ch
		jz      null_found
	next_word:
		add     ebp, eax
		add     eax, 4
		jmp     main_loop

		align   16
	null_found:
		add     eax, ebp
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		dec     eax
		ret

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
