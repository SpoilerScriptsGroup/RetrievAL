#include <windows.h>

#ifndef _M_IX86
wchar_t *wcsrchr(const wchar_t *string, wint_t c)
{
	wchar_t *p, c2;

	if (!c)
		return (wchar_t *)string + wcslen(string);
	p = NULL;
	while (c2 = *(string++))
		if (c2 == (wchar_t)c)
			p = (wchar_t *)string - 1;
	return p;
}
#else
#pragma function(wcslen)
__declspec(naked) wchar_t *wcsrchr(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		xor     eax, eax
		mov     dx, word ptr [c + 4]
		mov     ebx, dword ptr [string + 4]
		test    dx, dx
		jnz     L1
		push    ebx
		call    wcslen
		add     eax, eax
		pop     ecx
		add     eax, ebx
		pop     ebx
		ret

		align   16
	L1:
		mov     cx, word ptr [ebx]
		add     ebx, 2
		test    cx, cx
		jz      L2
		cmp     cx, dx
		jne     L1
		lea     eax, [ebx - 2]
		jmp     L1

		align   16
	L2:
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
