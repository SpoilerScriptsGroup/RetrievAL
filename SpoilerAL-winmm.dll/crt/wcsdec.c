#include <stddef.h>
#include <wchar.h>
#include <intrin.h>

#ifndef _WIN64
#pragma intrinsic(_subborrow_u32)
#define _sub_uintptr(a, b, out) _subborrow_u32(0, a, b, out)
#else
#pragma intrinsic(_subborrow_u64)
#define _sub_uintptr(a, b, out) _subborrow_u64(0, a, b, out)
#endif

#ifndef _M_IX86
wchar_t * __cdecl _wcsdec(const wchar_t *start, const wchar_t *current)
{
	ptrdiff_t difference;

	return
		_sub_uintptr((size_t)start, (size_t)current, &difference) ?
			(wchar_t *)((size_t)start - (difference & -2) - 2) :
			NULL;
}
#else
__declspec(naked) wchar_t * __cdecl _wcsdec(const wchar_t *start, const wchar_t *current)
{
	__asm
	{
		#define start   (esp + 4)
		#define current (esp + 8)

		mov     eax, dword ptr [current]
		mov     ecx, dword ptr [start]
		sub     eax, ecx
		jbe     L1
		dec     eax
		and     eax, -2
		add     eax, ecx
		ret

		align   16
	L1:
		xor     eax, eax
		ret

		#undef start
		#undef current
	}
}
#endif
