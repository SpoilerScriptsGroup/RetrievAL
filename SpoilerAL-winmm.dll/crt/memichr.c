#include <memory.h>

#pragma warning(disable:4414)

#pragma function(memchr)

#ifndef _M_IX86
void * __cdecl _memichr(const void *buffer, int c, size_t count)
{
	char c1, c2;

	c1 = (char)c | ('a' - 'A');
	if ((unsigned char)(c1 - 'a') > 'z' - 'a')
		return memchr(buffer, c, count);
	while (count--)
		if (((c2 = *(((char *)buffer)++)) | ('a' - 'A')) == c1)
			return (char *)buffer - 1;
		else if (!c2)
			break;
	return NULL;
}
#else
extern const char xmmconst_casebitA[16];
#define casebit xmmconst_casebitA

static void * __cdecl memichrSSE2(const void *buffer, int c, size_t count);
static void * __cdecl memichr386(const void *buffer, int c, size_t count);
static void * __cdecl memichrCPUDispatch(const void *buffer, int c, size_t count);

static void *(__cdecl * memichrDispatch)(const void *buffer, int c, size_t count) = memichrCPUDispatch;

__declspec(naked) void * __cdecl _memichr(const void *buffer, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memichrDispatch]
	}
}

__declspec(naked) static void * __cdecl memichrSSE2(const void *buffer, int c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                  // eax = count
		mov     ecx, dword ptr [c]
		test    eax, eax                                // check if count=0
		jz      count_equal_zero                        // if count=0, leave
		or      ecx, 'a' - 'A'
		xor     edx, edx
		mov     dl, cl
		sub     ecx, 'a'
		cmp     cl, 'z' - 'a'
		ja      memchr
		movd    xmm1, edx                               // xmm1 = search char
		punpcklbw xmm1, xmm1
		pshuflw xmm1, xmm1, 0
		movlhps xmm1, xmm1
		movdqa  xmm2, xmmword ptr [casebit]
		push    ebx                                     // preserve ebx
		mov     ebx, eax                                // ebx = count
		mov     edx, dword ptr [buffer + 4]             // edx = buffer
		dec     eax                                     // eax = count - 1
		add     ebx, edx                                // ebx = end of buffer
		xor     eax, -1                                 // eax = -count
		mov     ecx, edx
		and     edx, -16
		and     ecx, 15
		jz      loop_begin
		movdqa  xmm0, xmmword ptr [edx]
		por     xmm0, xmm2
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		test    edx, edx
		jnz     found
		sub     ecx, 16
		nop                                             // padding 1 byte
		sub     eax, ecx
		jae     retnull

		align   16                                      // already aligned
	loop_begin:
		movdqa  xmm0, xmmword ptr [ebx + eax]
		por     xmm0, xmm2
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     eax, 16
		jnc     loop_begin
	retnull:
		xor     eax, eax
		pop     ebx                                     // restore ebx
	count_equal_zero:
		ret

		align   16
	found:
		bsf     edx, edx
		add     eax, edx
		jc      retnull
		add     eax, ebx
		pop     ebx                                     // restore ebx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memichr386(const void *buffer, int c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                  // eax = count
		mov     edx, dword ptr [c]                      // dl = search char
		test    eax, eax                                // check if count=0
		jz      count_equal_zero                        // if count=0, leave
		or      edx, 'a' - 'A'
		xor     ecx, ecx
		mov     cl, dl
		sub     edx, 'a'
		cmp     dl, 'z' - 'a'
		ja      memchr
		mov     edx, eax                                // u edx = count
		mov     eax, dword ptr [buffer]                 // v eax = buffer
		                                                // set all 4 bytes of ebx to [value]
		push    ebx                                     // u preserve ebx
		mov     ebx, ecx                                // v ebx = 0/0/0/c
		shl     ebx, 8                                  // u ebx = 0/0/c/0
		push    esi                                     // v preserve esi
		mov     esi, ebx                                // u esi = 0/0/c/0
		or      ebx, ecx                                // v ebx = 0/0/c/c
		shl     ebx, 16                                 // u ebx = c/c/0/0
		or      ecx, esi                                // v ecx = 0/0/c/c
		or      ebx, ecx                                // u ebx = all 4 bytes = [search char]
		mov     ecx, eax                                // v ecx = buffer
		and     ecx, 3
		jz      loop_entry
		xor     ecx, 3
		jz      modulo3
		dec     ecx
		jz      modulo2
		mov     cl, byte ptr [eax]
		inc     eax
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
		dec     edx                                     // counter--
		jz      retnull
	modulo2:
		mov     cl, byte ptr [eax]
		inc     eax
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
		dec     edx                                     // counter--
		jz      retnull
	modulo3:
		mov     cl, byte ptr [eax]
		inc     eax
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
		dec     edx                                     // counter--
		jnz     loop_entry
		jmp     retnull

		align   16
	loop_begin:
		sub     edx, 4
		jbe     retnull
	loop_entry:
		mov     ecx, dword ptr [eax]                    // read 4 bytes
		add     eax, 4
		or      ecx, 20202020H
		xor     ecx, ebx                                // ebx is byte\byte\byte\byte
		mov     esi, 7EFEFEFFH
		add     esi, ecx
		xor     ecx, -1
		xor     ecx, esi
		and     ecx, 81010100H
		jz      loop_begin
		and     ecx, 01010100H
		jnz     byte_0_to_2
		test    esi, esi
		js      loop_begin
		cmp     edx, 3
		jbe     retnull
	found:
		dec     eax
		jmp     epilogue

		align   16
	byte_0_to_2:
		test    ch, ch
		jnz     byte_0
		and     ecx, 00010000H
		jnz     byte_1
		cmp     edx, 2
		jbe     retnull
		sub     eax, 2
		jmp     epilogue

	byte_0:
		sub     eax, 4
		jmp     epilogue

	byte_1:
		dec     edx
		jz      retnull
		sub     eax, 3
		jmp     epilogue

		align   16
	retnull:
		xor     eax, eax
	epilogue:
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
	count_equal_zero:
		ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memichrCPUDispatch(const void *buffer, int c, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [memichrDispatch], offset memichr386
		jmp     memichr386
	L1:
		mov     dword ptr [memichrDispatch], offset memichrSSE2
		jmp     memichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
