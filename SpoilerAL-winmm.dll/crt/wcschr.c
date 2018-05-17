#include <windows.h>
#include <limits.h>

#ifndef _M_IX86
wchar_t *wcschr(const wchar_t *string, wint_t c)
{
	wchar_t c2;

	do
		if ((c2 = *(string++)) == (wchar_t)c)
			return (wchar_t *)string - 1;
	while (c2);
	return NULL;
}
#else
__declspec(naked) wchar_t *wcschr(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [string]
		xor     ecx, ecx
		mov     cx, word ptr [c]

		align   16
	L1:
		mov     dx, word ptr [eax]
		add     eax, 2
		cmp     dx, cx
		je      L2
		test    dx, dx
		jnz     L1
		xor     eax, eax
		ret

		align   16
	L2:
		sub     eax, 2
		ret

		#undef string
		#undef c
	}
}
#endif
