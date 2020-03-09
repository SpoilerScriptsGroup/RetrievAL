#include <wchar.h>
extern wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count);
#include <xmmintrin.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	wchar_t c2;

	c2 = c | ('a' - 'A');
	if (c2 - 'a' >= 'z' - 'a' + 1)
		return _wmemrchr(buffer, c, count);
	while (count--)
		if ((buffer[count] | ('a' - 'A')) == c2)
			return buffer + count;
	return NULL;
}
#else
extern const wchar_t xmmconst_casebitW[8];
#define casebit xmmconst_casebitW

static wchar_t * __cdecl wmemrichrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemrichrSSE2(const wchar_t *buffer, __m128 c, size_t count);
static wchar_t * __cdecl wmemrichr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemrichrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl *wmemrichrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemrichrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemrichrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrichrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
{
	extern wchar_t * __cdecl wmemrchrSSE2(const wchar_t *buffer, wchar_t c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                  // edx = count
		mov     ecx, dword ptr [c]
		test    edx, edx                                // check if count == 0
		jz      retnull                                 // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     ax, cx
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a' + 1
		jae     wmemrchrSSE2
		movd    xmm0, eax                               // xmm0 = search char
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		mov     ecx, dword ptr [buffer]                 // ecx = buffer
		jmp     internal_wmemrichrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemrichrSSE2(const wchar_t *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    ebx                                     // preserve ebx
		push    esi                                     // preserve esi
		mov     eax, edx                                // eax = count
		lea     ebx, [ecx + edx * 2 - 2]                // ebx = last word of buffer
		add     edx, edx                                // edx = count * 2
		and     ebx, -16 or 1                           // ebx = last xmmword of buffer
		add     ecx, edx                                // ecx = end of buffer
		sub     ebx, edx                                // ebx = last xmmword of buffer - count
		movdqa  xmm2, xmmword ptr [casebit]
		and     ecx, 15
		jz      aligned_loop
		test    ecx, 1
		jnz     unaligned
		movdqa  xmm1, xmmword ptr [ebx + eax * 2]
		por     xmm1, xmm2
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
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
		movdqa  xmm1, xmmword ptr [ebx + eax * 2]
		por     xmm1, xmm2
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      aligned_loop
		jmp     retnull

		align   16
	unaligned:
		dec     ecx
		jz      unaligned_loop
		movdqa  xmm1, xmmword ptr [ebx + eax * 2 - 1]
		psrldq  xmm1, 1
		por     xmm1, xmm2
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
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
		movdqu  xmm1, xmmword ptr [ebx + eax * 2]
		por     xmm1, xmm2
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      unaligned_loop
	retnull:
		xor     eax, eax
		pop     esi                                     // restore esi
		pop     ebx                                     // restore ebx
		ret

		align   16
	has_char_at_last_xmmword:
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
		cmp     dx, 'z' - 'a' + 1
		jae     _wmemrchr
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
