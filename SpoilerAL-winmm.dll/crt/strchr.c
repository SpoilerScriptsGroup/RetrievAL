#include <string.h>

#ifndef _M_IX86
char * __cdecl strchr(const char *string, int c)
{
	char c2;

	if (!(char)c)
		return (char *)string + strlen(string);
	string--;
	do
		if ((c2 = *(++string)) == (char)c)
			return (char *)string;
	while (c2);
	return NULL;
}
#else
#pragma function(strlen)

char * __cdecl strchrSSE42(const char *string, int c);
char * __cdecl strchrSSE2(const char *string, int c);
char * __cdecl strchr386(const char *string, int c);
static char * __cdecl strchrCPUDispatch(const char *string, int c);

static char *(__cdecl *strchrDispatch)(const char *string, int c) = strchrCPUDispatch;

__declspec(naked) char * __cdecl strchr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strchrDispatch]
	}
}

// SSE4.2 version
__declspec(naked) char * __cdecl strchrSSE42(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]
		mov     ecx, dword ptr [string]
		test    dl, dl
		jz      char_is_null
		movd    xmm0, edx
		punpcklbw xmm0, xmm0
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		mov     eax, ecx
		or      edx, -1
		and     ecx, 15
		jz      loop_entry
		sub     eax, ecx
		shl     edx, cl
		movdqa  xmm1, xmmword ptr [eax]
		pxor    xmm2, xmm2
		pcmpeqb xmm2, xmm1
		pcmpeqb xmm1, xmm0
		por     xmm1, xmm2
		pmovmskb ecx, xmm1
		and     ecx, edx
		jz      loop_begin
		bsf     ecx, ecx
		cmp     byte ptr [eax + ecx], 0
		je      not_found
		jmp     found

		align   16
	loop_begin:
		add     eax, 16
	loop_entry:
		pcmpistri xmm0, xmmword ptr [eax], 00001000B
		jnbe    loop_begin
		jc      found
	not_found:
		xor     eax, eax
		ret

		align   16
	char_is_null:
		push    ecx
		push    ecx
		call    strlen
		pop     edx
		pop     ecx
	found:
		add     eax, ecx
		ret

		#undef string
		#undef c
	}
}

// SSE2 version
__declspec(naked) char * __cdecl strchrSSE2(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    dl, dl
		jnz     char_is_not_null
		push    eax
		push    eax
		call    strlen
		pop     edx
		pop     ecx
		add     eax, ecx
		ret

		align   16
	char_is_not_null:
		pxor    xmm1, xmm1
		movd    xmm2, edx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
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
__declspec(naked) char * __cdecl strchr386(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		xor     eax, eax
		mov     al, byte ptr [c + 4]                        // eax = search char
		mov     ecx, dword ptr [string + 4]                 // ecx = string
		test    al, al
		jz      char_is_null
		                                                    // set all 4 bytes of ebx to [value]
		mov     edx, eax                                    // u edx = 0/0/0/c
		push    esi                                         // v preserve esi
		shl     eax, 8                                      // u eax = 0/0/c/0
		push    edi                                         // v preserve edi
		mov     ebx, eax                                    // u ebx = 0/0/c/0
		or      eax, edx                                    // v eax = 0/0/c/c
		shl     eax, 16                                     // u eax = c/c/0/0
		or      ebx, edx                                    // v ebx = 0/0/c/c
		or      ebx, eax                                    // u ebx = all 4 bytes = [search char]
		mov     eax, ecx                                    // v eax = string
		and     ecx, 3
		jz      loop_begin
		dec     ecx
		jz      modulo1
		dec     ecx
		jz      modulo2
		mov     ch, byte ptr [eax]
		inc     eax
		jmp     modulo3

		align   16
	char_is_null:
		push    ecx
		push    ecx
		call    strlen
		pop     edx
		pop     ecx
		pop     ebx
		add     eax, ecx
		ret

		align   16
	modulo1:
		mov     ecx, dword ptr [eax - 1]
		add     eax, 3
		mov     edx, ecx
		xor     ecx, ebx
		lea     esi, [edx - 01010100H]
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
		cmp     cl, bl
		je      byte_2
		test    cl, cl
		jz      retnull
	modulo3:
		cmp     ch, bl
		je      byte_3
		test    ch, ch
		jnz     loop_begin
		jmp     retnull

		align   16
	loop_begin:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     edx, ecx
		xor     ecx, ebx
		lea     esi, [edx - 01010101H]
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

__declspec(naked) static char * __cdecl strchrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [strchrDispatch], offset strchrSSE42
		jmp     strchrSSE42

	L1:
		mov     dword ptr [strchrDispatch], offset strchrSSE2
		jb      L2
		jmp     strchrSSE2

	L2:
		mov     dword ptr [strchrDispatch], offset strchr386
		jmp     strchr386
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
