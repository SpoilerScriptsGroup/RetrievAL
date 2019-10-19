#include <memory.h>

#pragma warning(disable:4414)

extern wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count);

extern const wchar_t xmmconst_casebitW[8];
#define casebit xmmconst_casebitW

#ifndef _M_IX86
wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	wchar_t c1, c2;

	c1 = c | ('a' - 'A');
	if (c1 - 'a' > 'z' - 'a')
		return _wmemrchr(buffer, c, count);
	while (count--)
		if (((c2 = buffer[count]) | ('a' - 'A')) == c1)
			return buffer + count;
		else if (!c2)
			break;
	return NULL;
}
#else
static wchar_t * __cdecl wmemrichrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemrichr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemrichrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl * wmemrichrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemrichrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemrichrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrichrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
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
		mov     dx, cx
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a'
		ja      _wmemrchr
		movd    xmm1, edx                               // xmm1 = search char
		pshuflw xmm1, xmm1, 0
		movlhps xmm1, xmm1
		movdqa  xmm2, xmmword ptr [casebit]
		mov     ecx, dword ptr [buffer]                 // ecx = buffer
		push    ebx                                     // preserve ebx
		push    esi                                     // preserve esi
		lea     ebx, [ecx + eax * 2 - 2]                // ebx = last word of buffer
		lea     edx, [eax + eax]                        // edx = count * 2
		and     ebx, -16 or 1                           // ebx = last xmmword of buffer
		add     ecx, edx                                // ecx = end of buffer
		sub     ebx, edx                                // ebx = last xmmword of buffer - count
		mov     edx, ecx                                // edx = end of buffer
		and     ecx, 15
		jz      aligned_loop
		and     edx, 1
		jnz     unaligned
		movdqa  xmm0, xmmword ptr [ebx + eax * 2]
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		mov     esi, 3FFFH
		xor     ecx, 14
		shr     esi, cl
		and     edx, esi
		jnz     has_char_at_last_xmmword
		sub     ebx, ecx
		xor     ecx, 14
		shr     ecx, 1
		sub     ebx, 2
		sub     eax, ecx
		ja      aligned_loop
		jmp     retnull

		align   16
	aligned_loop:
		movdqa  xmm0, xmmword ptr [ebx + eax * 2]
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      aligned_loop
		jmp     retnull

		align   16
	unaligned:
		dec     ecx
		jz      unaligned_loop
		movdqa  xmm0, xmmword ptr [ebx + eax * 2 - 1]
		psrldq  xmm0, 1
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		mov     esi, 3FFFH
		xor     ecx, 14
		shr     esi, cl
		and     edx, esi
		jnz     has_char_at_last_xmmword
		sub     ebx, ecx
		xor     ecx, 14
		shr     ecx, 1
		sub     ebx, 2
		sub     eax, ecx
		ja      unaligned_loop
		jmp     retnull

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [ebx + eax * 2]
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      unaligned_loop
	retnull:
		xor     eax, eax
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
	count_equal_zero:
		ret

		align   16
	has_char_at_last_xmmword:
		cmp     eax, 8
		jae     found
		xor     ecx, 14
		lea     esi, [eax + eax]
		sub     ecx, esi
		jbe     found
		or      esi, -1
		jmp     mask_first_xmmword

		align   16
	has_char:
		cmp     eax, 8
		jae     found
		mov     ecx, ebx
		or      esi, -1
		and     ecx, 14
		jz      found
	mask_first_xmmword:
		shl     esi, cl
		and     edx, esi
		jz      retnull
	found:
		bsr     edx, edx
		pop     esi                                     // restore esi
		lea     eax, [ebx + eax * 2 - 1]
		pop     ebx                                     // restore ebx
		add     eax, edx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrichr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                  // eax = count
		mov     edx, dword ptr [c]                      // dx = search char
		test    eax, eax                                // check if count=0
		jz      retnull                                 // if count=0, leave
		or      edx, 'a' - 'A'
		xor     ecx, ecx
		mov     cx, dx
		sub     edx, 'a'
		cmp     dx, 'z' - 'a'
		ja      _wmemrchr
		push    ebx                                     // preserve ebx
		mov     ebx, ecx
		mov     ecx, eax
		add     eax, eax
		add     eax, dword ptr [buffer + 4]             // edx = buffer

		align   16
	loop_begin:
		mov     dx, word ptr [eax - 2]
		sub     eax, 2
		or      dx, 'a' - 'A'
		cmp     dx, bx
		je      found
		dec     ecx
		jnz     loop_begin
		xor     eax, eax
	found:
		pop     ebx                                     // restore ebx
	retnull:
		ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrichrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wmemrichrDispatch], offset wmemrichr386
		jmp     wmemrichr386
	L1:
		mov     dword ptr [wmemrichrDispatch], offset wmemrichrSSE2
		jmp     wmemrichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
