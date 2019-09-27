#include <wchar.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	int ret;

	(wchar_t *)buffer1 += count;
	(wchar_t *)buffer2 += count;
	count ^= -1;
	while (++count)
		if (ret = towlower(buffer1[count]) - towlower(buffer2[count]))
			return ret;
	return 0;
}
#else
static int __cdecl wmemicmpSSE2(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
static int __cdecl wmemicmp386(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
static int __cdecl wmemicmpCPUDispatch(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);

static int(__cdecl * wmemicmpDispatch)(const wchar_t *buffer1, const wchar_t *buffer2, size_t count) = wmemicmpCPUDispatch;

extern const wchar_t xmm_ahighW[8];
extern const wchar_t xmm_azrangeW[8];
extern const wchar_t xmm_casebitW[8];
#define ahigh   xmm_ahighW
#define azrange xmm_azrangeW
#define casebit xmm_casebitW

__declspec(naked) int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemicmpDispatch]
	}
}

#include "wmemicmp_sse2.h"

__declspec(naked) static int __cdecl wmemicmp386(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax
		mov     esi, dword ptr [buffer1 + 12]
		mov     edi, dword ptr [buffer2 + 12]
		mov     ecx, dword ptr [count + 12]
		xor     edx, edx
		lea     esi, [esi + ecx * 2]
		lea     edi, [edi + ecx * 2]
		xor     ecx, -1

		align   16
	loop_begin:
		inc     ecx
		jz      return_equal
		mov     ax, word ptr [esi + ecx * 2]
		mov     dx, word ptr [edi + ecx * 2]
		sub     eax, edx
		jz      loop_begin
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     loop_begin
		sub     edx, 'A'
		mov     eax, ebx
		jmp     secondary_to_lower

		align   16
	compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     loop_begin
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
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
	return_equal:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef buffer1
		#undef buffer2
		#undef count
	}
}

__declspec(naked) static int __cdecl wmemicmpCPUDispatch(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wmemicmpDispatch], offset wmemicmp386
		jmp     wmemicmp386
	L1:
		mov     dword ptr [wmemicmpDispatch], offset wmemicmpSSE2
		jmp     wmemicmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
