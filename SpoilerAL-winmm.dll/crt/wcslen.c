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
static size_t __cdecl wcslen_initializer(const wchar_t *string);
static size_t __cdecl wcslen386(const wchar_t *string);
static size_t __cdecl wcslenSSE2(const wchar_t *string);

static size_t(__cdecl * _imp__wcslen)(const wchar_t *string) = wcslen_initializer;

__declspec(naked) size_t __cdecl wcslen(const wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [_imp__wcslen]
	}
}

__declspec(naked) static size_t __cdecl wcslen_initializer(const wchar_t *string)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		ja      L1
		mov     dword ptr [_imp__wcslen], offset wcslen386
		jmp     wcslen386
	L1:
		mov     dword ptr [_imp__wcslen], offset wcslenSSE2
		jmp     wcslenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}

__declspec(naked) static size_t __cdecl wcslen386(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     ecx, dword ptr [string]
		xor     eax, eax

		align   16
	L1:
		mov     dx, word ptr [ecx + eax * 2]
		inc     eax
		test    dx, dx
		jnz     L1
		dec     eax
		ret

		#undef string
	}
}

__declspec(naked) static size_t __cdecl wcslenSSE2(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov         ecx, dword ptr [string]
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
		jnz         L5

		align       16
	L1:
		add         edx, 16
		movdqa      xmm1, xmmword ptr [edx]
		pcmpeqw     xmm1, xmm0
		pmovmskb    eax, xmm1
		test        eax, eax
		jz          L1
		jmp         L4

		align       16
	L2:
		pcmpeqb     xmm1, xmm0
		pmovmskb    eax, xmm1
		add         eax, 0x5555
		inc         ecx
		shr         eax, cl
		dec         edx
		and         eax, 0x1555
		jnz         L5

		align       16
	L3:
		add         edx, 16
		movdqu      xmm1, xmmword ptr [edx]
		pcmpeqw     xmm1, xmm0
		pmovmskb    eax, xmm1
		test        eax, eax
		jz          L3
	L4:
		bsf         eax, eax
		add         eax, edx
		mov         ecx, dword ptr [string]
		sub         eax, ecx
		shr         eax, 1
		ret

		align       16
	L5:
		bsf         eax, eax
		shr         eax, 1
		ret

		#undef string
	}
}
#endif
