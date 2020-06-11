#ifndef _M_IX86
#include <string.h>

char * __cdecl _strichr(const char *string, int c)
{
	char c1, c2;

	c1 = (char)c | ('a' - 'A');
	if ((unsigned char)(c1 - 'a') > 'z' - 'a' + 1)
		return strchr(string, c);
	string--;
	do
		if (((c2 = *(++string)) | ('a' - 'A')) == c1)
			return (char *)string;
	while (c2);
	return NULL;
}
#else
#include <stddef.h>

#pragma warning(disable:4414)

extern const char xmmconst_casebitA[16];
#define casebit xmmconst_casebitA

static char * __cdecl strichrSSE2(const char *string, int c);
static char * __cdecl strichr386(const char *string, int c);
static char * __cdecl strichrCPUDispatch(const char *string, int c);

static char *(__cdecl *strichrDispatch)(const char *string, int c) = strichrCPUDispatch;

__declspec(naked) char * __cdecl _strichr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strichrDispatch]
	}
}

__declspec(naked) static char * __cdecl strichrSSE2(const char *string, int c)
{
	extern char * __cdecl strchrSSE2(const char *string, int c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     eax, dword ptr [string]
		or      ecx, 'a' - 'A'
		xor     edx, edx
		mov     dl, cl
		sub     ecx, 'a'
		cmp     cl, 'z' - 'a' + 1
		jae     strchrSSE2
		movd    xmm2, edx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebit]
		mov     ecx, eax
		or      edx, -1
		and     ecx, 15
		jz      loop_entry
		shl     edx, cl
		sub     eax, ecx
		jmp     loop_entry

		align   16
	loop_begin:
		add     eax, 16
		or      edx, -1
	loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     ecx, edx
		jz      loop_begin
		bsf     ecx, ecx
		mov     dl, byte ptr [eax + ecx]
		add     eax, ecx
		xor     ecx, ecx
		test    dl, dl
		cmovz   eax, ecx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strichr386(const char *string, int c)
{
	extern char * __cdecl strchr386(const char *string, int c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]                          // dl = search char
		mov     ecx, dword ptr [string]                     // ecx = string
		or      edx, 'a' - 'A'
		xor     eax, eax
		mov     al, dl
		sub     edx, 'a'
		cmp     dl, 'z' - 'a' + 1
		jae     strchr386
		                                                    // set all 4 bytes of ebx to [value]
		push    ebx                                         // u preserve ebx
		mov     ebx, eax                                    // v ebx = 0/0/0/c
		shl     ebx, 8                                      // u ebx = 0/0/c/0
		push    esi                                         // v preserve esi
		mov     esi, ebx                                    // u esi = 0/0/c/0
		or      ebx, eax                                    // v ebx = 0/0/c/c
		shl     ebx, 16                                     // u ebx = c/c/0/0
		or      eax, esi                                    // v eax = 0/0/c/c
		or      ebx, eax                                    // u ebx = all 4 bytes = [search char]
		mov     eax, ecx                                    // v eax = string
		push    edi                                         // u preserve edi
		nop                                                 // v nop
		and     ecx, 3
		jz      loop_begin
		xor     ecx, 3
		jz      modulo3
		dec     ecx
		jz      modulo2
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jz      retnull
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
	modulo2:
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jz      retnull
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
	modulo3:
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jz      retnull
		or      cl, 'a' - 'A'
		cmp     cl, bl
		jne     loop_begin
		jmp     found

		align   16
	loop_begin:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     edx, ecx
		or      ecx, 20202020H
		mov     esi, ecx
		xor     ecx, ebx
		sub     edx, 01010101H
		lea     edi, [ecx - 81010101H]
		xor     esi, -1
		xor     ecx, -1
		and     edx, esi
		xor     ecx, edi
		and     edx, 80808080H
		jnz     null_is_found
		and     ecx, 81010100H
		jz      loop_begin
		and     ecx, 01010100H
		jnz     byte_0_to_2
		test    edi, edi
		js      loop_begin
	found:
		dec     eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	null_is_found:
		and     ecx, 01010100H
		jz      retnull
		test    ch, ch
		jnz     byte_0
		test    dl, dl
		jnz     retnull
		and     ecx, 00010000H
		jnz     byte_1
		test    dh, dh
		jz      byte_2
	retnull:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0_to_2:
		test    ch, ch
		jnz     byte_0
		and     ecx, 00010000H
		jnz     byte_1
	byte_2:
		sub     eax, 2
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0:
		sub     eax, 4
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_1:
		sub     eax, 3
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
