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
		push    ebx
		xor     ebx, ebx
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
		dec     ebx
		shl     ebx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb edx, xmm0
		pxor    xmm1, xmm1
		and     ecx, ebx
		and     edx, ebx
		xor     ebx, ebx
		or      edx, ecx
		jz      main_loop_increment
		test    ecx, ecx
		jnz     null_is_found

		align   16
	main_loop:
		bsr     edx, edx
		lea     ebx, [eax + edx]
	main_loop_increment:
		add     eax, 16
	main_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      main_loop_increment
		pmovmskb ecx, xmm1
		test    ecx, ecx
		jz      main_loop
	null_is_found:
		xor     edx, ecx
		jz      epilogue
		bsf     ecx, ecx
		xor     ecx, 15
		shl     edx, cl
		sub     eax, ecx
		and     edx, 7FFFH
		jz      epilogue
		bsr     edx, edx
		add     eax, edx
		pop     ebx
		ret

		align   16
	epilogue:
		mov     eax, ebx
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
	misaligned_loop:
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		jne     is_null
		lea     ebp, [eax - 1]
		jmp     is_aligned
	is_null:
		test    cl, cl
		jz      null_is_found_at_misaligned_loop
	is_aligned:
		test    eax, 3
		jnz     misaligned_loop
		jmp     main_loop

		align   16
	null_is_found_at_misaligned_loop:
		mov     eax, ebp
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	main_loop:
		mov     ecx, dword ptr [eax]
		mov     edi, 7EFEFEFFH
		mov     esi, ecx
		xor     ecx, ebx
		add     edi, ecx
		xor     ecx, -1
		xor     ecx, edi
		mov     edx, esi
		sub     esi, 01010101H
		xor     edx, -1
		and     esi, 80808080H
		add     eax, 4
		and     esi, edx
		jnz     null_is_found
		and     ecx, 81010100H
		jz      main_loop
		and     ecx, 01010100H
		jnz     chr_is_found
		test    edi, edi
		js      main_loop
	chr_is_found:
		mov     ebp, eax
		jmp     main_loop

		align   16
	null_is_found:
		and     ecx, 81010100H
		jz      process_stored_pointer
		and     ecx, 01010100H
		jnz     null_and_chr_are_found
		test    edi, edi
		js      process_stored_pointer
	null_and_chr_are_found:
		bswap   edx
		xor     edx, -1
		add     esi, esi
		jz      compare_byte_3
		shl     esi, 8
		jz      compare_byte_2
		shl     esi, 8
		jz      compare_byte_1
		shr     edx, 16
		jmp     compare_byte_0

	compare_byte_3:
		cmp     dl, bl
		je      byte_3
	compare_byte_2:
		cmp     dh, bl
		je      byte_2
	compare_byte_1:
		shr     edx, 16
		nop
		cmp     dl, bl
		je      byte_1
	compare_byte_0:
		cmp     dh, bl
		je      byte_0
	process_stored_pointer:
		mov     eax, ebp
		test    eax, eax
		jz      restore_register
		and     ebp, 3
		jnz     restore_register
		mov     ecx, dword ptr [eax - 4]
		bswap   ecx
		cmp     cl, bl
		je      byte_3
		cmp     ch, bl
		je      byte_2
		shr     ecx, 16
		nop
		cmp     cl, bl
		je      byte_1
	byte_0:
		sub     eax, 4
		jmp     restore_register

		align   16
	byte_1:
		sub     eax, 3
		jmp     restore_register

	byte_2:
		sub     eax, 2
		jmp     restore_register

	byte_3:
		dec     eax
	restore_register:
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
