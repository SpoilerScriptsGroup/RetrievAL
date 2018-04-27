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

		mov     ecx, dword ptr [string]
		xor     edx, edx
		mov     dx, word ptr [c]

		align   16
	L1:
		mov     ax, word ptr [ecx]
		add     ecx, 2
		cmp     ax, dx
		je      L2
		test    ax, ax
		jnz     L1
		mov     eax, 0
		ret

		align   16
	L2:
		lea     eax, [ecx - 2]
		ret

		#undef string
		#undef c
	}
}
#endif
