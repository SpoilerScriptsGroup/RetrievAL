#define wmemchr inline_wmemchr
#include <wchar.h>
#undef wmemchr
extern wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count);

#pragma warning(disable:4414)

#ifndef _M_IX86
wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	wchar_t c1, c2;

	c1 = c | ('a' - 'A');
	if (c1 - 'a' > 'z' - 'a')
		return wmemchr(buffer, c, count);
	while (count--)
		if (((c2 = *(buffer++)) | ('a' - 'A')) == c1)
			return (wchar_t *)buffer - 1;
		else if (!c2)
			break;
	return NULL;
}
#else
extern const wchar_t xmm_casebitW[8];
#define casebit xmm_casebitW

static wchar_t * __cdecl wmemichrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemichr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemichrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl * wmemichrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemichrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemichrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wmemichrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                  // edx = count
		mov     eax, dword ptr [c]
		test    edx, edx                                // check if count=0
		jz      count_equal_zero                        // if count=0, leave
		or      eax, 'a' - 'A'
		xor     ecx, ecx
		mov     cx, ax
		sub     eax, 'a'
		cmp     ax, 'z' - 'a'
		ja      wmemchr
		movd    xmm1, ecx                               // xmm1 = search char
		pshuflw xmm1, xmm1, 0
		movlhps xmm1, xmm1
		movdqa  xmm2, xmmword ptr [casebit]
		mov     eax, dword ptr [buffer]                 // eax = buffer
		dec     edx                                     // edx = count - 1
		push    ebx                                     // preserve ebx
		lea     ebx, [eax + edx * 2 + 2]                // ebx = end of buffer
		xor     edx, -1                                 // edx = -count
		test    eax, 1
		jnz     unaligned
		mov     ecx, eax
		and     eax, -16
		and     ecx, 15
		jz      aligned_loop
		movdqa  xmm0, xmmword ptr [eax]
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		shr     eax, cl
		shr     ecx, 1
		test    eax, eax
		lea     ecx, [ecx - 8]
		jnz     found
		sub     edx, ecx
		jae     retnull_at_aligned

		align   16
	aligned_loop:                                      // already aligned
		movdqa  xmm0, xmmword ptr [ebx + edx * 2]
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		test    eax, eax
		jnz     found
		add     edx, 8
		jnc     aligned_loop
	retnull_at_aligned:
		pop     ebx                                     // restore ebx
	count_equal_zero:
		xor     eax, eax
		ret                                             // __cdecl return

		align   16
	unaligned:
		lea     ecx, [eax + 1]
		and     eax, -16
		and     ecx, 15
		jz      unaligned_loop
		movdqa  xmm0, xmmword ptr [eax]
		pslldq  xmm0, 1
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		shr     eax, cl
		shr     ecx, 1
		test    eax, eax
		lea     ecx, [ecx - 8]
		jnz     found
		sub     edx, ecx
		jb      unaligned_loop
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [ebx + edx * 2]
		por     xmm0, xmm2
		pcmpeqw xmm0, xmm1
		pmovmskb eax, xmm0
		test    eax, eax
		jnz     found
		add     edx, 8
		jnc     unaligned_loop
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		align   16
	found:
		bsf     eax, eax
		shr     eax, 1
		add     eax, edx
		jc      retnull
		lea     eax, [ebx + eax * 2]
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		align   16
	retnull:
		xor     eax, eax
		pop     ebx                                     // restore ebx
		ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemichr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]
		mov     ecx, dword ptr [c]
		test    eax, eax
		jz      retnull
		or      ecx, 'a' - 'A'
		xor     edx, edx
		mov     dx, cx                                  // edx = search char
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a'
		ja      wmemchr
		mov     ecx, eax                                // ecx = count
		mov     eax, dword ptr [buffer]                 // eax = buffer
		push    ebx                                     // preserve ebx
		sub     eax, 2                                  // eax = buffer - 1

		align   16
	loop_begin:
		mov     bx, word ptr [eax + 2]
		add     eax, 2
		or      bx, 'a' - 'A'
		cmp     bx, dx
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

__declspec(naked) static wchar_t * __cdecl wmemichrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wmemichrDispatch], offset wmemichr386
		jmp     wmemichr386
	L1:
		mov     dword ptr [wmemichrDispatch], offset wmemichrSSE2
		jmp     wmemichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
