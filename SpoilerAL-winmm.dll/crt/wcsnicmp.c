#include <string.h>

#ifndef _M_IX86
int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	wchar_t c1, c2;

	string1 += count;
	string2 += count;
	count = ~count;
	for (; ; )
	{
		if (!++count)
			return 0;
		c1 = string1[count];
		c2 = string2[count];
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
__declspec(naked) int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		mov     ebx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		mov     edx, dword ptr [count + 8]
		xor     eax, eax
		lea     ebx, [ebx + edx * 2]
		lea     esi, [esi + edx * 2]
		xor     edx, -1

		align   16
	L1:
		inc     edx
		jz      L2
		mov     cx, word ptr [ebx + edx * 2]
		mov     ax, word ptr [esi + edx * 2]
		sub     cx, ax
		jnz     L3
		test    ax, ax
		jnz     L1
	L2:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	L3:
		cmp     cx, 'A' - 'a'
		je      L4
		cmp     cx, 'a' - 'A'
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
		add     cx, ax
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
