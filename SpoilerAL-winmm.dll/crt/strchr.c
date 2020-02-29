#include <string.h>

#ifndef _M_IX86
char * __cdecl strchr(const char *string, int c)
{
	char c2;

	if (!c)
		return (char *)string + strlen(string);
	do
		if ((c2 = *(string++)) == (char)c)
			return (char *)string - 1;
	while (c2);
	return NULL;
}
#else
#pragma function(strlen)

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
		pxor    xmm1, xmm1
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

__declspec(naked) char * __cdecl strchr386(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		xor     eax, eax
		mov     al, byte ptr [c + 4]                    // eax = search char
		mov     ecx, dword ptr [string + 4]             // ecx = string
		test    al, al
		jz      char_is_null
		                                                // set all 4 bytes of ebx to [value]
		mov     edx, eax                                // u edx = 0/0/0/c
		push    esi                                     // v preserve esi
		shl     eax, 8                                  // u eax = 0/0/c/0
		push    edi                                     // v preserve edi
		mov     ebx, eax                                // u ebx = 0/0/c/0
		or      eax, edx                                // v eax = 0/0/c/c
		shl     eax, 16                                 // u eax = c/c/0/0
		or      ebx, edx                                // v ebx = 0/0/c/c
		or      ebx, eax                                // u ebx = all 4 bytes = [search char]
		mov     eax, ecx                                // v eax = string
		and     ecx, 3
		jz      loop_begin
		xor     ecx, 3
		jz      modulo3
		dec     ecx
		jz      modulo2
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		test    cl, cl
		jz      retnull
	modulo2:
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		test    cl, cl
		jz      retnull
	modulo3:
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		je      found
		test    cl, cl
		jnz     loop_begin
		jmp     retnull

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
	loop_begin:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     esi, ecx
		xor     ecx, ebx
		lea     edx, [esi - 01010101H]
		lea     edi, [ecx + 7EFEFEFFH]
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

__declspec(naked) static char * __cdecl strchrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strchrDispatch], offset strchr386
		jmp     strchr386

	L1:
		mov     dword ptr [strchrDispatch], offset strchrSSE2
		jmp     strchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
