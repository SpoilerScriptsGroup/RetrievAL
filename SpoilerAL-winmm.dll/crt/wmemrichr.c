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
	buffer += count - 1;
	while (count--)
		if (((c2 = *(buffer--)) | ('a' - 'A')) == c1)
			return buffer + 1;
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
		push    ebx                                     // preserve ebx
		push    esi                                     // preserve esi
		mov     ecx, dword ptr [buffer + 8]             // ecx = buffer
		lea     esi, [eax + eax]                        // esi = count * 2
		lea     ebx, [ecx + esi - 2]                    // ebx = last word of buffer
		add     ecx, esi                                // ecx = end of buffer
		and     ebx, -16                                // ebx = aligned last word of buffer
		mov     edx, ebx                                // edx = aligned last word of buffer
		sub     ebx, esi                                // ebx = aligned last word of buffer - count
		mov     esi, ecx                                // esi = end of buffer
		and     ecx, 15
		jz      aligned_loop
		and     esi, 1
		jnz     unaligned
		movdqa  xmm0, xmmword ptr [edx]
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		mov     esi, 7FFFH
		xor     ecx, 15
		shr     esi, cl
		and     edx, esi
		jnz     found
		xor     ecx, 15
		sub     ebx, 16
		shr     ecx, 1
		sub     eax, ecx
		jb      retnull
		lea     ebx, [ebx + ecx * 2]

		align   16
	aligned_loop:
		movdqa  xmm0, xmmword ptr [ebx + eax * 2]
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     has_chr
		sub     eax, 8
		jae     aligned_loop
	retnull:
		xor     eax, eax
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
	count_equal_zero:
		ret

		align   16
	unaligned:
		inc     ebx
		dec     ecx
		jz      unaligned_loop
		movdqa  xmm0, xmmword ptr [edx]
		psrldq  xmm0, 1
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		mov     esi, 7FFFH
		xor     ecx, 15
		shr     esi, cl
		and     edx, esi
		jnz     found
		xor     ecx, 15
		sub     ebx, 16
		shr     ecx, 1
		sub     eax, ecx
		jb      retnull
		lea     ebx, [ebx + ecx * 2]

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [ebx + eax * 2]
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     has_chr
		sub     eax, 8
		jae     unaligned_loop
		xor     eax, eax
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
		ret

		align   16
	has_chr:
		cmp     eax, 8
		jae     found
		mov     ecx, ebx
		or      esi, -1
		and     ecx, 15
		shl     esi, cl
		and     edx, esi
		jz      retnull
	found:
		bsr     edx, edx
		pop     esi                                     // restore esi
		lea     eax, [ebx + eax * 2]
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