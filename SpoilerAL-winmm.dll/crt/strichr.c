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

extern const char    xmmconst_casebitA[16];
extern const wchar_t xmmconst_casebitW[8];
#define casebit     xmmconst_casebitA
#define insensitive xmmconst_casebitW

char * __cdecl strichrSSE42(const char *string, int c);
char * __cdecl strichrSSE2(const char *string, int c);
char * __cdecl strichr386(const char *string, int c);
static char * __cdecl strichrCPUDispatch(const char *string, int c);

static char *(__cdecl *strichrDispatch)(const char *string, int c) = strichrCPUDispatch;

__declspec(naked) char * __cdecl _strichr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strichrDispatch]
	}
}

// SSE4.2 version
__declspec(naked) char * __cdecl strichrSSE42(const char *string, int c)
{
	extern char * __cdecl strchrSSE42(const char *string, int c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [c]
		mov     ecx, dword ptr [string]
		or      eax, 'a' - 'A'
		xor     edx, edx
		mov     dl, al
		sub     eax, 'a'
		cmp     al, 'z' - 'a' + 1
		jae     strchrSSE42
		movd    xmm0, edx
		punpcklbw xmm0, xmm0
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		mov     eax, ecx
		or      edx, -1
		and     ecx, 15
		jz      loop_start
		sub     eax, ecx
		shl     edx, cl
		movdqa  xmm1, xmmword ptr [eax]
		pxor    xmm2, xmm2
		pcmpeqb xmm2, xmm1
		por     xmm1, xmmword ptr [casebit]
		pcmpeqb xmm1, xmm0
		por     xmm1, xmm2
		pmovmskb ecx, xmm1
		and     ecx, edx
		jnz     found_at_first
		add     eax, 16
	loop_start:
		pxor    xmm0, xmmword ptr [insensitive]
		jmp     loop_entry

		align   16
	loop_begin:
		add     eax, 16
	loop_entry:
		pcmpistri xmm0, xmmword ptr [eax], 00000000B
		jnbe    loop_begin
		jc      found
	not_found:
		xor     eax, eax
		ret

		align   16
	found_at_first:
		bsf     ecx, ecx
		cmp     byte ptr [eax + ecx], 0
		je      not_found
	found:
		add     eax, ecx
		ret

		#undef string
		#undef c
	}
}

// SSE2 version
__declspec(naked) char * __cdecl strichrSSE2(const char *string, int c)
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
		pxor    xmm1, xmm1
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

// 80386 version
__declspec(naked) char * __cdecl strichr386(const char *string, int c)
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
		push    esi                                         // v preserve esi
		push    edi                                         // u preserve edi
		mov     edx, eax                                    // v edx = 0/0/0/c
		shl     eax, 8                                      // u eax = 0/0/c/0
		mov     ebx, edx                                    // v ebx = 0/0/0/c
		or      edx, eax                                    // u eax = 0/0/c/c
		or      ebx, eax                                    // v ebx = 0/0/c/c
		shl     edx, 16                                     // u edx = c/c/0/0
		mov     eax, ecx                                    // v eax = string
		or      ebx, edx                                    // u ebx = all 4 bytes = [search char]
		nop                                                 // v nop
		and     ecx, 3
		jz      loop_begin
		dec     ecx
		jz      modulo1
		dec     ecx
		jz      modulo2
		mov     ch, byte ptr [eax]
		inc     eax
		mov     dh, ch
		or      ch, 'a' - 'A'
		jmp     modulo3

		align   16
	modulo1:
		mov     ecx, dword ptr [eax - 1]
		add     eax, 3
		mov     esi, ecx
		or      ecx, 20202000H
		mov     edx, ecx
		xor     ecx, ebx
		sub     esi, 01010100H
		lea     edi, [ecx - 01010100H]
		xor     edx, -1
		xor     ecx, -1
		and     edx, esi
		and     ecx, edi
		and     edx, 80808000H
		jnz     null_is_found
		and     ecx, 80808000H
		jz      loop_begin
		jmp     has_char

		align   16
	modulo2:
		mov     cx, word ptr [eax]
		add     eax, 2
		mov     edx, ecx
		or      ecx, 00002020H
		cmp     cl, bl
		je      byte_2
		test    dl, dl
		jz      retnull
	modulo3:
		cmp     ch, bl
		je      byte_3
		test    dh, dh
		jnz     loop_begin
		jmp     retnull

		align   16
	loop_begin:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     esi, ecx
		or      ecx, 20202020H
		mov     edx, ecx
		xor     ecx, ebx
		sub     esi, 01010101H
		lea     edi, [ecx - 01010101H]
		xor     edx, -1
		xor     ecx, -1
		and     edx, esi
		and     ecx, edi
		and     edx, 80808080H
		jnz     null_is_found
		and     ecx, 80808080H
		jz      loop_begin
	has_char:
		test    cx, cx
		jnz     byte_0_or_1
		and     ecx, 00800000H
		jnz     byte_2
	byte_3:
		dec     eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	null_is_found:
		and     ecx, 00808080H
		jz      retnull
		test    cl, cl
		jnz     byte_0
		test    dl, dl
		jnz     retnull
		test    ch, ch
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
	byte_0_or_1:
		test    cl, cl
		jz      byte_1
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

		align   16
	byte_2:
		sub     eax, 2
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
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [strichrDispatch], offset strichrSSE42
		jmp     strichrSSE42

	L1:
		mov     dword ptr [strichrDispatch], offset strichrSSE2
		jb      L2
		jmp     strichrSSE2

	L2:
		mov     dword ptr [strichrDispatch], offset strichr386
		jmp     strichr386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
