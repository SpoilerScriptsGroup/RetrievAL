#include <string.h>

#ifndef _M_IX86
int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	for (; ; )
	{
		wchar_t c1, c2;

		if (!count--)
			return 0;
		c1 = *(string1++);
		c2 = *(string2++);
		if (!(c1 -= c2))
			if (c2)
				continue;
			else
				return 0;
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
		return (int)c1 - (int)c2;
	}
}
#else
__declspec(naked) int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     eax, dword ptr [count]
		mov     edx, dword ptr [string1]
		test    eax, eax
		jz      L3
		push    ebx
		push    esi
		mov     esi, dword ptr [string2 + 8]

		align   16
	L1:
		mov     cx, word ptr [edx]
		mov     bx, word ptr [esi]
		add     edx, 2
		add     esi, 2
		sub     cx, bx
		jnz     L4
		test    bx, bx
		jz      L2
		dec     eax
		jnz     L1
	L2:
		xor     eax, eax
		pop     esi
		pop     ebx
	L3:
		ret

		align   16
	L4:
		cmp     cx, 'A' - 'a'
		je      L5
		cmp     cx, 'a' - 'A'
		jne     L6
		cmp     bx, 'A'
		jl      L6
		cmp     bx, 'Z'
		jbe     L1
		jmp     L6

		align   16
	L5:
		cmp     bx, 'a'
		jl      L6
		cmp     bx, 'z'
		jbe     L1
	L6:
		add     cx, bx
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
