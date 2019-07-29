#include <string.h>

#ifndef _M_IX86
size_t __cdecl wcsnlen(const wchar_t *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		while (*(string++) && --length);
		length = maxlen - length;
	}
	return length;
}
#else
static size_t __cdecl wcsnlenSSE2(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlen386(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlenCPUDispatch(const wchar_t *string, size_t maxlen);

static size_t(__cdecl * wcsnlenDispatch)(const wchar_t *string, size_t maxlen) = wcsnlenCPUDispatch;

__declspec(naked) size_t __cdecl wcsnlen(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		jmp     dword ptr [wcsnlenDispatch]
	}
}

__declspec(naked) static size_t __cdecl wcsnlenSSE2(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov         eax, dword ptr [maxlen]
		mov         ecx, dword ptr [string]
		test        eax, eax
		jz          L1
		mov         edx, ecx
		and         ecx, 15
		xor         edx, ecx
		pxor        xmm0, xmm0
		movdqa      xmm1, xmmword ptr [edx]
		test        ecx, 1
		jnz         L2
		pcmpeqw     xmm1, xmm0
		pmovmskb    eax, xmm1
		shr         eax, cl
		test        eax, eax
		jz          L3
		jmp         L5

		align       16
	L1:
		ret

		align       16
	L2:
		pcmpeqb     xmm1, xmm0
		pmovmskb    eax, xmm1
		shr         eax, cl
		inc         ecx
		add         eax, 0x1555
		dec         edx
		xor         eax, -1
		and         eax, 0x1555
		jnz         L5

		align       16
	L3:
		shr         ecx, 1
		mov         eax, 8
		sub         eax, ecx
		mov         ecx, dword ptr [maxlen]
		sub         ecx, eax
		jbe         L6
		lea         edx, [edx + ecx * 2 + 16]
		lea         ecx, [ecx * 2 - 1]
		xor         ecx, -1
		test        edx, 1
		jnz         L7

		align       16
	L4:
		movdqa      xmm1, xmmword ptr [ecx + edx]
		pcmpeqw     xmm1, xmm0
		pmovmskb    eax, xmm1
		test        eax, eax
		jnz         L8
		add         ecx, 16
		jnc         L4
		mov         eax, dword ptr [maxlen]
		ret

		align       16
	L5:
		bsf         eax, eax
		jmp         L9

		align       16
	L6:
		add         eax, ecx
		ret

		align       16
	L7:
		movdqu      xmm1, xmmword ptr [ecx + edx]
		pcmpeqw     xmm1, xmm0
		pmovmskb    eax, xmm1
		test        eax, eax
		jnz         L8
		add         ecx, 16
		jnc         L7
		mov         eax, dword ptr [maxlen]
		ret

		align       16
	L8:
		bsf         eax, eax
		add         eax, ecx
		mov         ecx, dword ptr [string]
		add         eax, edx
		sub         eax, ecx
	L9:
		shr         eax, 1
		mov         ecx, dword ptr [maxlen]
		cmp         eax, ecx
		cmova       eax, ecx
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl wcsnlen386(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     edx, dword ptr [string]
		test    eax, eax
		jz      L3

		align   16
	L1:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jz      L2
		dec     eax
		jnz     L1
	L2:
		mov     ecx, eax
		mov     eax, dword ptr [maxlen]
		sub     eax, ecx
	L3:
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl wcsnlenCPUDispatch(const wchar_t *string, size_t maxlen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsnlenDispatch], offset wcsnlen386
		jmp     wcsnlen386
	L1:
		mov     dword ptr [wcsnlenDispatch], offset wcsnlenSSE2
		jmp     wcsnlenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
