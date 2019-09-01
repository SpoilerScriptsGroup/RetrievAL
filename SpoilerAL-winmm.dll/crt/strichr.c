#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
char * __cdecl _strichr(const char *string, int c)
{
	const char *p;
	char       c1, c2;

	c1 = (char)c | ('a' - 'A');
	if ((unsigned char)(c1 - 'a') > 'z' - 'a')
		return strchr(string, c);
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

static char * __cdecl strichrSSE2(const char *string, int c);
static char * __cdecl strichr386(const char *string, int c);
static char * __cdecl strichrCPUDispatch(const char *string, int c);

static char *(__cdecl * strichrDispatch)(const char *string, int c) = strichrCPUDispatch;

__declspec(naked) char * __cdecl _strichr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strichrDispatch]
	}
}

__declspec(naked) static char * __cdecl strichrSSE2(const char *string, int c)
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
		ja      strchr
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
		jnz     epilogue

		align   16
	main_loop:
		movdqa  xmm0, xmmword ptr [eax + 16]
		add     eax, 16
		pcmpeqb xmm1, xmm0
		por     xmm0, xmm3
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

__declspec(naked) static char * __cdecl strichr386(const char *string, int c)
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
		ja      strchr
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
		mov     dl, cl
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
		test    dl, dl
		jz      retnull
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
	retnull:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_3:
		and     ecx, 80808080H
		xor     edx, -1
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
		cmp     dl, bl
		je      epilogue
		shr     esi, 9
		jc      retnull
		inc     eax
		cmp     dh, bl
		je      epilogue
		shr     esi, 8
		jc      retnull
		inc     eax
	epilogue:
		sub     eax, 4
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strichrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strichrDispatch], offset strichr386
		jmp     strichr386
	L1:
		mov     dword ptr [strichrDispatch], offset strichrSSE2
		jmp     strichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
