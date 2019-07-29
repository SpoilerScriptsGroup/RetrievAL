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

		mov     al, byte ptr [c]
		mov     edx, dword ptr [string]
		or      al, 'a' - 'A'
		mov     cl, al
		sub     al, 'a'
		cmp     al, 'z' - 'a'
		ja      strchr
		movd    xmm3, cl
		punpcklbw xmm3, xmm3
		pshuflw xmm3, xmm3, 0
		movlhps xmm3, xmm3
		movdqa  xmm4, xmm3
		psubb   xmm4, xmmword ptr [casebitA]
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

__declspec(naked) static char * __cdecl strichr386(const char *string, int c)
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
		ja      strchr

		align   16
	L1:
		mov     cl, byte ptr [eax + 1]
		inc     eax
		test    cl, cl
		jz      L2
		or      cl, 'a' - 'A'
		cmp     cl, dl
		jne     L1
		ret

		align   16
	L2:
		xor     eax, eax
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
