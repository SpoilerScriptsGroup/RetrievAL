#include <string.h>

#ifndef _M_IX86
size_t __cdecl wcsspn(const wchar_t *string1, const wchar_t *string2)
{
	const wchar_t *p1, *p2;
	wchar_t       c1, c2;

	for (p1 = string1; c1 = *p1++; )
		for (p2 = string2; (c2 = *p2++) != c1; )
			if (!c2)
				goto DONE;
DONE:
	return (p1 - 1 - string1);
}
#else
__declspec(naked) size_t __cdecl wcsspn(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		mov     ebx, dword ptr [string1 + 4]

		align   16
	L1:
		mov     dx, word ptr [ebx]
		add     ebx, 2
		test    dx, dx
		jz      L3
		mov     ecx, dword ptr [string2 + 4]

		align   16
	L2:
		mov     ax, word ptr [ecx]
		add     ecx, 2
		cmp     ax, dx
		je      L1
		test    ax, ax
		jnz     L2
	L3:
		mov     ecx, dword ptr [string1 + 4]
		lea     eax, [ebx - 2]
		sub     eax, ecx
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
