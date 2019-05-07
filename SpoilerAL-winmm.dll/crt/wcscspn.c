#include <string.h>

#ifndef _M_IX86
size_t __cdecl wcscspn(const wchar_t *string, const wchar_t *control)
{
	size_t        n;
	const wchar_t *p1, *p2;
	wchar_t       c1, c2;

	n = -1;
	for (p1 = string + 1; c1 = p1[n++]; )
		for (p2 = control; c2 = *(p2++); )
			if (c2 == c1)
				goto DONE;
DONE:
	return n;
}
#else
__declspec(naked) size_t __cdecl wcscspn(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [string + 8]
		mov     eax, -1
		add     ebx, 2

		align   16
	L1:
		mov     dx, word ptr [ebx + eax * 2]
		inc     eax
		test    dx, dx
		jz      L3
		mov     esi, dword ptr [control + 8]

		align   16
	L2:
		mov     cx, word ptr [esi]
		add     esi, 2
		test    cx, cx
		jz      L1
		cmp     cx, dx
		jne     L2
	L3:
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef control
	}
}
#endif
