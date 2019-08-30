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
		xor     ebx, ebx
		movd    xmm2, al
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		mov     eax, -1
		and     ecx, 15
		shl     eax, cl
		and     edx, -16
		movdqa  xmm0, xmmword ptr [edx]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     eax, ecx
		jz      main_loop_by_xmmword
		jmp     is_null

		align   16
	main_loop:
		mov     esi, eax
		mov     eax, -2
		shl     eax, cl
		lea     ebx, [edx + ecx]
		and     eax, esi
		jnz     is_null
	main_loop_by_xmmword:
		movdqa  xmm0, xmmword ptr [edx + 16]
		add     edx, 16
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb eax, xmm0
		test    eax, eax
		jz      main_loop_by_xmmword
	is_null:
		bsf     ecx, eax
		cmp     byte ptr [edx + ecx], 0
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
		xor     ebp, ebp
		or      ebx, ecx
		jmp     is_aligned

		align   16
	str_misaligned:
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		jne     is_null
		lea     ebp, [eax - 1]
		jmp     is_aligned
	is_null:
		test    cl, cl
		jz      epilogue
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
	epilogue:
		mov     eax, ebp
		pop     edi
		pop     esi
		pop     ebp
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
		jnz     epilogue
		lea     ebp, [eax - 1]
		jmp     main_loop

		align   16
	byte_0_to_2:
		cmp     dl, bl
		je      assign_0
		test    dl, dl
		jz      epilogue
		cmp     dh, bl
		je      assign_1
		test    dh, dh
		jz      epilogue
		shr     edx, 16
		jmp     assign_2

		align   16
	assign_0:
		xor     ebp, ebp
		cmp     dh, bl
		jne     is_null_1
	assign_1:
		mov     ebp, 1
		jmp     compare_2
	is_null_1:
		test    dh, dh
		jz      ret_0_to_2
	compare_2:
		shr     edx, 16
		cmp     dl, bl
		jne     is_null_2
	assign_2:
		mov     ebp, 2
		jmp     compare_3
	is_null_2:
		test    dl, dl
		jz      ret_0_to_2
	compare_3:
		cmp     dh, bl
		jne     is_null_3
		mov     ebp, 3
		jmp     next_word
	is_null_3:
		test    dh, dh
		jz      ret_0_to_2
	next_word:
		add     ebp, eax
		add     eax, 4
		jmp     main_loop

		align   16
	ret_0_to_2:
		add     eax, ebp
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
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
