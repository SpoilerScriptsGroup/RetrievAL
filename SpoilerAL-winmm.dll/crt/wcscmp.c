#include <wchar.h>
#include "PageSize.h"

#pragma function(wcscmp)

#ifndef _M_IX86
int __cdecl wcscmp(const wchar_t *string1, const wchar_t *string2)
{
	unsigned short c1, c2;

	do
		if ((c1 = *(string1++)) != (c2 = *(string2++)))
			return (int)c1 - (int)c2;
	while (c1);
	return 0;
}
#else
static int __cdecl wcscmpSSE2(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcscmp386(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcscmpCPUDispatch(const wchar_t *string1, const wchar_t *string2);

static int(__cdecl * wcscmpDispatch)(const wchar_t *string1, const wchar_t *string2) = wcscmpCPUDispatch;

__declspec(naked) int __cdecl wcscmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		jmp     dword ptr [wcscmpDispatch]
	}
}

#include "wcscmp_sse2.h"

__declspec(naked) static int __cdecl wcscmp386(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		xor     eax, eax
		mov     ecx, dword ptr [string1 + 4]
		mov     ebx, dword ptr [string2 + 4]
		xor     edx, edx

		align   16
	loop_begin:
		mov     ax, word ptr [ecx]
		mov     dx, word ptr [ebx]
		add     ecx, 2
		add     ebx, 2
		cmp     ax, dx
		jne     epilogue
		test    ax, ax
		jnz     loop_begin
	epilogue:
		sub     eax, edx
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}

__declspec(naked) static int __cdecl wcscmpCPUDispatch(const wchar_t *string1, const wchar_t *string2)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcscmpDispatch], offset wcscmp386
		jmp     wcscmp386
	L1:
		mov     dword ptr [wcscmpDispatch], offset wcscmpSSE2
		jmp     wcscmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
