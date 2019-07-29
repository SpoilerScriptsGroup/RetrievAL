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

		mov     al, byte ptr [c]
		mov     edx, dword ptr [string]
		or      al, 'a' - 'A'
		mov     cl, al
		sub     al, 'a'
		cmp     al, 'z' - 'a'
		ja      strrchr
		push    ebx
		push    esi
		movd    xmm3, cl
		punpcklbw xmm3, xmm3
		pshuflw xmm3, xmm3, 0
		movlhps xmm3, xmm3
		movdqa  xmm4, xmm3
		psubb   xmm4, xmmword ptr [casebitA]
		mov     ecx, edx
		xor     esi, esi
		and     ecx, 15
		or      eax, -1
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
		movdqa  xmm1, xmmword ptr [edx]
		pxor    xmm0, xmm0
		movdqa  xmm2, xmm1
		pcmpeqb xmm0, xmm1
		pcmpeqb xmm1, xmm3
		pcmpeqb xmm2, xmm4
		por     xmm0, xmm1
		por     xmm0, xmm2
		pmovmskb ecx, xmm0
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

__declspec(naked) static char * __cdecl strrichr386(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     cl, byte ptr [c]
		mov     eax, dword ptr [string]
		or      cl, 'a' - 'A'
		dec     eax
		mov     dl, cl
		sub     cl, 'a'
		cmp     cl, 'z' - 'a'
		ja      strrchr
		push    ebx
		xor     ebx, ebx

		align   16
	main_loop:
		mov     cl, byte ptr [eax + 1]
		inc     eax
		test    cl, cl
		jz      epilogue
		or      cl, 'a' - 'A'
		cmp     cl, dl
		jne     main_loop
		mov     ebx, eax
		jmp     main_loop

		align   16
	epilogue:
		mov     eax, ebx
		pop     ebx
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
