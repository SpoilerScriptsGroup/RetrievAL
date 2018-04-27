#include <string.h>

#ifndef _M_IX86
int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	wchar_t c1, c2;

	for (; ; )
	{
		c1 = *(string1++);
		c2 = *(string2++);
		if (!(c1 -= c2))
		{
			if (!c2)
				return 0;
		}
		else
		{
			if (c1 == (wchar_t)(L'A' - L'a'))
			{
				if ((short)c2 >= (short)L'a' && c2 <= L'z')
					continue;
			}
			else if (c1 == (wchar_t)(L'a' - L'A'))
			{
				if ((short)c2 >= (short)L'A' && c2 <= L'Z')
					continue;
			}
			c1 += c2;
			break;
		}
	}
	return (int)c1 - (int)c2;
}
#else
__declspec(naked) int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		mov     ecx, dword ptr [string1]
		mov     edx, dword ptr [string2]
		push    ebx

		align   16
	L1:
		mov     bx, word ptr [ecx]
		mov     ax, word ptr [edx]
		add     ecx, 2
		add     edx, 2
		sub     bx, ax
		jnz     L3
		test    ax, ax
		jnz     L1
		mov     eax, 0
		pop     ebx
		ret

		align   16
	L3:
		cmp     bx, 'A' - 'a'
		je      L4
		cmp     bx, 'a' - 'A'
		jne     L5
		cmp     ax, 'A'
		jl      L5
		cmp     ax, 'Z'
		jbe     L1
		jmp     L5

		align   16
	L4:
		cmp     ax, 'a'
		jl      L5
		cmp     ax, 'z'
		jbe     L1
	L5:
		add     bx, ax
		mov     eax, ecx
		sbb     eax, eax
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
