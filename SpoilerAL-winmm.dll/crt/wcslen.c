#pragma function(wcslen)

#include <string.h>

#ifndef _M_IX86
size_t __cdecl wcslen(const wchar_t *string)
{
	size_t length;

	length = 0;
	while (string[length++]);
	return --length;
}
#else
static size_t __cdecl wcslenSSE2(const wchar_t *string);
static size_t __cdecl wcslen386(const wchar_t *string);
static size_t __cdecl wcslenCPUDispatch(const wchar_t *string);

static size_t(__cdecl * wcslenDispatch)(const wchar_t *string) = wcslenCPUDispatch;

__declspec(naked) size_t __cdecl wcslen(const wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcslenDispatch]
	}
}

__declspec(naked) static size_t __cdecl wcslenSSE2(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     eax, dword ptr [string]
		mov     ecx, 15
		pxor    xmm1, xmm1
		test    eax, 1
		jnz     unaligned
		and     ecx, eax
		jz      aligned_loop_entry
		and     eax, -16
		align   16                                          // padding 5 byte
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		jnz     found_at_first

		// 16 byte aligned
		align       16
	aligned_loop:
		add     eax, 16
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      aligned_loop
		jmp     found

		align   16
	unaligned:
		mov     ecx, eax
		and     eax, -16
		inc     ecx
		dec     eax
		and     ecx, 15
		jz      unaligned_loop
		movdqa  xmm0, xmmword ptr [eax + 1]
		pslldq  xmm0, 1
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		jz      unaligned_loop
	found_at_first:
		bsf     eax, edx
		shr     eax, 1
		ret

		align   16
	unaligned_loop:
		add     eax, 16
		movdqu  xmm0, xmmword ptr [eax]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      unaligned_loop

		align   16
	found:
		bsf     edx, edx
		mov     ecx, dword ptr [string]
		add     eax, edx
		sub     eax, ecx
		shr     eax, 1
		ret

		#undef string
	}
}

__declspec(naked) static size_t __cdecl wcslen386(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     ecx, dword ptr [string]
		mov     eax, -1
		add     ecx, 2

		align   16
	loop_head:
		mov     dx, word ptr [ecx + eax * 2]
		inc     eax
		test    dx, dx
		jnz     loop_head
		ret

		#undef string
	}
}

__declspec(naked) static size_t __cdecl wcslenCPUDispatch(const wchar_t *string)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcslenDispatch], offset wcslen386
		jmp     wcslen386
	L1:
		mov     dword ptr [wcslenDispatch], offset wcslenSSE2
		jmp     wcslenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
