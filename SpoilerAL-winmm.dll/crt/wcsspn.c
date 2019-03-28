#include <string.h>

#ifndef _M_IX86
size_t __cdecl wcsspn(const wchar_t *string1, const wchar_t *string2)
{
	size_t        n;
	const wchar_t *p1, *p2;
	wchar_t       c1, c2;

	n = -1;
	for (p1 = string1 + 1; c1 = p1[n++]; )
		for (p2 = string2; (c2 = *(p2++)) != c1; )
			if (!c2)
				goto DONE;
DONE:
	return n;
}
#else
__declspec(naked) size_t __cdecl wcsspn(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [string1 + 8]
		mov     eax, -1
		add     ebx, 2

		align   16
	L1:
		mov     dx, word ptr [ebx + eax * 2]
		inc     eax
		test    dx, dx
		jz      L3
		mov     esi, dword ptr [string2 + 8]

		align   16
	L2:
		mov     cx, word ptr [esi]
		add     esi, 2
		cmp     cx, dx
		je      L1
		test    cx, cx
		jnz     L2
	L3:
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
