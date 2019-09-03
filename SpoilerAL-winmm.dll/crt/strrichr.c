#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
char * __cdecl _strrichr(const char *string, int c)
{
	const char *p;
	char       c1, c2;

	c1 = (char)c | ('a' - 'A');
	if ((unsigned)(c1 - 'a') > 'z' - 'a')
		return strrchr(string, c);
	p = string - 1;
	do
		if (((c2 = *(++p)) | ('a' - 'A')) == c1)
			goto DONE;
	while (c2);
	p = NULL;
DONE:
	return (char *)p;
}
#else
extern const char casebitA[16];

static char * __cdecl strrichrSSE2(const char *string, int c);
static char * __cdecl strrichr386(const char *string, int c);
static char * __cdecl strrichrCPUDispatch(const char *string, int c);

static char *(__cdecl * strrichrDispatch)(const char *string, int c) = strrichrCPUDispatch;

__declspec(naked) char * __cdecl _strrichr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strrichrDispatch]
	}
}

__declspec(naked) static char * __cdecl strrichrSSE2(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     cl, byte ptr [c]
		mov     eax, dword ptr [string]
		or      cl, 'a' - 'A'
		xor     edx, edx
		mov     dl, cl
		sub     cl, 'a'
		cmp     cl, 'z' - 'a'
		ja      strrchr
		push    ebx
		push    esi
		xor     ebx, ebx
		movd    xmm2, edx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebitA]
		mov     ecx, eax
		mov     edx, -1
		and     ecx, 15
		and     eax, -16
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		por     xmm0, xmm3
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
		por     xmm0, xmm3
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

__declspec(naked) static char * __cdecl strrichr386(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     dl, byte ptr [c]
		mov     eax, dword ptr [string]
		or      dl, 'a' - 'A'
		xor     ecx, ecx
		mov     cl, dl
		sub     dl, 'a'
		cmp     dl, 'z' - 'a'
		ja      strrchr
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
		mov     dl, cl
		or      cl, 'a' - 'A'
		cmp     cl, bl
		jne     is_null
		mov     ebp, eax
		jmp     is_aligned
	is_null:
		test    dl, dl
		jz      epilogue
	is_aligned:
		test    eax, 3
		jnz     str_misaligned

		align   16
	main_loop:
		mov     esi, dword ptr [eax]
		add     eax, 4
		mov     ecx, esi
		or      esi, 20202020H
		mov     edx, esi
		xor     esi, ebx
		lea     edi, [esi - 81010101H]
		xor     esi, -1
		xor     esi, edi
		sub     ecx, 01010101H
		and     esi, 81010100H
		jz      compare_null
		and     esi, 01010100H
		jnz     byte_0_to_2
		and     edi, 80000000H
		jz      byte_3
	compare_null:
		and     ecx, 80808080H
		xor     edx, -1
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
		and     ecx, 80808080H
		xor     edx, -1
		test    ecx, edx
		jnz     epilogue
		mov     ebp, eax
		jmp     main_loop

		align   16
	byte_0_to_2:
		add     ecx, 01010101H
		cmp     dl, bl
		je      assign_0
		test    cl, cl
		jz      epilogue
		cmp     dh, bl
		je      assign_1
		test    ch, ch
		jz      epilogue
		shr     edx, 16
		mov     esi, ecx
		or      ebp, -1
		jmp     compare_3

		align   16
	assign_0:
		mov     ebp, -3
		cmp     dh, bl
		jne     is_null_1
	assign_1:
		mov     ebp, -2
		jmp     compare_2
	is_null_1:
		test    ch, ch
		jz      null_found
	compare_2:
		shr     edx, 16
		mov     esi, ecx
		cmp     dl, bl
		jne     is_null_2
		or      ebp, -1
		jmp     compare_3
	is_null_2:
		and     ecx, 00FF0000H
		jz      null_found
	compare_3:
		cmp     dh, bl
		jne     is_null_3
		mov     ebp, eax
		jmp     main_loop
	is_null_3:
		shr     esi, 24
		jz      null_found
		add     ebp, eax
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

__declspec(naked) static char * __cdecl strrichrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strrichrDispatch], offset strrichr386
		jmp     strrichr386
	L1:
		mov     dword ptr [strrichrDispatch], offset strrichrSSE2
		jmp     strrichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
