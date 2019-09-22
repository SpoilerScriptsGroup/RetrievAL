#pragma function(wcscmp)

#include <string.h>

#ifndef _M_IX86
int __cdecl wcscmp(const wchar_t *string1, const wchar_t *string2)
{
	unsigned short c1, c2;

	do
		if ((c1 = *(string1++)) != (c2 = *(string2++)))
			return (int)c1 - (int)c2;
	while (c1);
	return 0;
}
#else
__declspec(naked) int __cdecl wcscmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		xor     eax, eax
		mov     ecx, dword ptr [string1 + 4]
		mov     ebx, dword ptr [string2 + 4]
		xor     edx, edx

		align   16
	loop_head:
		mov     ax, word ptr [ecx]
		mov     dx, word ptr [ebx]
		add     ecx, 2
		add     ebx, 2
		cmp     ax, dx
		jne     epilogue
		test    ax, ax
		jnz     loop_head
	epilogue:
		sub     eax, edx
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
