#include <wchar.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	int ret, c;

	do
		if (ret = tolower(*(string1++)) - (c = tolower(*(string2++))))
			return ret;
	while (c);
	return 0;
}
#else
static int __cdecl wcsicmpSSE2(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcsicmp386(const wchar_t *string1, const wchar_t *string2);
static int __cdecl wcsicmpCPUDispatch(const wchar_t *string1, const wchar_t *string2);

static int(__cdecl * wcsicmpDispatch)(const wchar_t *string1, const wchar_t *string2) = wcsicmpCPUDispatch;

extern const wchar_t xmm_ahighW[8];
extern const wchar_t xmm_azrangeW[8];
extern const wchar_t xmm_casebitW[8];
#define ahigh   xmm_ahighW
#define azrange xmm_azrangeW
#define casebit xmm_casebitW

__declspec(naked) int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		jmp     dword ptr [wcsicmpDispatch]
	}
}

#include "wcsicmp_sse2.h"

__declspec(naked) static int __cdecl wcsicmp386(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		xor     eax, eax
		mov     ecx, dword ptr [string1 + 4]
		mov     ebx, dword ptr [string2 + 4]
		sub     ebx, ecx

		align   16
	loop_begin:
		xor     edx, edx
		mov     ax, word ptr [ecx]
		mov     dx, word ptr [ecx + ebx]
		add     ecx, 2
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_begin
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		sub     edx, 'a'
		xor     eax, eax
		cmp     edx, 'z' - 'a'
		jbe     loop_begin
		mov     eax, edx
		add     edx, 'a' - 'A'
		jmp     secondary_to_lower

		align   16
	compare_above:
		sub     edx, 'A'
		xor     eax, eax
		cmp     edx, 'Z' - 'A'
		jbe     loop_begin
		lea     eax, [edx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	return_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      primary_to_lower
		add     edx, 'a' - 'A'
	primary_to_lower:
		cmp     eax, 'Z' - 'A'
		ja      difference
		add     eax, 'a' - 'A'
	difference:
		sub     eax, edx
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}

__declspec(naked) static int __cdecl wcsicmpCPUDispatch(const wchar_t *string1, const wchar_t *string2)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsicmpDispatch], offset wcsicmp386
		jmp     wcsicmp386
	L1:
		mov     dword ptr [wcsicmpDispatch], offset wcsicmpSSE2
		jmp     wcsicmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
