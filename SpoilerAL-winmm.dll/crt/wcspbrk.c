#include <string.h>

#ifndef _M_IX86
wchar_t * __cdecl wcspbrk(const wchar_t *string, const wchar_t *control)
{
	const wchar_t *p1, *p2;
	wchar_t       c1, c2;

	for (p1 = string; c1 = *(p1++); )
		for (p2 = control; c2 = *(p2++); )
			if (c2 == c1)
				return (wchar_t *)p1;
	return NULL;
}
#else
__declspec(naked) wchar_t * __cdecl wcspbrk(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     eax, dword ptr [string]
		push    ebx

		align   16
	L1:
		mov     dx, word ptr [eax]
		add     eax, 2
		test    dx, dx
		jz      L3
		mov     ebx, dword ptr [control + 4]

		align   16
	L2:
		mov     cx, word ptr [ebx]
		add     ebx, 2
		test    cx, cx
		jz      L1
		cmp     cx, dx
		jne     L2
		sub     eax, 2
		pop     ebx
		ret

		align   16
	L3:
		xor     eax, eax
		pop     ebx
		ret

		#undef string
		#undef control
	}
}
#endif
